/**
 * @file StochasticWindow.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief A window of size 1 and stride 1. Used to collect batches.
 *  * 
 * @version 0.1
 * @date 2021-03-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "StochasticWindow.h"
#include "HelperFunctions.h"

#include <string>
#include <sstream>
#include <utility>

/**
 * @brief Get line, making sure it is complete.
 * 
 * @param inputStream The inputStream.
 * @return std::string The line as a string.
 */
std::string StochasticWindow::getStreamedLine(std::ifstream& inputStream) const {
  std::stringstream res;
  std::string line;
    
  while(true){
    HelperFunctions::resetStreamState(inputStream);
    getline(inputStream, line);
    if(line.empty()){
      continue;
    }
    res << std::move(line) << "\n";
    line.clear();
    if(!inputStream.eof()){
      return res.str();
    }
  }
}

/**
 * @brief Gets the line as a vector.
 * 
 * @param inputStream The stream.
 * @return std::vector<std::string> The line. 
 */
std::vector<std::string> StochasticWindow::getWindow(std::ifstream& inputStream) const {
  std::vector<std::string> res;
  auto line = getStreamedLine(inputStream); 

  res.push_back(std::move(line));
  return res;
}