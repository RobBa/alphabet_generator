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

  /**
   * @brief Fetches the symbols out of the window frame.
   * 
   * We chose this abstract representation to have an easy conversion from 
   * symbols to any desired output format.
   * 
   * @return std::vector<int> Vector with the symbols.
   */
  virtual const std::vector<int> getSymbols() = 0;

  const std::string toAbbadingoFormat(const std::vector<int>& symbols) const;
  
  const std::string toAugmentedAbbadingoFormat(const std::string& ip1, 
                                               const std::string& ip2, 
                                               const std::vector<std::string>& allFeatures,
                                               const int symbol, 
                                               const std::string& flowType) const;

public: 
  TransformerBase();

  virtual ~TransformerBase(){

    free(window);
    free(transformParameters);

    inputStream.close();
    outputStream.close();
  }
  
  /**
   * @brief Main loop that converts the file.
   * 
   */
  virtual void convert() = 0;
};

#endif