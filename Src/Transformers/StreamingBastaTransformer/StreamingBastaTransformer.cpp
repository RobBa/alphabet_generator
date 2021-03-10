/**
 * @file DiscreteTransformer.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief This transformer transforms a stream of data of specific format (e.g. netflows) 
 * into discrete integer-value sequences in abbadingo format.
 * 
 * The sequences can for example be used as an input alphabet for state machines. 
 * 
 * @version 0.1
 * @date 2021-01-27
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "StreamingBastaTransformer.h"
#include "StreamingBastaFeatures.h"
#include "HelperFunctions.h"

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>

/**
 * @brief The constructor.
 * 
 */
StreamingBastaTransformer::StreamingBastaTransformer() : TransformerBase() {
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto& featureIniFile = globalParameters.getFeatureIni();

  // this transformer only uses the stochastic window
  delete this->window;
  this->window = new StochasticWindow();

  transformParameters = new StreamingBastaFeatures;
  dynamic_cast<StreamingBastaFeatures*>(transformParameters)->initFromFile(featureIniDir + featureIniFile);

  if(transformParameters->hasHeader()){
    // skip one line, so we don't read in header accidentally
    std::string line;
    while(line.empty()){
      HelperFunctions::resetStreamState(inputStream);
      getline(inputStream, line);
    }
  }
}

/**
 * @brief Main function where conversion takes place.
 * 
 */
void StreamingBastaTransformer::convert(){
  // TODO: check the mode (batch, stream) here, and keep on rolling the ball until finished
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto& outputFormat = dynamic_cast<StreamingBastaFeatures*>(transformParameters)->getOutputFormat();

  std::stringstream outStringStream;
  int linecount = 0;

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){
    // TODO: shall not exists
    throw new std::invalid_argument("Not implemented in StreamingTransformer.");
  }

  else if(globalParameters.getStreamMode() == StreamMode::StreamMode){
    outputStream << "0 0\n"; // to avoid clashes with flexfringe
    while(true){
      auto count = 0;
      writeEntry(outStringStream);
      outputStream << outStringStream.str();

      outStringStream.clear();
      outputStream.flush();
      ++count;
      if(count == 20){
        break;
      }
    }
  }
  else{
    throw new std::invalid_argument("Requested streaming mode not implemented");
  }
}

/**
 * @brief This method encodes streams, such as e.g. netflows. As a pre-
 * requisite, the internals should be set at this moment, e.g. the 
 * transformParameters and the featureMap.
 * 
 * Algorithm follows pattern as depicted in "Learning Behavioral Fingerprints 
 * From Netflows Using Timed Automata" (Pellegrino et al., 2017)
 * 
 * @param stream The netflow as a vector of strings.
 * @return int The encoded netflow.
 */
unsigned int StreamingBastaTransformer::encodeStream(const std::string& stream) const {
  const auto& globalParameters = GlobalParameters::getInstance();
  auto lineSplit = HelperFunctions::splitString(stream, globalParameters.getInputFileDelimiter());

  return encodeStream(lineSplit);
}

/**
 * @brief This method encodes streams, such as e.g. netflows. As a pre-
 * requisite, the internals should be set at this moment, e.g. the 
 * transformParameters and the featureMap.
 * 
 * Algorithm follows pattern as depicted in "Learning Behavioral Fingerprints 
 * From Netflows Using Timed Automata" (Pellegrino et al., 2017)
 * 
 * @param stream The netflow as a vector of strings.
 * @return int The encoded netflow.
 */
unsigned int StreamingBastaTransformer::encodeStream(const std::vector<std::string>& stream) const {
  const auto featureTypeMap = dynamic_cast<StreamingBastaFeatures*>(transformParameters)->getFeatureTypeMap();
  const auto featureIndexMap = dynamic_cast<StreamingBastaFeatures*>(transformParameters)->getFeatureindexMap();

  unsigned int res = 0;

  auto transformParametersCasted = dynamic_cast<StreamingBastaFeatures*>(transformParameters);
  if(transformParametersCasted == nullptr){
    throw new std::invalid_argument("encodeStream-method can only be used in conjunction with transformParameters of type StreamingBastaFeatures.");
  }
  else if(stream.size() == 0){
    throw new std::invalid_argument("Error: Tried to encode empty stream.");
  } 

  auto spaceSize = static_cast<unsigned int>(transformParametersCasted->getAllCategoricalDataSize()) * static_cast<unsigned int>(transformParametersCasted->getAllRangeValueSize());
  
  for(const auto& feature: featureTypeMap){
    const int index = featureIndexMap.at(feature.first);
    const auto& rawValue = stream.at(index);

    if(feature.second == FeatureBase::FeatureType::categorical){
      /* TODO: the three lines below do not make sense, since for this method we need to know our spaceSize beforehand. Either we preprocess the 
         file once, or we just give them raw */
      //if(!transformParametersCasted->hasCategoricalEntry(feature.first, rawValue)){
      //  transformParametersCasted->addCategoricalEntry(feature.first, rawValue);
      //}
      const auto factor = spaceSize / static_cast<unsigned int>(transformParametersCasted->getCategoricalDataSize(feature.first)); // to avoid an overflow
      res += transformParametersCasted->getCategoricalValue(feature.first, rawValue) * factor;
      spaceSize /= transformParametersCasted->getCategoricalDataSize(feature.first);
    }
    else if(feature.second == FeatureBase::FeatureType::rangeBased){
      const double value = std::stod(rawValue);
      const auto factor = spaceSize / static_cast<unsigned int>(transformParametersCasted->getRangeDataSize(feature.first)); // to avoid an overflow
      res += transformParametersCasted->getRangeValue(feature.first, value) * factor;
      spaceSize /= transformParametersCasted->getRangeDataSize(feature.first);
    }
  }

  return res;
}

/**
 * @brief Write an entry to the stream. Side effect: 
 * 
 * @param stream The stream to write to.
 * @return const std::string The string.
 */
void StreamingBastaTransformer::writeEntry(std::stringstream& stream){
  using SeparatedLine = std::vector<std::string>;

  const auto& globalParameters = GlobalParameters::getInstance();
  const auto featureInformation = dynamic_cast<StreamingBastaFeatures*>(transformParameters);

  const auto& outputFormat = featureInformation->getOutputFormat();
  const static auto batchSize = featureInformation->getBatchSize();
  const static auto labelIndex = featureInformation->getLabelIndex();

  // map dst-address to pair with label as int and vector of readily-split netflows
  static std::map< std::string, std::pair<int, std::vector<SeparatedLine> > > batch; 

  // collect a batch
  for(auto i = 0; i < batchSize; ++i){
    auto line = dynamic_cast<StochasticWindow*>(window)->getStreamedLine(inputStream);

    const auto lineSplit = HelperFunctions::splitString(line, globalParameters.getInputFileDelimiter(), true);
    auto& dst = batch[lineSplit.at(featureInformation->getDstIndex())];
    dst.second.push_back(lineSplit);

    if(labelIndex > -1){
      const auto label = dst.first;
      dst.first = std::max(label, featureInformation->getLabel(lineSplit.at(labelIndex)));
    }
  }

  // stream the batch
  for(auto& dst: batch){
    std::stringstream labelStream;

    const auto label = dst.second.first;
    if(labelIndex > -1){
      labelStream << label << " ";
    }

    unsigned int lineCount = 0;
    std::vector<unsigned int> symbols;

    const auto& flows = dst.second.second;
    for(const auto& flow: flows){
      if(flow.empty()){
        continue;
      }

      const auto code = encodeStream(flow);
      symbols.push_back(code);
    }
    stream << toAbbadingoFormat(symbols, labelStream.str()) << "\n";
  }
};
