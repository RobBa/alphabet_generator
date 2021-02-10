/**
 * @file WindowBase.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Abstract base class for all the windows.
 * @version 0.1
 * @date 2021-02-10
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "WindowBase.h"

#include <iostream>

/**
 * @brief Checks the input stream on end-of-file and resets state if happened.
 * 
 * @param inputStream The input stream.
 */
void WindowBase::resetStreamState(std::ifstream& inputStream) const{
  if(inputStream.eof()){
    try{
      inputStream.setstate(std::ios_base::goodbit);
      inputStream.clear();
    }
    catch(std::ifstream::failure e) {
      std::cerr << "Exception opening/reading/closing file\n";
    }
  }
}
