#include "Firewall.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

Firewall::Firewall(string path) {
    try {
        ifstream fin(path);
        string rule;

        while (getline(fin, rule)) {
            parseRule(rule);
        }
    } catch (...) {
        cerr << "error reading file";
        exit(1);
    }
}

void Firewall::parseRule(string rule) {
    string dir, protocol, port, IP;

    // break rule into different parts
    size_t pos = rule.find(",");
    dir = rule.substr(0, pos);
    rule = rule.substr(pos + 1);
    pos = rule.find(",");
    protocol = rule.substr(0, pos);
    rule = rule.substr(pos + 1);
    pos = rule.find(",");
    port = rule.substr(0, pos);
    rule = rule.substr(pos + 1);
    IP = rule;

    // find port range
    int port1, port2;
    pos = port.find("-");
    if (pos != string::npos) {
        port1 = stoi(port.substr(0, pos));
        port2 = stoi(port.substr(pos + 1));
    } else {
        port1 = stoi(port);
        port2 = stoi(port);
    }

    // find IP address range
    string ip1, ip2;
    pos = IP.find("-");
    if (pos != string::npos) {
        ip1 = IP.substr(0, pos);
        ip2 = IP.substr(pos + 1);
    } else {
        ip1 = IP;
        ip2 = IP;
    }

    // create PortInterval and add it to a list
    PortInterval interval(port1, port2, ip1, ip2);
    if (dir == "inbound" && protocol == "TCP") {
        return addPortInterval(inboundTCP, interval);
    } else if (dir == "outbound" && protocol == "TCP") {
        return addPortInterval(outboundTCP, interval);
    } else if (dir == "inbound" && protocol == "UDP") {
        return addPortInterval(inboundUDP, interval);
    } else {
        return addPortInterval(outboundUDP, interval);
    }
}

void Firewall::addPortInterval(vector<PortInterval> &listPorts, PortInterval interval) {
    if (listPorts.empty()) { listPorts.push_back(interval); return;}

    vector<PortInterval> portStack;
    portStack.push_back(interval);
    for (unsigned int i = 0; i < listPorts.size(); i++) {
        PortInterval incomingInterval = listPorts[i];
        while (true) {
            if (portStack.empty()) {
                portStack.push_back(incomingInterval);
                break;
            }
            PortInterval topInterval = portStack.back();
            if (topInterval.getEnd() < incomingInterval.getStart()) {
                portStack.push_back(incomingInterval);
                break;
            } else if (topInterval.getStart() > incomingInterval.getEnd()) {
                portStack.pop_back();
                portStack.push_back(incomingInterval);
                portStack.push_back(topInterval);
                break;
            } else {
                int newStartPort, newEndPort;
                if (topInterval.getStart() < incomingInterval.getStart()) {
                    newStartPort = topInterval.getStart();
                } else {
                    newStartPort = incomingInterval.getStart();
                }

                if (topInterval.getEnd() > incomingInterval.getEnd()) {
                    newEndPort = topInterval.getEnd();
                } else {
                    newEndPort = incomingInterval.getEnd();
                }

                portStack.pop_back();
                vector<IPAddressInterval> concatList = topInterval.getAddresses();
                concatList.insert(concatList.end(), incomingInterval.getAddresses().begin(), incomingInterval.getAddresses().end());
                incomingInterval = PortInterval(newStartPort, newEndPort, concatList);
            }
        }
    }

    listPorts = portStack;
}

bool Firewall::acceptPacket(string dir, string protocol, int port, string IP) {
    if (dir == "inbound" && protocol == "TCP") {
        return checkIfPacketFits(inboundTCP, port, IP);
    } else if (dir == "outbound" && protocol == "TCP") {
        return checkIfPacketFits(outboundTCP, port, IP);
    } else if (dir == "inbound" && protocol == "UDP") {
        return checkIfPacketFits(inboundUDP, port, IP);
    } else {
        return checkIfPacketFits(outboundUDP, port, IP);
    }
}

bool Firewall::checkIfPacketFits(vector<PortInterval> &listPortInterval, int port, string addr) {
    for (PortInterval portInterval : listPortInterval) {
        if (portInterval.isPortInInterval(port, addr)) return true;
    }
    return false;
}