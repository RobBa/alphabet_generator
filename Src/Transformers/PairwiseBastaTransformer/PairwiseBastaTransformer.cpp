/**
 * @file PairwiseBastaTransformer.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Implementation of the PairwiseBastaTransformer
 * 
 * The sequences can for example be used as an input alphabet for state machines.
 * 
 * @version 0.1
 * @date 2021-02-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "PairwiseBastaTransformer.h"
#include "PairwiseBastaFeatures.h"
#include "HelperFunctions.h"

#include <set>
#include <cassert>

#include <iostream>

/**
 * @brief The constructor.
 * 
 */
PairwiseBastaTransformer::PairwiseBastaTransformer() : StreamingBastaTransformer(true) {
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto& featureIniFile = globalParameters.getFeatureIni();

  transformParameters = new PairwiseBastaFeatures;
  dynamic_cast<PairwiseBastaFeatures*>(transformParameters)->initFromFile(featureIniDir + featureIniFile);

  this->window = new StochasticWindow;

  if(transformParameters->hasHeader()){
    // skip one line, so we don't read in header accidentally
    std::string line;
    getline(inputStream, line);
  }

  dynamic_cast<PairwiseBastaFeatures*>(transformParameters)->preprocessInput(this->inputStream);
}

/**
 * @brief Main function where conversion takes place.
 * 
 */
void PairwiseBastaTransformer::convert(){
  // TODO: check the mode (batch, stream) here, and keep on rolling the ball until finished
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto featureParameters = dynamic_cast<PairwiseBastaFeatures*>(transformParameters);

  const auto& hosts = featureParameters->getHosts();
  const auto filterBySourceAddress = featureParameters->filterBySourceAddress();

  if(filterBySourceAddress){
    std::ofstream outstream(globalParameters.getOutputFile(), std::ios_base::out);

    const auto& hostAddress = featureParameters->getSourceAddressPair()->first;
    const auto& host = hosts.at(hostAddress);

    std::stringstream outStringStream;
    writeConnection(outStringStream, hostAddress, host);
    outstream << outStringStream.str();
    outstream.close();
  }
  else{
    unsigned int hostNumber = 0;
    const auto& outputFile = globalParameters.getOutputFile();
    const auto featureString = getFeatureString(featureParameters->getAllFeatureNames());

    for(const auto& host: hosts){
      std::ofstream outstream(outputFile + "_host_" + std::to_string(hostNumber) + ".txt");
      outstream << host.first << "\n";

      std::stringstream outStringStream;
      outstream << "Encoded features:" << featureString << "\n";

      writeConnection(outStringStream, host, hostNumber);
      outstream << outStringStream.str();
      outstream.close();

      ++hostNumber;
    }   
  }
}

/**
 * @brief We do not need this method in this class
 * 
 * @param stream 
 */
void PairwiseBastaTransformer::writeEntry(std::stringstream& stream){
  // empty
}

/**
 * @brief Converts entire connection and writes it into the stream.
 * 
 * @param stream Stream to write to.
 * @param host The host we will transform to a string.
 */
void PairwiseBastaTransformer::writeConnection(std::stringstream& stream, const std::pair< std::string, Host >& host) const {
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto featureInformation = dynamic_cast<PairwiseBastaFeatures*>(transformParameters);
  static auto featureString = getFeatureString(featureInformation->getAllFeatureNames());

  const auto& ipAddress = host.first;

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){
    for(auto& dst: host.second.getLabeledNetflows()){
      //std::stringstream dstString;
      std::stringstream symbolString;

      stream << ipAddress << " <-> " << dst.first << "\n";
      stream << "Encoded features: " << featureString << "\n";

      const auto& label = host.second.getLabel(dst.first);

      unsigned int lineCount = 0;
      std::set<unsigned int> symbolSet; // keep track of size of alphabet for this host
      std::vector<unsigned int> symbols;

      dynamic_cast<FixedSizeWindow*>(window)->setIsInitialized(false);
      auto flows = dst.second.second;

      while(!flows.empty()){
        auto lines = dynamic_cast<FixedSizeWindow*>(window)->getWindow(flows); // TODO: we could spare the copy here
        for(auto& line: lines){
          if(line.empty()){
            continue;
          }

          const auto code = encodeStream(line);
          symbols.push_back(code);
          symbolSet.insert(code);
        }

        assert(symbols.size() <= globalParameters.getWindowSize());

        symbolString << toAbbadingoFormat(symbols, label);
        symbols.clear();
        ++lineCount;
      }
      stream << lineCount << " " << symbolSet.size() << "\n" << symbolString.str() << "\n";
    }
  }
  else if(globalParameters.getStreamMode() == StreamMode::StreamMode){
    throw new std::invalid_argument("Stream mode not implemented in PairwiseBastaTransformer.");
  }
}

/**
 * @brief Converts all connections of one host to to abbadingo.
 * 
 * Whether we get all netflows pairwise or just process the entire host and do omit the dst-information
 * can be set in the parameters of the transformer.
 * 
 * @param stream The stream to write to.
 * @param hostAddress The address of the host.
 * @param host The host object.
 */
void PairwiseBastaTransformer::writeConnection(std::stringstream& stream, const std::string& hostAddress, const Host& host) const {
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto featureInformation = dynamic_cast<PairwiseBastaFeatures*>(transformParameters);
  static auto featureString = getFeatureString(featureInformation->getAllFeatureNames());

  static const bool filterByDst = featureInformation->filterByDestination();

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){
    stream << "Host address: " << hostAddress << "\n";
    stream << "Encoded features: " << featureString << "\n";

    unsigned int lineCount = 0;

    std::unordered_map<unsigned int, unsigned int> symbolSet; // keep track of size of alphabet for this host
    std::stringstream symbolString;

    for(auto& dst: host.getLabeledNetflows()){
      std::vector<unsigned int> symbols;

      dynamic_cast<FixedSizeWindow*>(window)->setIsInitialized(false);
      auto flows = dst.second.second;

      const auto& label = host.getLabel(dst.first);

      while(!flows.empty()){
        auto lines = dynamic_cast<FixedSizeWindow*>(window)->getWindow(flows); // TODO: we could spare the copy here
        for(auto& line: lines){
          if(line.empty()){
            continue;
          }

          const auto code = encodeStream(line);
          if(symbolSet.count(code) == 0){
            // make the symbols outnumbered from zero to size
            symbolSet[code] = symbolSet.size();
          }

          symbols.push_back(symbolSet.at(code));
        }

        assert(symbols.size() <= globalParameters.getWindowSize());

        symbolString << toAbbadingoFormat(symbols, label);
        symbols.clear();
        ++lineCount;
      }
    }
    stream << lineCount << " " << symbolSet.size() << "\n" << symbolString.str() << "\n";
  }
}

/**
 * @brief Converts entire connection and writes it into the stream.
 * 
 * Creates a new file for each source-destination pair.
 * 
 * @param stream Stream to write to.
 * @param host The host we will transform to a string.
 */
void PairwiseBastaTransformer::writeConnection(std::stringstream& stream, const std::pair< std::string, Host >& host, const unsigned int hostNumber) const {
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto& outputFile = globalParameters.getOutputFile();

  const auto featureInformation = dynamic_cast<PairwiseBastaFeatures*>(transformParameters);
  static const auto featureString = getFeatureString(featureInformation->getAllFeatureNames());

  const auto batchSize = featureInformation->getBatchSize();
  if(batchSize <= 0){
    std::cerr << "Batch size must be set and be larger than zero to run this mode." << std::endl;
    throw new std::runtime_error("Batchsize should be larger than 0 when this function is called.");
  }

  const auto& ipAddress = host.first;
  std::set<unsigned int> symbolSet; // keep track of size of alphabet for this host(src)-dst pair
  std::vector<unsigned int> symbols;
  for(auto& dst: host.second.getLabeledNetflows()){
    std::stringstream symbolString;
    int lineCount = 0;
    int batchCounter = 0;
    std::vector< std::string > batch;

    const auto& flows = dst.second.second;
    for(int i = 0; i < flows.size(); ++i){
      const auto& flow = flows.at(i);
      // collect a batch
      batch.push_back(flow);
      ++batchCounter;
      if(batchCounter == batchSize || i == flows.size() - 1){
        batchCounter = 0;
      }
      else{
        continue;
      }

      // stream the batch
      std::stringstream labelStream;
      const auto& label = host.second.getLabel(dst.first);
      labelStream << label << " ";

      std::vector<unsigned int> symbols;
      for(const auto& flow: batch){
        if(flow.empty()){
          continue;
        }

        const auto code = encodeStream(flow);
        symbolSet.insert(code);
        symbols.push_back(code);
      }
      symbolString << toAbbadingoFormat(symbols, labelStream.str()) << "\n";
      ++lineCount;
      batch.clear();
      batch.resize(0);
    }
    stream << lineCount << " " << symbolSet.size() << "\n" << symbolString.str() << "\n";
  }
}