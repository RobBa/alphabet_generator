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
  // The netflow with the destination along with a label of type T (templated).
  using DstPair = std::pair<int, std::vector<std::string> >;

private:
  /**
   * @brief Mapping the netflows, dst-address to a vector of netflows.
   * 
   */
  std::unordered_map<std::string, std::vector<std::string> > unlabeledNetflows;

  /**
   * @brief Mapping the netflows, dst-address to a labeled vector of netflows.
   * 
   */
  std::unordered_map<std::string, DstPair> labeledNetflows;

public:
  Host() = default;

  void addNetflow(const std::string& ipAddress, const std::string& netflow);
  void addNetflow(const std::string& ipAddress, const std::string& netflow, const int label);
  
  std::vector<std::string> getConnection(const std::string& dstAddress) const;

  inline auto getLabeledNetflows() const {
    return this->labeledNetflows;
  }

  inline void setlabel(std::string dstAddress, const int label) noexcept {
    labeledNetflows.at(dstAddress).first = label;
  }

  inline const auto& getLabel(std::string dstAddress) const {
    return labeledNetflows.at(dstAddress).first;
  }
};

#endif