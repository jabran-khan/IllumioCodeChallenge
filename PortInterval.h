#pragma once

#include <string>
#include <vector>
#include "IPAddressInterval.h"

class PortInterval {
    int start, end;
    std::vector<IPAddressInterval> listAddresses;
    void addAddressToInterval(std::string addr1, std::string addr2);
    bool addressLessThan(std::vector<int> addr1, std::vector<int> addr2);
    bool addressGreaterThan(std::vector<int> addr1, std::vector<int> addr2);
  public:
    PortInterval(int port1, int port2, std::string addr1, std::string addr2);
    PortInterval(int port1, int port2, std::vector<IPAddressInterval> listAddresses);
    std::vector<IPAddressInterval> getAddresses();
    bool isPortInInterval(int port, std::string addr);
    int getStart();
    int getEnd();
};