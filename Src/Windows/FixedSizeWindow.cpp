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
#include <utility>

std::vector<std::string> FixedSizeWindow::getWindow(std::ifstream& inputStream){
  std::vector<std::string> res;

  for(int i = 0; i < this->size; i++){
    std::string line;
    if(!getline(inputStream, line)){
      return res;
    }

    res.push_back(std::move(line)); // TODO: does this what we want?
  }

  return res;
}