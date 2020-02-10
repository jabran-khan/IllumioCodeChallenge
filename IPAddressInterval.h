#pragma once

#include <vector>
#include <string>

class IPAddressInterval {
    std::vector<int> start, end;
    std::vector<int> getIPAddressFromString(std::string addr);
  public:
    IPAddressInterval(std::string addr1, std::string addr2);
    IPAddressInterval(std::vector<int> addr1, std::vector<int> addr2);
    bool isAddressInInterval(std::string addr);
    std::vector<int> getStart();
    std::vector<int> getEnd();
};