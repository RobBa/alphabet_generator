/**
 * @file HelperFunctions.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Implements helper functions.
 * @version 0.1
 * @date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "HelperFunctions.h"

#include <sstream>
#include <algorithm>

/**
 * @brief Splits string and returns vector of string. 
 * 
 * @param input The input string.
 * @param delimiter The delimiter.
 * @return std::vector<std::string> Split (past tense) string returned as vector of substrings.
 */
std::vector<std::string> HelperFunctions::splitString(const std::string& input, const char delimiter){
  std::vector<std::string> res;
  std::stringstream sstream(input);
  std::string token;

  while(getline(sstream, token, delimiter)){
    if(!token.empty()){
      res.push_back(token);
    }
  }

  return res;
}

/**
 * @brief Splits string and returns vector of string.
 * Removes all the tabs in a preliminary step if removeTabs set to true.
 * 
 * @param input The input string.
 * @param removeTabs Remove the tabs in preliminary step if set to true.
 * @param delimiter The delimiter.
 * @return std::vector<std::string> Split (past tense) string returned as vector of substrings.
 */
std::vector<std::string> HelperFunctions::splitString(std::string& input, bool removeTabs, const char delimiter){
  if(removeTabs){
    input.erase(std::remove(input.begin(), input.end(), '\t'), input.end());
  }
  return splitString(input, delimiter);
}
