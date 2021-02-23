/**
 * @file FixedSizeWindow.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief A window of fixed size with respect to the number of data. 
 * 
 * Has been applied e.g. by Skoulos et al., for more information read "Learning State Machines 
 * faster using Locality-Sensitive Hashing and an application in network-based threat detection"
 * 
 * @version 0.1
 * @date 2021-01-26
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef __FIXED_SIZE_WINDOW_
#define __FIXED_SIZE_WINDOW_

#include "WindowBase.h"

class FixedSizeWindow : public WindowBase{

private:
  const int size; // The number of netflows the window gathers
  bool initialized;

public:
  FixedSizeWindow(const int size, const int stride)
   : WindowBase(stride), size(size){};

  std::vector<std::string> getWindow(std::ifstream& inputStream) const override;
  std::vector<std::string> getWindow(std::vector<std::string>& inputStream);

  inline void setIsInitialized(bool init) noexcept {
    initialized = init;
  }
};

#endif