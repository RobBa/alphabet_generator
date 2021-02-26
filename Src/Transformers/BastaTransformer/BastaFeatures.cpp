/**
 * @file BastaFeatures.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Transforminformation of the BastaTransformer.
 * @version 0.1
 * @date 2021-02-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include "BastaFeatures.h"
#include "HelperFunctions.h"

#include <algorithm>
#include <utility>
#include <type_traits>
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>


/**
 * @brief Parses the .ini file of the features.
 * 
 * @param filePath Path to the .ini-file of the features.
 */
void BastaFeatures::initFromFile(const std::string& filePath){
  // From https://stackoverflow.com/a/27588225
  std::ifstream test(filePath); 
  if (!test){
    throw new std::invalid_argument("Error: Please give a valid feature-ini file");
  }

  auto stream = std::ifstream(filePath);
  std::string line;    

  while(std::getline(stream, line))
  {
    if(line.empty() || line[0] == '#'){
      continue;
    }

    auto lineSplit = HelperFunctions::splitString(line, ' ');
    if(lineSplit.size() < 3){
      continue;
    }

    const auto& feature = lineSplit[0];
    const auto& category = lineSplit[1];
    const auto& value = lineSplit.back();

    if(category == "categorical"){
      this->allFeatures.push_back(feature);
      this->addCategoricalFeature(feature);

      std::vector<std::string> categories;
      for(int i = 2; i < lineSplit.size() - 2; ++i){
        categories.push_back(lineSplit[i]);
      }
      this->setCategoricalData(feature, categories);
    }

    else if(category == "rangeBased"){
      this->allFeatures.push_back(feature);
      this->addRangeBasedFeature(feature);

      std::vector<double> ranges;
      for(int i = 2; i < lineSplit.size() - 2; ++i){
        ranges.push_back(std::stod(lineSplit[i]));
      }
      this->setRanges(feature, ranges);
    }
    else if(feature == "Label"){
      const auto& labels = HelperFunctions::splitString(std::string(category), ',', true);
      int i = 0;
      for(const auto& label: labels){
        this->labels[label] = i;
        ++i;
      }
      this->labelIndex = std::stoi(value);
    }

    else if(feature == "SourceAddress"){
      this->filterSourceAddress = true;
      if(this->sourceAddressPair == nullptr){
        this->sourceAddressPair = std::make_unique< std::pair<std::string, int> >(std::make_pair(value, -1));
        continue;
      }
      this->sourceAddressPair->first = value;
    }

    else if(feature == "SrcIndex"){
      const auto index = std::stoi(value);
      if(this->sourceAddressPair == nullptr){
        this->sourceAddressPair = std::make_unique< std::pair<std::string, int> >(std::make_pair("", index));
        continue;
      }
      this->sourceAddressPair->second = index;
    }

    else if(feature == "DstIndex"){
      this->dstAddressIndex = std::stoi(value);
    }

    else if(feature == "HasHeader"){
      if(value == "true"){
        this->setHasHeader(true);
      }
      else if(value == "false"){
        this->setHasHeader(false);
      }
      else{
        throw new std::invalid_argument("Argument " + value + "for HasHeader in the feature's .ini is invalid. Please give either 'true' or 'false'");
      }
      continue;
    }
    else if(feature == "outputFileFormat"){
      const auto intValue = std::stoi(value);
      if(intValue == 0){
        this->ofFormat == BastaOutputFormat::Abbadingo;
      }
      else if(intValue == 1){
        this->ofFormat == BastaOutputFormat::AugmentedAbbadingo;
      }
      else{
        throw new std::invalid_argument("Argument " + value + "for outputFileFormat in the feature's .ini is invalid.");
      }
    }
    else{
        throw new std::invalid_argument("Feature " + feature + "not registered in BastaFeatures-Parser.");
    }

    this->featureIndexMap[feature] = std::stoi(value);
  }
}


/**
 * @brief Returns the assigned integer value of the category and name. 
 * 
 * @param category The category to look in, e.g. "protocol". Needs to be set.
 * @param name The name of the value, e.g. "TCP" in protocol.
 * @return int The assigned integer value.
 */
const int BastaFeatures::getCategoricalValue(const std::string& category, const std::string& name){
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
const int BastaFeatures::getRangeValue(const std::string& category, const double value){
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
const int BastaFeatures::getAllCategoricalDataSize() const {
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
const int BastaFeatures::getAllRangeValueSize() const {
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
const int BastaFeatures::getCategoricalDataSize(const std::string category) const {
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
const int BastaFeatures::getRangeDataSize(const std::string category) const {
  return clusteringRanges.at(category).size();
}


/**
 * @brief Add an entry to categorical data. This function helps to determine categorical data mapping
 * at runtime.
 * 
 * An example would be "protocol" for category, and a name "udp". If "protocol" already exists checks out
 * if "upd" exists within "protocol" category, and if not, then assign unique (ascending) integer to "udp". 
 * "protocol" would be pointing towards a map mapping the names onto integers, which can then 
 * be retrieved via getNumericalCategory().
 * 
 * @param category Name of the category.
 * @param name Name of the entry of the category.
 */
void BastaFeatures::addCategoricalEntry(const std::string& category, const std::string& name){
  auto& categoryMap = categoricalData.at(category);
  if(categoryMap.count(name) == 0){
    categoryMap[name] = categoryMap.size() + 1;
  }
}


/**
 * @brief Checks if entry already exists.
 * 
 * @param category Name of the category.
 * @param name Name of the entry to be checked.
 */
bool BastaFeatures::hasCategoricalEntry(const std::string& category, const std::string& name) const {
  const auto& categoryMap = categoricalData.at(category);
  if(categoryMap.count(name) == 0){
    return false;
  }

  return true;
}


/**
 * @brief Add entire entry list to categorical data. If category already exists, then it will be overwritten.
 * 
 * An example would be "protocol" for category, and a vector containing {"TCP", "UDP"}. 
 * "protocol" would be pointing towards a map mapping the names onto unique integers, which can then 
 * be retrieved via getNumericalCategory().
 * 
 * @param names A string of all the names in the category.
 */
  void BastaFeatures::setCategoricalData(const std::string& category, const std::vector<std::string>& names){

  // first make sure that key is clean to have consistent behavior in the methods
  categoricalData.erase(category);

  try{
    auto& field = categoricalData[category];
    int i = 1;
    for(const auto& name: names){
      field[name] = i;
      i++;
    }
  }
  catch(const std::runtime_error e)
  {
    std::cerr << e.what() << std::endl;
  }

  assert(categoricalData.at(category).size() == names.size());
}

/**
 * @brief Sets the clustering ranges at name and overwrites them if they already exist.
 * 
 * Sorts the ranges before setting.
 * 
 * @param name The name of the range-parameter.
 * @param ranges Ranges in sorted or unsorted order.
 */
void BastaFeatures::setRanges(const std::string& category, std::vector<double>& ranges){

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

  assert(clusteringRanges.at(category).size() == ranges.size());
}