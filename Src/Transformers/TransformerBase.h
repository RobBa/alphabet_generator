/**
 * @file TransformerBase.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief The base class for all transformers.
 * @version 0.1
 * @date 2021-01-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _TRANSFORMER_BASE_
#define _TRANSFORMER_BASE_

#include "WindowBase.h"
#include "FeatureBase.h"
#include "GlobalParameters.h"

#include <string>
#include <memory>
#include <unordered_map>

class TransformerBase{
protected:

  std::ifstream inputStream;
  std::ofstream outputStream;

  WindowBase* window;
  FeatureBase* transformParameters;

public: 
  TransformerBase();

  ~TransformerBase(){
    free(window);
    free(transformParameters);
  }
  
  virtual void convert() = 0;
};

#endif