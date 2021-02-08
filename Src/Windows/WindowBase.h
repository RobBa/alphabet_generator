/**
 * @file WindowBase.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Abstract base class for all the windows.
 * @version 0.1
 * @date 2021-01-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _WINDOWBASE_
#define _WINDOWBASE_

#include <vector>
#include <string>
#include <fstream>

class WindowBase{

protected:
  const int stride; // The stride at which the windows will be shifted.

public:
  WindowBase(const int stride) : stride(stride){};
  virtual std::vector<std::string> getWindow(std::ifstream& ifstream) = 0;
};

#endif