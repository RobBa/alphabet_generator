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
#include <cassert>


/**
 * @brief Extracts window out of input stream and returns it. TODO: not 100% functional. Debug
 * 
 * @param inputStream The input stream.
 * @return std::vector<std::string> The window.
 */
std::vector<std::string> FixedSizeWindow::getWindow(std::ifstream& inputStream) const {
  std::vector<std::string> res;
  const static auto& globalParameters = GlobalParameters::getInstance();

  assert(this->size >= this->stride);

  // helpers for stride; TODO: lineBuffer can get rid off, see issue on GitHub
  static std::vector<std::string> lineBuffer(this->size - this->stride);
  static bool initialized = false;

  // fill linebuffer once in first step
  if(!initialized){
    std::string line;
    for(int i = 0; i < this->size - this->stride; ++i){
      getline(inputStream, line);
      if(line.empty()){
        continue;
      }
      lineBuffer.push_back(std::move(line));
    }
  initialized = true;
  }

  for(auto& line: lineBuffer){
    res.push_back(std::move(line));
  }
  lineBuffer.clear();

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){
    for(int i = res.size(); i <= this->size; ++i){ // <= so we match size exactly
      std::string line;
      if(!getline(inputStream, line)){
        return res;
      }
      lineBuffer.push_back(line); // copies element, so we're fine with the move below
      res.push_back(std::move(line));
    }
  }

  else if(globalParameters.getStreamMode() == StreamMode::StreamMode){
    // sometimes we only get half a line. Keep on pulling from stream until we have the whole string.
    std::string previousLine;

    int i = res.size();
    while(i <= this->size){ // <= so we match size exactly
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
        lineBuffer.push_back(line);
        res.push_back(std::move(line));
      }
      else{
        lineBuffer.push_back(previousLine);
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

/**
 * @brief Extracts window out of input stream, represented by vector of strings, and returns it.
 * 
 * @param inputStream The input stream.
 * @return std::vector<std::string> The window.
 */
std::vector<std::string> FixedSizeWindow::getWindow(std::vector<std::string>& inputStream) {
  std::vector<std::string> res;
  const static auto& globalParameters = GlobalParameters::getInstance();

  assert(this->size >= this->stride);

  static std::vector<std::string> lineBuffer;

  // fill linebuffer once in first step
  if(!this->initialized){
    lineBuffer.clear();
    std::string line;
    while(lineBuffer.size() < this->size - this->stride){
      if(inputStream.empty()){
        res = lineBuffer;
        assert(res.size() <= this->size);
        return res;
      }

      line = std::move(inputStream.front());
      inputStream.erase(inputStream.begin());
      if(line.empty()){
        continue;
      }
      lineBuffer.push_back(std::move(line));
    }
  this->initialized = true;
  }

  for(auto& line: lineBuffer){
    res.push_back(std::move(line));
  }
  lineBuffer.clear();

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){
    for(int i = res.size(); i < this->size; ++i){
      if(inputStream.empty()){
        return res;
      }

      lineBuffer.push_back(inputStream.front()); // copies element, so we're fine with the move below
      res.push_back(std::move(inputStream.front()));
      inputStream.erase(inputStream.begin());
    }
  }

  else if(globalParameters.getStreamMode() == StreamMode::StreamMode){
    throw new std::invalid_argument("Stream mode not implemented in this overload of FixedSizeWindow.");
  }
  else{
    throw new std::invalid_argument("Streaming mode not implemented in FixedSizeWindow.");
  }
  assert(res.size() <= this->size);
  return res;
}
