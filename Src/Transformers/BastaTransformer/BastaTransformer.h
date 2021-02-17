/**
 * @file BastaTransformer.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief This transformer transforms a stream of data of specific format (e.g. netflows) 
 * into discrete integer-value sequences in abbadingo format. 
 * 
 * The sequences can for example be used as an input alphabet for state machines. 
 * 
 * @version 0.1
 * @date 2021-01-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _DISCRETE_TRANSFORMER_
#define _DISCRETE_TRANSFORMER_

#include "TransformerBase.h"
#include "FixedSizeWindow.h"

class BastaTransformer : public TransformerBase{
private:
  /**
   * @brief Convenience field.
   * 
   */
  unsigned int alphabetSize = 0;

  /**
   * @brief Convenience, to save us paths in the main-ini file.
   * 
   */
  const std::string featureIniDir = "../Src/Transformers/BastaTransformer/FeatureParameters/";

  void writeEntry(std::stringstream& stream) override;
  unsigned int encodeStream(const std::string& stream) const;

public:   
  BastaTransformer();
  virtual void convert() override;
};

#endif