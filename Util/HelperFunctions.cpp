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
 * @brief Remove all occurences of the rmChar in input string.
 * 
 * @param string The string to be modified.
 * @param rmChar Char to be removed.
 */
void HelperFunctions::removeChars( /*out*/ std::string& input, const char rmChar){
  input.erase(std::remove(input.begin(), input.end(), rmChar), input.end());
}

/**
 * @brief Replace the target char replacement char in input string.
 * 
 * @param string The string to be modified.
 * @param targetChar Target char.
 * @param replacement Replacement char.
 */
void HelperFunctions::replaceCharacters( /*out*/ std::string& input, const char targetChar, const char replacement){
  replace(input.begin(), input.end(), targetChar, replacement);
}

/**
 * @brief Splits string and returns vector of string.
 * Removes all the tabs in a preliminary step if removeTabs set to true.
 * 
 * @param input The input string.
 * @param delimiter The delimiter.
 * @param replaceTabs Replace the tabs by delimiter in preliminary step if set to true.
 * @return std::vector<std::string> Split (past tense) string returned as vector of substrings.
 */
std::vector<std::string> HelperFunctions::splitString(std::string& input, const char delimiter, bool replaceTabs){
  if(replaceTabs){
    HelperFunctions::replaceCharacters(input, '\t', ' ');
  }

  return splitString(input, delimiter);
}
