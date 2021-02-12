/**
 * @file TransformerParameters.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief This file contains the parameter classes for the transformers. 
 * 
 * For each type of transformation there should be one class parameters, derived from the 
 * FeatureBase class.
 * 
 * @version 0.1
 * @date 2021-01-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _TRANSFORMER_PARAMETERS_
#define _TRANSFORMER_PARAMETERS_

#include <vector>
#include <string>
#include <map>
#include <unordered_map>

/**
 * @brief Class serves as base only.
 * 
 */
class FeatureBase{
public:
  enum class FeatureType;

protected:
  int alphabetSize;
  bool inputFileHasHeader;

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
   * @brief Add categorical feature to featureTypeMap.
   * 
   * @param feature Name of feature.
   */
  inline void addCategoricalFeature(const std::string& feature){
    if(featureTypeMap.count(feature) > 0){
      throw new std::invalid_argument("Error in addCategoricalFeature: Feature " + feature + " already exists. Please make sure to not give the same feature twice.");
    }
    this->featureTypeMap[feature] = FeatureType::categorical;
  }

  /**
   * @brief Add range-based feature to featureTypeMap.
   * 
   * @param feature Name of feature.
   */
  inline void addRangeBasedFeature(const std::string& feature){
    if(featureTypeMap.count(feature) > 0){
      throw new std::invalid_argument("Error in addRangeBasedFeature: Feature " + feature + " already exists. Please make sure to not give the same feature twice.");
    }
    this->featureTypeMap[feature] = FeatureType::rangeBased;
  }

public:
  /**
   * @brief Convenience enum class to accelerate searching.
   * 
   */
  enum class FeatureType{
    categorical,
    rangeBased
  };

  FeatureBase() = default;
  virtual void initFromFile(const std::string& filePath) = 0;

  inline auto getFeatureTypeMap() const noexcept {
    return this->featureTypeMap;
  }

  inline auto getFeatureindexMap() const noexcept {
    return this->featureIndexMap;
  }

  inline void setAlphabetSize(const int alphabetSize) noexcept {
    this->alphabetSize = alphabetSize; 
  }
  inline const int getAlphabetSize() const {
    return this->alphabetSize;
  }
  inline void setHasHeader(const bool inputFileHasHeader) noexcept {
    this->inputFileHasHeader = inputFileHasHeader;
  }
  inline const bool hasHeader() const {
    return this->inputFileHasHeader;
  }
};

#endif