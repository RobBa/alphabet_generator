/**
 * @file TestStreaming.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Simple test, as it failed with Python.
 * @version 0.1
 * @date 2021-02-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdexcept>
#include <string>
#include <fstream>

#include <unistd.h>

using namespace std;

const int MAX_LINES = 5000;
const uint TIME_MICROSECONDS = 50000;
const string inputFile = "../Data/example.txt";
const string outputFile = "./output.txt";

int main(){
  auto instream = ifstream(inputFile);
  auto outstream = ofstream(outputFile);

  for(auto i = 0; i < MAX_LINES; ++i){
    string line;
    if(!getline(instream, line)){
      throw new runtime_error("Getline did not work");
    } 

    outstream << line << "\n";
    usleep(TIME_MICROSECONDS);
  }

  instream.close();
  outstream.close();

  return 0;
}