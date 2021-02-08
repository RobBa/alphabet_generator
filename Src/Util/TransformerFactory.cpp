/**
 * @file TransformerFactory.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Factory for transformer objects.
 * @version 0.1
 * @date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "TransformerFactory.h"

TransformerBase* TransformerFactory::createTransformer(){
  const auto& globalParameters = GlobalParameters::getInstance();

  if(globalParameters.getTransformerType() == TransformerType::TestTransformer){
    return new TestTransformer;
  }
  else{
    throw new std::invalid_argument("Transformer-type argument not implemented in TransformerFactory");
  }
}