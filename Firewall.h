#pragma once
#include <vector>
#include "PortInterval.h"

class Firewall {
    std::vector<PortInterval> inboundTCP;
    std::vector<PortInterval> outboundTCP;
    std::vector<PortInterval> inboundUDP;
    std::vector<PortInterval> outboundUDP;
    void parseRule(std::string rule);
    void addPortInterval(std::vector<PortInterval> &listPorts, PortInterval interval);
    bool checkIfPacketFits(std::vector<PortInterval> &listPortInterval, int port, std::string addr);
  public:
    Firewall(std::string path);
    bool acceptPacket(std::string dir, std::string protocol, int port, std::string IP);
};