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

#include "StreamingBastaFeatures.h"
#include "Host.h"

#include <fstream>

class PairwiseBastaFeatures : public StreamingBastaFeatures {
private:
  /* Map source ip address to a host */
  std::unordered_map<std::string, Host> hosts;

  bool filterByDst;

public:
  PairwiseBastaFeatures() : StreamingBastaFeatures() {};
  PairwiseBastaFeatures(const std::string& filePath) : StreamingBastaFeatures(filePath) {};

  void initFromFile(const std::string& filePath) override;

  void preprocessInput(std::ifstream& inputStream);

  inline const auto& getHosts() const {
    return this->hosts;
  }

  inline const int getLabel(const std::string& label) {
    return this->labels.at(label);
  }

  inline const auto filterByDestination() const {
    return this->filterByDst;
  }
};

#endif