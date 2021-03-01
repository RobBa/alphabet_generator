/**
 * @file main.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief This is the main file of this project. It reads in command line
 * parameters and instantiates objects and variables accordingly, then 
 * runs the main process.
 * 
 * @version 0.1
 * @date 2021-02-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "InputParser.h"
#include "TransformerFactory.h"

#include <stdexcept>
#include <fstream>
#include <iostream>

/**
 * @brief The main method.
 * 
 */
int main(int argc, char **argv){
  if(argc != 2){
    throw new std::invalid_argument("Error: Please give a path to an input file. Program only accepts one argument.");
  }

  // From https://stackoverflow.com/a/27588225
  std::ifstream test(argv[1]); 
  if (!test){
    throw new std::invalid_argument("Error: Please give a valid input file");
  }

  std::string path = argv[1];
  HelperFunctions::parseInput(argv[1]);

  // TODO: implement this is in a nicer factory design pattern
  if(GlobalParameters::getInstance().getTransformerType() == TransformerType::StreamingBastaTransformer){
    auto transformer = new StreamingBastaTransformer;
    transformer->convert();
    free(transformer);
  }
  else if(GlobalParameters::getInstance().getTransformerType() == TransformerType::PairwiseBastaTransformer){
    auto transformer = new PairwiseBastaTransformer;
    transformer->convert();
    free(transformer);
  }
  //auto transformer = TransformerFactory::createTransformer();
  return 0;
}