/**
 * @file DiscreteTransformer.h
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

class TestTransformer : public TransformerBase{
protected:
  /**
   * @brief Convenience field.
   * 
   */
  int alphabetSize = 0;
  
  std::string sourceAddressHeader;

  virtual std::string getConvertedLine() override;
  virtual void preprocess() override;
public:   
  TestTransformer() : TransformerBase(){
    this->sourceAddressHeader = "SrcAddr";
    preprocess();
  };

  virtual void convert() override;
};

#endif