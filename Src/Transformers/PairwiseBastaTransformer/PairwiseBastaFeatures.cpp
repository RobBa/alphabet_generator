/**
 * @file PairwiseBastaFeatures.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Implementation of PairwiseBastaFeatures.h
 * @version 0.1
 * @date 2021-02-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "PairwiseBastaFeatures.h"
#include "GlobalParameters.h"
#include "HelperFunctions.h"

void PairwiseBastaFeatures::preprocessInput(std::ifstream& inputStream){
  const auto& globalParameters = GlobalParameters::getInstance();
  std::string line;

  while(!inputStream.eof()){
    getline(inputStream, line);
    if(line.empty()){
      continue;
    }

    // inefficient, but compatible with later code.
    const auto lineSplit = HelperFunctions::splitString(line, globalParameters.getInputFileDelimiter(), true);
    const auto& sourceAddress = lineSplit[sourceAddressPair->second];
    hosts[sourceAddress].addNetflow(lineSplit[dstAddressIndex], line);
  }
}