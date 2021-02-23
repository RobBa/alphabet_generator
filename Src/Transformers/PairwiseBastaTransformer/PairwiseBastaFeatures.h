/**
 * @file PairwiseBastaFeatures.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Transforminformation of the PairwiseBastaTransformer.
 * @version 0.1
 * @date 2021-02-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _PAIRWISE_BASTA_FEATURES_
#define _PAIRWISE_BASTA_FEATURES_

#include "BastaFeatures.h"
#include "Host.h"

#include <fstream>

class PairwiseBastaFeatures : public BastaFeatures {
private:
  /* Map source ip address to a host */
  std::unordered_map<std::string, Host> hosts;

public:
  PairwiseBastaFeatures() : BastaFeatures() {};
  PairwiseBastaFeatures(const std::string& filePath) : BastaFeatures(filePath) {};

  void preprocessInput(std::ifstream& inputStream);

  inline const auto& getHosts() const {
    return this->hosts;
  }
};

#endif