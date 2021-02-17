/**
 * @file FixedSizeWindow.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief A window of fixed size with respect to the number of data.
 * 
 * Has been applied e.g. by Skoulos et al., for more information read "Learning State Machines
 * faster using Locality-Sensitive Hashing and an application in network-based threat detection"
 * 
 * @version 0.1
 * @date 2021-01-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "FixedSizeWindow.h"
#include "GlobalParameters.h"
#include "HelperFunctions.h"

#include <utility>

std::vector<std::string> FixedSizeWindow::getWindow(std::ifstream& inputStream) const {
  std::vector<std::string> res;
  const static auto& globalParameters = GlobalParameters::getInstance();

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){
    for(int i = 0; i < this->size; ++i){
      std::string line;
      if(!getline(inputStream, line)){
        return res;
      }

      res.push_back(std::move(line));
    }
  }

  else if(globalParameters.getStreamMode() == StreamMode::StreamMode){
    // sometimes we only get half a line. Keep on pulling from stream until we have the whole string.
    std::string previousLine;

    int i = 0;
    while(i < this->size){
      std::string line;
      resetStreamState(inputStream);

      if(!getline(inputStream, line)){
        continue;
      }

      // covers the unfortunate case that last read line hit eof just before newline-character was written
      if(line.empty() || line.at(0) == globalParameters.getInputFileNewlineCharacter()){
        continue;
      }

      if(!previousLine.empty()){
        previousLine = previousLine + line;
      }

      if(inputStream.eof()){
        if(!previousLine.empty()){
          previousLine = previousLine + line;
        }
        else{
          previousLine = line;
        }
        continue;
      }

      if(previousLine.empty()){
        res.push_back(std::move(line));
      }
      else{
        res.push_back(previousLine); // no move to avoid undefined behavior, rare case in real world
      }
      previousLine.clear();
      ++i;
    }
  }
  else{
    throw new std::invalid_argument("Streaming mode not implemented in FixedSizeWindow.");
  }

  return res;
}