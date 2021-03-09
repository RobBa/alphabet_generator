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
  unlabeledNetflows[ipAddress].push_back(netflow);
}

/**
 * @brief Adds a piece of netflow into the stored netflows.
 * 
 * @param ipAddress The ip-address we want to add to.
 * @param netflow The netflow.
 */
void Host::addNetflow(const std::string& ipAddress, const std::string& netflow, const int label){
  if(labeledNetflows.count(ipAddress) == 0){
    labeledNetflows[ipAddress] = std::make_pair(label, std::vector<std::string>(0));
  }
  labeledNetflows[ipAddress].first = std::max(labeledNetflows[ipAddress].first, label);
  labeledNetflows[ipAddress].second.push_back(netflow);
}