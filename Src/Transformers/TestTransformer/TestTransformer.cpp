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

#include "TestTransformer.h"
#include "TransformerParameters.h"
#include "HelperFunctions.h"

#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <iostream>

/**
 * @brief Preprocessing step. This function gets automatically called when calling TransformerBase's 
 * constructor. It should always be overwritten and serve as an init()-step.
 * 
 */
void TestTransformer::preprocess(){
  transformParameters = new DiscreteTransformParameters;

  // step 1: define the features we want and map them onto their respective class
  const auto protocolFeature = std::string("Proto");
  const auto totPaketsFeature = std::string("Dur");

  featureTypeMap[protocolFeature] = FeatureType::categorical;
  featureTypeMap[totPaketsFeature] = FeatureType::rangeBased;
  
  // step 2: initialize the featureIndexMap from the header
  initFeatureIndexMapFromHeader(sourceAddressHeader, '\t');

  // we need to redjust, since some data is corrupted
  featureIndexMap["Proto"] += 1;
  featureIndexMap["Dur"] += 1;
  featureIndexMap[sourceAddressHeader] += 1;

  // step 3: somehow define the categories and the ranges
  std::vector<std::string> protocols{"llc", "arp", "ipv6-icmp", "tcp", "udp"};
  dynamic_cast<DiscreteTransformParameters*>(transformParameters)->setCategoricalData(protocolFeature, protocols);

  std::vector<double> totalByteRanges{80., 120., 180.}; // we treat them here as upper bounds on the range, the last is >180.
  dynamic_cast<DiscreteTransformParameters*>(transformParameters)->setRanges(totPaketsFeature, totalByteRanges);
}

/**
 * @brief Main function where conversion takes place.
 * 
 */
void TestTransformer::convert(){
  // TODO: check the mode (batch, stream) here, and keep on rolling the ball until finished
  const auto& globalParameters = GlobalParameters::getInstance();
  std::stringstream outStringStream;
  int linecount = 0;

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){

    while(true){
      auto line = getConvertedLine();
      if(line.empty()){
        break; // TODO: you can do better than that
      }
      outStringStream << line;
      linecount++;
    }

    outputStream << linecount << " " << alphabetSize << "\n";
    outputStream << outStringStream.str();
  }

  else if(globalParameters.getStreamMode() == StreamMode::StreamMode){

    while(true){
      if(inputStream.eof()){
        try{
          inputStream.setstate(std::ios_base::goodbit);
          inputStream.clear();
        }
        catch(std::ifstream::failure e) {
          std::cerr << "Exception opening/reading/closing file\n";
        }
      }
      else{
        auto line = getConvertedLine();
        if(line.empty()){
          continue;
        }

        outputStream << line;
      }
    }
  }
  else{
    throw new std::invalid_argument("Requested streaming mode not implemented");
  }
}

/**
 * @brief Gets a line in the new format. Side effect: 
 * Keeps track of the alphabet size, i.e. updates the alphabet size if
 * we find a new element.
 * 
 * @param inputStream The input stream.
 * @return const std::string The string.
 */
std::string TestTransformer::getConvertedLine(){
  const auto& globalParameters = GlobalParameters::getInstance();
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

    const auto& lineSplit = HelperFunctions::splitString(line, true, ' ');
    
    if(lineSplit.at(featureIndexMap[sourceAddressHeader]) == globalParameters.getSrcAddress()){ // TODO: split twice now
      const int code = encodeStream(line);
      alphabetSize = std::max(alphabetSize, code); // TODO: a good way to do?
      symbols << code << " ";
      symbolCount++;
    }
  }

  sstream << symbolCount << " " << symbols.str() << "\n";

  return sstream.str();
};