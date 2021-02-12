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
  std::stringstream outStringStream;
  int linecount = 0;

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){

    while(true){
      const auto line = getConvertedLine();
      if(line.empty()){ // finished reading file
        break;
      }
      outStringStream << line;
      linecount++;
    }

    outputStream << linecount << " " << alphabetSize << "\n";
    outputStream << outStringStream.str();
  }

  else if(globalParameters.getStreamMode() == StreamMode::StreamMode){
    int counter = 0;

    while(true){
      const auto line = getConvertedLine();
      if(line.empty()){
        continue;
      }

      outputStream << line;
        
      counter++;
      if(counter >= 10){
        counter = 0;
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
int BastaTransformer::encodeStream(const std::string& stream) const{
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto featureTypeMap = dynamic_cast<BastaFeatures*>(transformParameters)->getFeatureTypeMap();
  const auto featureIndexMap = dynamic_cast<BastaFeatures*>(transformParameters)->getFeatureindexMap();

  int res = 0;

  auto transformParametersCasted = dynamic_cast<BastaFeatures*>(transformParameters);
  if(transformParametersCasted == nullptr){
    throw new std::invalid_argument("encodeStream-method can only be used in conjunction with transformParameters of type BastaFeatures.");
  }
  else if(stream.size() == 0){
    throw new std::invalid_argument("Error: Tried to encode empty stream.");
  } 

  int spaceSize = transformParametersCasted->getAllCategoricalDataSize() * transformParametersCasted->getAllRangeValueSize();

  auto lineSplit = HelperFunctions::splitString(stream, globalParameters.getInputFileDelimiter());
  
  for(const auto& feature: featureTypeMap){
    const int index = featureIndexMap.at(feature.first);
    const auto& rawValue = lineSplit.at(index);

    if(feature.second == FeatureBase::FeatureType::categorical){
      res += transformParametersCasted->getCategoricalValue(feature.first, rawValue) * spaceSize / transformParametersCasted->getCategoricalDataSize(feature.first);
      spaceSize /= transformParametersCasted->getCategoricalDataSize(feature.first);
    }
    else if(feature.second == FeatureBase::FeatureType::rangeBased){
      const double value = std::stod(rawValue);
      res += transformParametersCasted->getRangeValue(feature.first, value) * spaceSize / transformParametersCasted->getRangeDataSize(feature.first);
      spaceSize /= transformParametersCasted->getRangeDataSize(feature.first);
    }
  }

  return res;
}

/**
 * @brief Gets a line in the new format. Side effect: 
 * Keeps track of the alphabet size, i.e. updates the alphabet size if
 * we find a new element.
 * 
 * @param inputStream The input stream.
 * @return const std::string The string.
 */
const std::string BastaTransformer::getConvertedLine(){
  const auto& globalParameters = GlobalParameters::getInstance();

  auto featureIndexMap = dynamic_cast<BastaFeatures*>(transformParameters)->getFeatureindexMap();
  const auto& sourceAddressField = dynamic_cast<BastaFeatures*>(transformParameters)->getSourceAddressField();
  const auto& sourceAddress = dynamic_cast<BastaFeatures*>(transformParameters)->getSourceAddress();

  std::stringstream sstream; 
  int symbolCount = 0;

  std::vector<std::string> lines = window->getWindow(inputStream);
  if(lines.empty()){
    return sstream.str();
  }

  std::stringstream symbols;

  // convert the lines into a line in abbadingo
  for(auto& line: lines){
    if(line.empty()){
      continue;
    }

    const auto& lineSplit = HelperFunctions::splitString(line, globalParameters.getInputFileDelimiter(), true);
    
    if(lineSplit.at(featureIndexMap[sourceAddressField]) == sourceAddress){ // TODO: split twice now
      const int code = encodeStream(line);
      alphabetSize = std::max(alphabetSize, code); // TODO: a good way to do?
      symbols << code << " ";
      symbolCount++;
    }
  }

  sstream << symbolCount << " " << symbols.str() << "\n";

  return sstream.str();
};

/**
 * @brief Helper method for the preprocessing step. Gets the features 
 * from the header of the input stream and maps them onto the position
 * of each input line and writes them into the featureMap. Prerequisites:
 * The featureTypeMap should have been filled by now.
 * 
 * Function can be overwritten by child classes if needed.
 * String manipulation taken from https://stackoverflow.com/a/5888676
 * 
 * @param delimiter The delimiter character.
 */
//void TransformerBase::initFeatureIndexMapFromHeader(const std::string& sourceAddressHeader, const char delimiter){
//  if(featureTypeMap.size() == 0){
//    throw new std::logic_error("getFeaturesFromHeader-method must only be called after featureTypeMap has been defined properly.");
//  }
//
//  std::string header;
//  std::getline(inputStream, header);
//  const auto fields = HelperFunctions::splitString(header, delimiter);
//  
//  int index = 0;
//  for(const auto& field: fields){
//    if(featureTypeMap.count(field) > 0 || sourceAddressHeader == field){
//      featureIndexMap[field] = index;
//    }
//    index++;
//  }
//}