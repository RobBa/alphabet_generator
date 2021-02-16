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
  outputStream.open(globalParameters.getOutputFile(), std::ios_base::out);
}

/**
 * @brief Convert a line to abbadingo format and returns that line.
 * 
 * @param symbols The input symbols to convert.
 * @return const std::string The converted line as a string.
 */
const std::string TransformerBase::toAbbadingoFormat(const std::vector<int>& symbols) const {
  std::stringstream res;
  res << symbols.size();

  for(const auto symbol: symbols){
    res << " " << symbol;
  }
  res << "\n";

  return res.str();  
}

/**
/**
 * @brief Convert a line to the augmented abbadingo format and returns that line.
 * 
 * @param symbols The input symbols to convert.
 * @return const std::string The converted line as a string.
 */
const std::string TransformerBase::toAugmentedAbbadingoFormat(const std::string& ip1, 
                                                              const std::string& ip2, 
                                                              const std::vector<std::string>& allFeatures,
                                                              const int symbol, 
                                                              const std::string& flowType) const {
  //std::stringstream res;
  //res << ip1 << " <-> " << ip2 << "\n";
  //
  //static bool hasInitialized = false;
  //static std::stringstream featureString;
  //if(!hasInitialized && !allFeatures.empty()){
  //  featureString << allFeatures.front();
  //  for(int i = 0; i < allFeatures.size(); ++i){
  //    featureString << ", " << allFeatures[i];
  //  }
  //  featureString << "/";
  //  hasInitialized = true;
  //}
  //
  //// TODO
  //res << 
}

