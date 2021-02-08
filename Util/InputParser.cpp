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

/**
 * @brief Parses the input file and writes them into the global parameters.
 * 
 * @param path 
 */
void HelperFunctions::parseInput(const std::string& path){
  std::ifstream file(path);
  std::string line;
  
  auto& globalParameters = GlobalParameters::getInstance();
  while(std::getline(file, line))
  {
    if(line.empty() || line[0] == '#'){
      continue;
    }

    const auto lineSplit = splitString(line);
    if(lineSplit.size() < 3){
      continue;
    }

    const auto& indicator = lineSplit[0];
    const auto& value = lineSplit[2];
    
    if(indicator == "alphabetSize"){
      globalParameters.setAlphabetSize(std::stoi(value));
    }
    else if(indicator == "windowSize"){
      globalParameters.setWindowSize(std::stoi(value));
    }
    else if(indicator == "windowStride"){
      globalParameters.setWindowStride(std::stoi(value));
    }
    else if(indicator == "streamMode"){
      globalParameters.setStreamMode(std::stoi(value));
    }
    else if(indicator == "srcAddress"){
      globalParameters.setSrcAddress(value);
    }
    else if(indicator == "inputFile"){
      //std::string filepath = std::filesystem::current_path();

      globalParameters.setInputFile(value);
    }
    else if(indicator == "outputFile"){
      globalParameters.setOutputFile(value);
    }
    else if(indicator == "inputFileFormat"){
      globalParameters.setInputfileFormat(std::stoi(value));
    }
    else if(indicator == "outputFileFormat"){
      globalParameters.setOutputfileFormat(std::stoi(value));
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
