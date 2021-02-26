/**
 * @file InputParser.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief The parser for the input file, as given via the command line.
 * @version 0.1
 * @date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "InputParser.h"
#include "GlobalParameters.h"
#include "HelperFunctions.h"

#include <fstream>
#include <algorithm>

/**
 * @brief Parses the input file and writes them into the global parameters.
 * 
 * @param path 
 */
void HelperFunctions::parseInput(const std::string& path){
  auto file = std::ifstream(path);
  std::string line;
  
  auto& globalParameters = GlobalParameters::getInstance();
  while(std::getline(file, line))
  {
    if(line.empty() || line[0] == '#'){
      continue;
    }

    const auto lineSplit = HelperFunctions::splitString(line, ' ');
    if(lineSplit.size() < 3){
      continue;
    }

    const auto& indicator = lineSplit[0];
    const auto& value = lineSplit[2];
    
    if(indicator == "windowSize"){
      globalParameters.setWindowSize(std::stoi(value));
    }
    else if(indicator == "windowStride"){
      globalParameters.setWindowStride(std::stoi(value));
    }
    else if(indicator == "streamMode"){
      globalParameters.setStreamMode(std::stoi(value));
    }
    else if(indicator == "inputFile"){
      globalParameters.setInputFile(value);
    }
    else if(indicator == "outputFile"){
      globalParameters.setOutputFile(value);
    }
    else if(indicator == "featureIni"){
      globalParameters.setFeatureIni(value);
    }
    else if(indicator == "delimiter"){
      auto delimiter = value;
      HelperFunctions::removeChars(delimiter, '\'');
      if(delimiter.size() != 1){
        throw new std::invalid_argument("Character " + delimiter + " invalid for delimiter in .ini");
      }
      globalParameters.setDelimiter(delimiter.front());
    }
    else if(indicator == "transformerType"){
      globalParameters.setTransformerType(std::stoi(value));
    }
    else if(indicator == "windowType"){
      globalParameters.setWindowType(std::stoi(value));
    }
    else{
      throw new std::invalid_argument("Init-option not defined in input parser.");
    }
  }
}
