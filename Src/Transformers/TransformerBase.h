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
#include "TransformerParameters.h"
#include "GlobalParameters.h"

#include <string>
#include <memory>
#include <unordered_map>

class TransformerBase{
protected:

  /**
   * @brief Convenience enum class to accelerate searching.
   * 
   */
  enum class FeatureType{
    categorical,
    rangeBased
  };

  /**
   * @brief The maximum chars a line can have. Can lead to 
   * errors when file has more characters per line than 
   * this number.
   * 
   */
  const size_t MAX_CHARS_PER_LINE = 512;

  std::ifstream inputStream;
  std::ofstream outputStream;

  WindowBase* window;
  TransformParameterBase* transformParameters;

  /**
   * @brief See getFeaturesFromHeader()-method.
   * 
   */
  std::unordered_map<std::string, int> featureIndexMap;

  /**
   * @brief The features that we want to observe with this transformer,
   * and their mapping to a feature type, to make look ups faster.
   * 
   */
  std::unordered_map<std::string, FeatureType> featureTypeMap;

  /**
   * @brief Get a line from the input file.
   * 
   * @return const std::string& The line as a string.
   */
  virtual std::string getConvertedLine() = 0;

  /**
   * @brief Function that is supposed to preprocess the input file to determine 
   * parameters such as ranges and the categorical-data mapping.
   * 
   * At this stage, the input and output stream are open already, hence one 
   * can start with manipulating the strings.
   * 
   */
  virtual void preprocess() = 0;

  void initFeatureIndexMapFromHeader(const std::string& sourceAddressHeader, const char delimiter = ' ');
  int encodeStream(const std::string& stream) const;

public: 
  TransformerBase();

  ~TransformerBase(){
    free(window);
    free(transformParameters);
  }
  
  virtual void convert() = 0;
};

#endif