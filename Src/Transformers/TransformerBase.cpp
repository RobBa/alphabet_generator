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

#include <sstream>

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

  // From https://stackoverflow.com/a/27588225
  std::ifstream test(globalParameters.getInputFile()); 
  if (!test){
    throw new std::invalid_argument("Error: Please give a valid input file");
  }

  inputStream.open(globalParameters.getInputFile(), std::ios_base::in);
  std::string ofPath(globalParameters.getOutputFile());
  const auto rawOfPath = HelperFunctions::splitString(ofPath, '.')[0];
  outputStream.open(rawOfPath + ".txt", std::ios_base::out);
}

/**
 * @brief Contructs a comma separated string out of allFeatures.
 * 
 * @param allFeatures The features.
 * @return const std::string The resulting string.
 */
const std::string TransformerBase::getFeatureString(const std::vector<std::string>& allFeatures) const {
  std::stringstream featureString;

  if(!allFeatures.empty()){
    featureString << allFeatures.front();
    for(int i = 1; i < allFeatures.size(); ++i){
      featureString << "," << allFeatures[i];
    }
  }

  return featureString.str();
}