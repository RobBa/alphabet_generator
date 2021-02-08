/**
 * @file InputParser.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief The parser for the input file, as given via the command line.
 * @version 0.1
 * @date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _INPUT_PARSER_
#define _INPUT_PARSER_

#include <string>

namespace HelperFunctions{
  void parseInput(const std::string& path);
}

#endif