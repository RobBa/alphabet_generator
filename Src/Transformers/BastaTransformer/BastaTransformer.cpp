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

#include "BastaTransformer.h"
#include "BastaFeatures.h"
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
BastaTransformer::BastaTransformer() : TransformerBase() {
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto& featureIniFile = globalParameters.getFeatureIni();

  transformParameters = new BastaFeatures;
  dynamic_cast<BastaFeatures*>(transformParameters)->initFromFile(featureIniDir + featureIniFile);

  if(transformParameters->hasHeader()){
    // skip one line, so we don't read in header accidentally
    std::string line;
    getline(inputStream, line);
  }
}

/**
 * @brief Main function where conversion takes place.
 * 
 */
void BastaTransformer::convert(){
  // TODO: check the mode (batch, stream) here, and keep on rolling the ball until finished
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto& outputFormat = globalParameters.getOutputfileFormat();

  std::stringstream outStringStream;
  int linecount = 0;

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){

    while(true){
      if(inputStream.eof()){
        break;
      }

      writeEntry(outStringStream);
      linecount++;
    }

    if(outputFormat == OutputFileFormat::Abbadingo){
      outputStream << linecount << " " << alphabetSize << "\n";
    }

    outputStream << outStringStream.str();

    outputStream.close();
    inputStream.close();
  }

  else if(globalParameters.getStreamMode() == StreamMode::StreamMode){
    int counter = 0;

    while(true){
      writeEntry(outStringStream);

      counter++;
      if(counter >= 10){
        counter = 0;
        outputStream << outStringStream.str();
        outStringStream.clear();
        outputStream.flush();
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
unsigned int BastaTransformer::encodeStream(const std::string& stream) const {
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto featureTypeMap = dynamic_cast<BastaFeatures*>(transformParameters)->getFeatureTypeMap();
  const auto featureIndexMap = dynamic_cast<BastaFeatures*>(transformParameters)->getFeatureindexMap();

  unsigned int res = 0;

  auto transformParametersCasted = dynamic_cast<BastaFeatures*>(transformParameters);
  if(transformParametersCasted == nullptr){
    throw new std::invalid_argument("encodeStream-method can only be used in conjunction with transformParameters of type BastaFeatures.");
  }
  else if(stream.size() == 0){
    throw new std::invalid_argument("Error: Tried to encode empty stream.");
  } 

  auto spaceSize = static_cast<unsigned int>(transformParametersCasted->getAllCategoricalDataSize()) * static_cast<unsigned int>(transformParametersCasted->getAllRangeValueSize());

  auto lineSplit = HelperFunctions::splitString(stream, globalParameters.getInputFileDelimiter());
  
  for(const auto& feature: featureTypeMap){
    const int index = featureIndexMap.at(feature.first);
    const auto& rawValue = lineSplit.at(index);

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
 * Keeps track of the alphabet size, i.e. updates the alphabet size if
 * we find a new element.
 * 
 * @param inputStream The input stream.
 * @return const std::string The string.
 */
void BastaTransformer::writeEntry(std::stringstream& stream){
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto featureInformation = dynamic_cast<BastaFeatures*>(transformParameters);
  const auto& outputFormat = globalParameters.getOutputfileFormat();

  const auto& featureIndexMap = featureInformation->getFeatureindexMap();
  const auto& sourceAddress = featureInformation->getSourceAddressPair();
  static bool filterBySourceAddress = featureInformation->filterBySourceAddress();

  if(outputFormat == OutputFileFormat::Abbadingo){
    std::vector<unsigned int> symbols;

    std::vector<std::string> lines = window->getWindow(inputStream);
    if(lines.empty()){
      return;
    }

    // convert the lines into a line in abbadingo
    for(auto& line: lines){
      if(line.empty()){
        continue;
      }

      const auto lineSplit = HelperFunctions::splitString(line, globalParameters.getInputFileDelimiter(), true);
      if(filterBySourceAddress && !(lineSplit.at(sourceAddress->second) == sourceAddress->first)){
        continue;
      }

      const auto code = encodeStream(line);
      alphabetSize = std::max(alphabetSize, code); // TODO: a good way to do?
      symbols.push_back(code);
    }

    stream << toAbbadingoFormat(symbols);
  }

  else if(outputFormat == OutputFileFormat::AugmentedAbbadingo){
    std::string line;
    getline(inputStream, line);
    if(line.empty()){
      return;
    }

    const auto lineSplit = HelperFunctions::splitString(line, globalParameters.getInputFileDelimiter(), true);
    if(filterBySourceAddress && !(lineSplit.at(sourceAddress->second) == sourceAddress->first)){
      return;
    }

    const auto code = encodeStream(line);
    stream << toAugmentedAbbadingoFormat(lineSplit[sourceAddress->second], lineSplit[featureInformation->getDstIndex()], 
                                         featureInformation->getAllFeatureNames(), code, lineSplit[featureInformation->getLabelIndex()]);
  }

  else{
    throw new std::invalid_argument("Output format not implemented in BastaTransformer.");
  }
};
