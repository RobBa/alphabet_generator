/**
 * @file StochasticWindow.cpp
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief A window of size 1 and stride 1. Used to collect batches.
 *  * 
 * @version 0.1
 * @date 2021-03-09
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __STOCHASTIC_WINDOW_
#define __STOCHASTIC_WINDOW_

#include "WindowBase.h"

class StochasticWindow : public WindowBase{

private:
  bool initialized;

  void resetStreamState(std::ifstream& inputStream) const;

public:
  StochasticWindow() : WindowBase(1) {};

  std::vector<std::string> getWindow(std::ifstream& inputStream) const override;
  std::string getStreamedLine(std::ifstream& inputStream) const;
};

#endif