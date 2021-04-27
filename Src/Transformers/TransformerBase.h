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

#include <sstream>

class TransformerBase{
protected:

  std::ifstream inputStream;

  WindowBase* window;
  FeatureBase* transformParameters;

  /**
   * @brief Writes and entry into the stream
   * 
   * We chose this abstract representation to have an easy conversion from 
   * symbols to any desired output format.
   * 
 * @param stream The stream to write to.
   */
  virtual void writeEntry(std::stringstream& stream) = 0;

  const std::string getFeatureString(const std::vector<std::string>& allFeatures) const;

public: 
  TransformerBase();

  virtual ~TransformerBase(){

    free(window);
    free(transformParameters);

    inputStream.close();
  }
  
  /**
   * @brief Main loop that converts the file.
   * 
   */
  virtual void convert() = 0;

protected:
  /**
   * @brief Convert a line to abbadingo format and returns that line.
   * 
   * @param symbols The input symbols to convert.
   * @return const std::string The converted line as a string.
   */
  template<typename T>
  const std::string toAbbadingoFormat(const std::vector<T>& symbols) const {
    std::stringstream res;
    res << symbols.size();

    for(const auto symbol: symbols){
      res << " " << symbol;
    }
    res << "\n";

    return res.str();  
  }

  /**
   * @brief Convert a line to abbadingo format and returns that line.
   * 
   * @param symbols The input symbols to convert.
   * @return const std::string The converted line as a string.
   */
  template<typename T>
  const std::string toAbbadingoFormat(const std::vector<T>& symbols, const int label) const {
    std::stringstream res;
    res << label << " " << symbols.size();

    for(const auto symbol: symbols){
      res << " " << symbol;
    }

    return res.str();  
  }

  /**
   * @brief Convert a line to abbadingo format and returns that line.
   * 
   * @param symbols The input symbols to convert.
   * @return const std::string The converted line as a string.
   */
  template<typename T>
  const std::string toAbbadingoFormat(const std::vector<T>& symbols, const std::string& label) const {
    std::stringstream res;
    res << label << symbols.size();

    for(const auto symbol: symbols){
      res << " " << symbol;
    }

    return res.str();  
  }

  /**
  /**
   * @brief Convert a line to the augmented abbadingo format and returns that line.
   * 
   * @param symbols The input symbols to convert.
   * @return const std::string The converted line as a string.
   */
  template<typename T>
  const std::string toAugmentedAbbadingoFormat(const std::string& ip1, 
                                               const std::string& ip2, 
                                               const std::vector<std::string>& allFeatures,
                                               const T symbol, 
                                               const std::string& flowType) const {
    std::stringstream res;
    static const auto featureString = getFeatureString(allFeatures);

    res << ip1 << " <-> " << ip2 << "\n";
    res << symbol << ":" << featureString << "/" << flowType << "\n";
    return res.str();
  }
};

#endif