/**
 * @file HelperFunctions.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Helper functions globally used in this project.
 * @version 0.1
 * @date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _HELPER_FUNCTIONS_
#define _HELPER_FUNCTIONS_

#include <vector>
#include <string>

namespace HelperFunctions{
  void removeChars( /*out*/ std::string& input, const char rmChar);
  void replaceCharacters( /*out*/ std::string& input, const char targetChar, const char replacement);

  std::vector<std::string> splitString(const std::string& input, const char delimiter);
  std::vector<std::string> splitString(std::string& input, const char delimiter, bool replaceTabs);
  std::vector<std::string> splitString(std::string&& input, const char delimiter, bool replaceTabs);
}

#endif