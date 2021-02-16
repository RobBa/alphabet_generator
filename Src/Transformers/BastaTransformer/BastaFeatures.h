/**
 * @file BastaFeatures.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Transforminformation of the BastaTransformer.
 * @version 0.1
 * @date 2021-02-12
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _BASTA_FEATURES_
#define _BASTA_FEATURES_

#include "FeatureBase.h"


class BastaFeatures : public FeatureBase{
private:

  /**
   * @brief Convenience field, to filter out desired 
   * netflows in network data.
   * 
   */
  const std::string sourceAddressField = "Src";

  /**
   * @brief In case we want to parse a netflow file for a source address.
   * 
   */
  std::string sourceAddress;

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

  void setCategoricalData(const std::string& category, const std::vector<std::string>& names);
  void setRanges(const std::string& category, std::vector<double>& ranges);

public:
  BastaFeatures() = default;
  BastaFeatures(const std::string& filePath){
    this->initFromFile(filePath);
  }

  void initFromFile(const std::string& filePath) override;

  inline const auto& getSourceAddressField() const {
    return this->sourceAddressField;
  } 

  inline const auto& getSourceAddress() const {
    return this->sourceAddress;
  }

  const int getCategoricalValue(const std::string& category, const std::string& name);
  const int getRangeValue(const std::string& category, const double value);

  const int getAllCategoricalDataSize() const;
  const int getAllRangeValueSize() const;

  const int getCategoricalDataSize(const std::string category) const;
  const int getRangeDataSize(const std::string category) const;

  void addCategoricalEntry(const std::string& category, const std::string& name);
};

#endif