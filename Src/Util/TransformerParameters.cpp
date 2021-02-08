/**
 * @file TransformerParameters.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief The .cpp to the transformer_parameters.
 * @version 0.1
 * @date 2021-01-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "TransformerParameters.h"

#include <algorithm>
#include <utility>
#include <type_traits>
#include <stdexcept>
#include <iostream>
#include <cassert>


/**
 * @brief Returns the assigned integer value of the category and name. 
 * 
 * @param category The category to look in, e.g. "protocol". Needs to be set.
 * @param name The name of the value, e.g. "TCP" in protocol.
 * @return int The assigned integer value.
 */
const int DiscreteTransformParameters::getCategoricalValue(const std::string& category, const std::string& name){
  if(categoricalData.count(category) == 0){
    throw new std::invalid_argument("Category does not exist.");
  }
  
  return categoricalData[category][name];
}

/**
 * @brief Get the numerical cluster value within the ranges.
 * 
 * @param category The category to parse through (== the key in clusteringRanges).
 * @param value The value.
 * @return const int Within cluster value.
 */
const int DiscreteTransformParameters::getRangeValue(const std::string& category, const double value){
  int res = 0;

  if(clusteringRanges.count(category) == 0){
    throw new std::invalid_argument("Range category does not exist.");
  }

  const auto& rangeVec = clusteringRanges[category];
  for(auto bound: rangeVec){
    if(value <= bound){
      return res;
    }
    res++;
  }

  return res;
}

/**
 * @brief Gets the multiplied size of the categorical data. Because we expect this quantity 
 * to not grow after initialization, it will be computed only once, and then returned.
 * 
 * If interested, see "Learning Behavioral Fingerprints From Netflows Using
 * Timed Automata" (Pellegrino et al., 2017)
 * 
 * @return const int The size of the data.
 */
const int DiscreteTransformParameters::getAllCategoricalDataSize() const{
  static int res = 1;

  static bool isComputed = false;
  if(!isComputed){
    for(const auto& category: categoricalData){
      res *= category.second.size();
    }
    isComputed = true;
  }

  return res;
}

/**
 * @brief Gets the multiplied size of the categorical data. Because we expect this quantity to not 
 * grow after initialization, it will be computed only once, and then returned.
 * 
 * If interested, see "Learning Behavioral Fingerprints From Netflows Using
 * Timed Automata" (Pellegrino et al., 2017)
 * 
 * @return const int The size of the data.
 */
const int DiscreteTransformParameters::getAllRangeValueSize() const{
  static int res = 1;

  static bool isComputed = false;
  if(!isComputed){
    for(const auto& category: clusteringRanges){
      res *= category.second.size();
    }
    isComputed = true;
  }

  return res;
}

/**
 * @brief Gets the size of the categorical data bin specified by category. 
 * 
 * E.g. category=="protocal", and protocol exists, then method returns the 
 * number of protocols that we stored.
 * 
 * @param category The category to look out for.
 * @return const int The requested size.
 */
const int DiscreteTransformParameters::getCategoricalDataSize(const std::string category) const{
  return categoricalData.at(category).size();
}

/**
 * @brief Gets the size of the range data bin specified by category. 
 * 
 * E.g. category=="protocal", and protocol exists, then method returns the 
 * number of protocols that we stored.
 * 
 * @param category The category to look out for.
 * @return const int The requested size.
 */
const int DiscreteTransformParameters::getRangeDataSize(const std::string category) const{
  return clusteringRanges.at(category).size();
}

/**
 * @brief Add categorical data. If category already exists, then it will be overwritten.
 * 
 * An example would be "protocol" for category, and a vector containing {"TCP", "UDP"}. 
 * Protocol would be pointing towards a map mapping the names onto integers, which can then 
 * be retrieved via getNumericalCategory().
 * 
 * @param names A string of all the names in the category.
 */
  void DiscreteTransformParameters::setCategoricalData(const std::string& category, const std::vector<std::string>& names){

  // first make sure that key is clean to have consistent behavior in the methods
  categoricalData.erase(category);

  try{
    auto& field = categoricalData[category];
    int i = 0;
    for(const auto& name: names){
      field[name] = i;
      i++;
    }
  }
  catch(const std::runtime_error e)
  {
    std::cerr << e.what() << std::endl;
  }

  assert(categoricalData[category].size() == names.size());
}

/**
 * @brief Sets the clustering ranges at name and overwrites them if they already exist.
 * 
 * Sorts the ranges before setting.
 * 
 * @param name The name of the range-parameter.
 * @param ranges Ranges in sorted or unsorted order.
 */
void DiscreteTransformParameters::setRanges(const std::string& category, std::vector<double>& ranges){

  // first make sure that key is clean to have consistent behavior in the methods
  clusteringRanges.erase(category);

  try{
    std::sort(ranges.begin(), ranges.end());
    for(const double range: ranges){
      clusteringRanges[category].push_back(range);
    }
  }
  catch(const std::runtime_error e)
  {
    std::cerr << e.what() << std::endl;
  }

  assert(clusteringRanges[category].size() == ranges.size());
}