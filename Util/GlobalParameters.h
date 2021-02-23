/**
 * @file GlobalHelpers.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief This file defines global helpers, i.e. helpers that can be used by projects including this project.
 * @version 0.1
 * @date 2021-02-01
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _HELPERS_
#define _HELPERS_

#include <string>
#include <stdexcept>

/********************** Switches defining methods to use. ********************/

enum class StreamMode{
  BatchMode,
  StreamMode
};

enum class InputFileFormat{
  Netflow
};

enum class OutputFileFormat{
  Abbadingo,
  AugmentedAbbadingo
};

enum class TransformerType{
  BastaTransformer,
  PairwiseBastaTransformer
};

enum class WindowType{
  FixedSizeWindow
};


/**
 * @brief Singleton implementation of the global parameters.
 * 
 */
class GlobalParameters{  
private:
  GlobalParameters(){};

  std::string inputFile;
  std::string outputFile;
  std::string featureIni;

  int windowSize;
  int windowStride;

  char inFileDelimiter = ' ';
  char inputFileNewlineCharacter = '\n';
  
  StreamMode streamMode;
  InputFileFormat ifFormat;
  OutputFileFormat ofFormat;
  TransformerType transformerType;
  WindowType windowType;

public:
  static GlobalParameters& getInstance(){
    static GlobalParameters instance;
    return instance;
  }

  GlobalParameters(GlobalParameters const&) = delete;
  void operator=(GlobalParameters const&) = delete;

  /**************************** getter *******************************/

  inline const auto getWindowSize() const{
    return windowSize;
  }

  inline const auto getWindowStride() const{
    return windowStride;
  }

  inline const char getInputFileDelimiter() const{
    return inFileDelimiter;
  }

  inline const char getInputFileNewlineCharacter() const{
    return inputFileNewlineCharacter;
  }

  inline const auto& getInputFile() const{
    return inputFile;
  }

  inline const auto& getOutputFile() const{
    return outputFile;
  }

  inline const auto& getFeatureIni() const{
    return featureIni;
  }

  inline const auto getStreamMode() const{
    return streamMode;
  }

  inline const auto getInputfileFormat() const{
    return ifFormat;
  }

  inline const auto getOutputfileFormat() const{
    return ofFormat;
  }

  inline const auto getTransformerType() const{
    return transformerType;
  }

  inline const auto getWindowType() const{
    return windowType;
  }

  /**************************** setter ****************************/

  inline void setWindowSize(const int windowSize){
    this->windowSize = windowSize;
  }

  inline void setWindowStride(const int windowStride){
    this->windowStride = windowStride;
  }

  inline void setInputFile(const std::string& inputFile){
    this->inputFile = inputFile;
  }

  inline void setOutputFile(const std::string& outputFile){
    this->outputFile = outputFile;
  }

  inline void setFeatureIni(const std::string& featureIni){
    this->featureIni = featureIni;
  }

  inline void setDelimiter(const char delimiter){
    this->inFileDelimiter = delimiter;
  }

  inline void setStreamMode(const int i){
    if(i == 0){
      this->streamMode = StreamMode::BatchMode;
    }
    else if(i == 1){
      this->streamMode = StreamMode::StreamMode;
    }
    else{
      throw new std::invalid_argument("Stream mode argument not implemented.");
    }
  }

  inline void setInputfileFormat(const int i){
    if(i == 0){
      this->ifFormat = InputFileFormat::Netflow;
    }
    else{
      throw new std::invalid_argument("Input file format argument not implemented.");
    }
  }

  inline void setOutputfileFormat(const int i){
    if(i == 0){
      this->ofFormat = OutputFileFormat::Abbadingo;
    }
    else if(i == 1){
      this->ofFormat = OutputFileFormat::AugmentedAbbadingo;
    }
    else{
      throw new std::invalid_argument("Output file format argument not implemented.");
    }
  }

  inline void setTransformerType(const int i){
    if(i == 0){
      this->transformerType = TransformerType::BastaTransformer;
    }
    else if(i == 1){
      this->transformerType = TransformerType::PairwiseBastaTransformer;
    }
    else{
      throw new std::invalid_argument("Transformer type argument not implemented.");
    }
  }

  inline void setWindowType(const int i){
    if(i == 0){
      this->windowType = WindowType::FixedSizeWindow;
    }
    else{
      throw new std::invalid_argument("Window type argument not implemented.");
    }
  }
};

#endif