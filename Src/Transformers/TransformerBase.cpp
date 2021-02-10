/**
 * @file TransformerBase.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Implementation to the TransformerBase.h
 * @version 0.1
 * @date 2021-02-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "TransformerBase.h"
#include "WindowBase.h"
#include "FixedSizeWindow.h"
#include "HelperFunctions.h"

/**
 * @brief Construct a new Transformer Base:: Transformer Base object
 * 
 * Forces the preprocess()-method on its child classes, which will be necessary
 * to determine parameters, depending on what method we are aiming to use.
 * 
 */
TransformerBase::TransformerBase(){
  const auto& globalParameters = GlobalParameters::getInstance();

  switch(globalParameters.getWindowType()){
    case WindowType::FixedSizeWindow:
      window = new FixedSizeWindow(globalParameters.getWindowSize(), globalParameters.getWindowStride());
      break;
    default:
      throw new std::invalid_argument("Window type not implemented in TransformerBase.");
  }

  // Check for valid input file. From https://stackoverflow.com/a/27588225
  std::ifstream test(globalParameters.getInputFile()); 
  if (!test){
    throw new std::invalid_argument("Error: Please give a valid input file");
  }

  inputStream.open(globalParameters.getInputFile(), std::ios_base::in);
  outputStream.open(globalParameters.getOutputFile(), std::ios_base::out);
}

/**
 * @brief This method encodes streams, such as e.g. netflows. As a pre-
 * requisite, the internals should be set at this moment, e.g. the 
 * transformParameters and the featureMap.
 * 
 * Algorithm follows pattern as depicted in "Learning Behavioral Fingerprints 
 * From Netflows Using Timed Automata" (Pellegrino et al., 2017)
 * 
 * @param netflow The netflow as a vector of strings.
 * @return int The encoded netflow.
 */
int TransformerBase::encodeStream(const std::string& stream) const{
  const auto& globalParameters = GlobalParameters::getInstance();
  int res = 0;

  auto transformParametersCasted = dynamic_cast<DiscreteTransformParameters*>(transformParameters);
  if(transformParametersCasted == nullptr){
    throw new std::invalid_argument("encodeStream-method can only be used in conjunction with transformParameters of type DiscreteTransformParameters.");
  }
  else if(stream.size() == 0){
    throw new std::invalid_argument("Error: Tried to encode empty stream.");
  } 

  int spaceSize = transformParametersCasted->getAllCategoricalDataSize() * transformParametersCasted->getAllRangeValueSize();

  auto lineSplit = HelperFunctions::splitString(stream, globalParameters.getInputFileDelimiter());

  for(const auto& feature: featureTypeMap){
    const int index = featureIndexMap.at(feature.first);
    const auto& rawValue = lineSplit.at(index);

    if(feature.second == FeatureType::categorical){
      res += transformParametersCasted->getCategoricalValue(feature.first, rawValue) * spaceSize / transformParametersCasted->getCategoricalDataSize(feature.first);
      spaceSize /= transformParametersCasted->getCategoricalDataSize(feature.first);
    }
    else if(feature.second == FeatureType::rangeBased){
      const double value = std::stod(rawValue);
      res += transformParametersCasted->getRangeValue(feature.first, value) * spaceSize / transformParametersCasted->getRangeDataSize(feature.first);
      spaceSize /= transformParametersCasted->getRangeDataSize(feature.first);
    }
  }

  return res;
}

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
void TransformerBase::initFeatureIndexMapFromHeader(const std::string& sourceAddressHeader, const char delimiter){
  if(featureTypeMap.size() == 0){
    throw new std::logic_error("getFeaturesFromHeader-method must only be called after featureTypeMap has been defined properly.");
  }

  std::string header;
  std::getline(inputStream, header);
  const auto fields = HelperFunctions::splitString(header, delimiter);
  
  int index = 0;
  for(const auto& field: fields){
    if(featureTypeMap.count(field) > 0 || sourceAddressHeader == field){
      featureIndexMap[field] = index;
    }
    index++;
  }
}