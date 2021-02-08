/**
 * @file TransformerParameters.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief This file contains the parameter classes for the transformers. 
 * 
 * For each type of transformation there should be one class parameters, derived from the 
 * TransformParameterBase class.
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
class TransformParameterBase{
public:
  TransformParameterBase(){}; // not instantiable

protected:
  int alphabetSize;

public:
  virtual void setAlphabetSize(const int alphabetSize) noexcept {
    this->alphabetSize = alphabetSize; 
  }
};

/**
 * @brief The parameters of the transformation. We keep them separate from the
 * implementations so we can define them in a separate preprocessing step.
 * 
 */
class DiscreteTransformParameters : public TransformParameterBase{
  private:

  /**
   * @brief We map categorical data to ranges.
   * 
   * We chose an unordered map, because apart from iteration, it is faster than a map.
   */
  std::unordered_map<std::string, std::unordered_map<std::string, int> > categoricalData;

  /**
   * @brief The features (strings) mapped to their ranges.
   * 
   * The std::vector remains sorted in ascending order.
   * We chose an unordered map, because apart from iteration, it is faster than a map.
   * 
   */
  std::unordered_map<std::string, std::vector<double> > clusteringRanges;

  public:
    DiscreteTransformParameters(){};

    const int getCategoricalValue(const std::string& category, const std::string& name);
    const int getRangeValue(const std::string& category, const double value);

    const int getAllCategoricalDataSize() const;
    const int getAllRangeValueSize() const;

    const int getCategoricalDataSize(const std::string category) const;
    const int getRangeDataSize(const std::string category) const;

    void setCategoricalData(const std::string& category, const std::vector<std::string>& names);
    void setRanges(const std::string& category, std::vector<double>& ranges);
};

#endif