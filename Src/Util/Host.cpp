/**
 * @file Host.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Implementation of Host.h
 * @version 0.1
 * @date 2021-02-22
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Host.h"

/**
 * @brief Adds a piece of netflow into the stored netflows.
 * 
 * @param ipAddress The ip-address we want to add to.
 * @param netflow The netflow.
 */
void Host::addNetflow(const std::string& ipAddress, const std::string& netflow){
  netflows[ipAddress].push_back(netflow);
}

/**
 * @brief Adds a piece of netflow into the stored netflows.
 * 
 * @param ipAddress The ip-address we want to add to.
 * @param netflow The netflow.
 */
void Host::addNetflow(const std::string& ipAddress, const std::string& netflow, const int label){
  netflows[ipAddress].push_back(netflow);
  if(label != 0 && this->label == 0){
    this->label = label;
  }
}