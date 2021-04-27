/**
 * @file PairwiseBastaFeatures.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Implementation of PairwiseBastaFeatures.h
 * @version 0.1
 * @date 2021-02-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "PairwiseBastaFeatures.h"
#include "GlobalParameters.h"
#include "HelperFunctions.h"

/**
 * @brief Parses the .ini file of the features.
 * 
 * @param filePath Path to the .ini-file of the features.
 */
void PairwiseBastaFeatures::initFromFile(const std::string& filePath){
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
      int i = 1;
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

    else if(feature == "BatchSize"){
      this->batchSize = std::stoi(value);
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
    else if(feature == "FilterByDestination"){
      if(value == "true"){
        this->filterByDst = true;
      }
      else if(value == "false"){
        this->filterByDst = false;
      }
      else{
        throw new std::invalid_argument("Argument " + value + "for FilterByDestination in the feature's .ini is invalid. Please give either 'true' or 'false'");
      }
      continue;
    }
    else{
        throw new std::invalid_argument("Feature " + feature + "not registered in StreamingBastaFeatures-Parser.");
    }

    this->featureIndexMap[feature] = std::stoi(value);
  }
}


/**
 * @brief Prepocesses the input.
 * 
 * @param inputStream 
 */
void PairwiseBastaFeatures::preprocessInput(std::ifstream& inputStream){
  const auto& globalParameters = GlobalParameters::getInstance();

  std::string line;

  while(!inputStream.eof()){
    getline(inputStream, line);
    if(line.empty()){
      continue;
    }

    // inefficient, but compatible with later code.
    const auto lineSplit = HelperFunctions::splitString(line, globalParameters.getInputFileDelimiter(), true);
    const auto& sourceAddress = lineSplit[sourceAddressPair->second];

    //const auto& rawLabel = lineSplit[labelIndex];
    //if(labels.count(rawLabel) == 0){
    //  labels[rawLabel] = labels.size();
    //}

    //if(!labels.empty()){
      hosts[sourceAddress].addNetflow(lineSplit[dstAddressIndex], line, 0);
    // }
    //else{
      // TODO: what to do when we do not want labeled output here?
    //  hosts[sourceAddress].addNetflow(lineSplit[dstAddressIndex], line);
    //}
  }
}