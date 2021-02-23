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

/**
 * @brief The constructor.
 * 
 */
PairwiseBastaTransformer::PairwiseBastaTransformer() : BastaTransformer(true) {
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto& featureIniFile = globalParameters.getFeatureIni();

  transformParameters = new PairwiseBastaFeatures;
  dynamic_cast<PairwiseBastaFeatures*>(transformParameters)->initFromFile(featureIniDir + featureIniFile);

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
  const auto& outputFormat = globalParameters.getOutputfileFormat();

  const auto featureParameters = dynamic_cast<PairwiseBastaFeatures*>(transformParameters);

  std::stringstream outStringStream;

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){
    const auto hosts = featureParameters->getHosts();
    for(const auto& host: hosts){
      writeConnection(outStringStream, host);

      outputStream << outStringStream.str();
      outStringStream.clear();
      outputStream.flush();
    }

    outputStream.close();
    inputStream.close();
  }

  else if(globalParameters.getStreamMode() == StreamMode::StreamMode){
    throw new std::invalid_argument("Stream mode not implemented in PairwiseBastaTransformer.");
  }
  else{
    throw new std::invalid_argument("Requested streaming mode not implemented");
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
void PairwiseBastaTransformer::writeConnection(std::stringstream& stream, const std::pair< std::string, Host > host) const {
  const auto& globalParameters = GlobalParameters::getInstance();
  const auto featureInformation = dynamic_cast<BastaFeatures*>(transformParameters);
  static auto featureString = getFeatureString(featureInformation->getAllFeatureNames());

  const auto& ipAddress = host.first;

  if(globalParameters.getStreamMode() == StreamMode::BatchMode){
    for(auto& dst: host.second.getNetflows()){
      std::stringstream dstString;
      std::stringstream symbolString;

      stream << ipAddress << " <-> " << dst.first << "\n";
      stream << featureString << "\n";

      unsigned int lineCount = 0;
      std::set<unsigned int> symbolSet; // keep track of size of alphabet for this host

      std::vector<unsigned int> symbols; 

      dynamic_cast<FixedSizeWindow*>(window)->setIsInitialized(false);
      auto flows = dst.second;

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

        symbolString << toAbbadingoFormat(symbols);
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