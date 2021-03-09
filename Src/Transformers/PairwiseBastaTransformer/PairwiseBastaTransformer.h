/**
 * @file PairwiseBastaTransformer.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief This transformer transforms a stream of data of specific format (e.g. netflows) 
 * into discrete integer-value sequences in abbadingo format. Unlike the normal 
 * StreamingBastaTransformer it builds pairs of source and IP addresses, and it outputs the entire
 * file.
 * 
 * The sequences can for example be used as an input alphabet for state machines. 
 * 
 * @version 0.1
 * @date 2021-02-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _PAIRWISE_BASTA_TRANSFORMER_
#define _PAIRWISE_BASTA_TRANSFORMER_

#include "StreamingBastaTransformer.h"
#include "FixedSizeWindow.h"
#include "Host.h"

#include <utility>

class PairwiseBastaTransformer : public StreamingBastaTransformer {
private:
  /**
   * @brief Convenience, to save us paths in the main-ini file.
   * 
   */
  const std::string featureIniDir = "../Src/Transformers/PairwiseBastaTransformer/FeatureParameters/";

  void writeEntry(std::stringstream& stream) override;
  
  void writeConnection(std::stringstream& stream, const std::pair< std::string, Host >& host) const;
  void writeConnection(std::stringstream& stream, const std::string& hostAddress, const Host& host) const;
  void writeConnection(std::stringstream& stream, const std::pair< std::string, Host >& host, const unsigned int hostNumber) const;

public:   
  PairwiseBastaTransformer();

  ~PairwiseBastaTransformer(){
      inputStream.close();
  }

  virtual void convert() override;
};

#endif