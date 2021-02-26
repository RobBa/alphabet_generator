/**
 * @file Host.h
 * @author Robert Baumgartner (r.baumgartner-1@tudelft.nl)
 * @brief Class where we model a host.
 * @version 0.1
 * @date 2021-02-19
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifndef _HOST_
#define _HOST_

#include <string>
#include <vector>
#include <unordered_map>

/**
 * @brief A class that models a connection. It holds 
 * 
 */
class Host{
private:
  /**
   * @brief Mapping the netflows, dst-address to a vector of netflows.
   * 
   */
  std::unordered_map<std::string, std::vector<std::string> > netflows;

  int label = 0;

public:
  Host() = default;

  void addNetflow(const std::string& ipAddress, const std::string& netflow);
  void addNetflow(const std::string& ipAddress, const std::string& netflow, const int label);
  
  std::vector<std::string> getConnection(const std::string& dstAddress) const;

  inline auto getNetflows() const {
    return this->netflows;
  }

  inline void setlabel(int label) noexcept {
    this->label = label;
  }

  inline const auto getLabel() const noexcept {
    return label;
  }
};

#endif