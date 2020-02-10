#include <iostream>
#include <algorithm>
#include "IPAddressInterval.h"
using namespace std;

IPAddressInterval::IPAddressInterval(string addr1, string addr2):
    start{getIPAddressFromString(addr1)}, end{getIPAddressFromString(addr2)} {}

IPAddressInterval::IPAddressInterval(vector<int> addr1, vector<int> addr2):
    start{addr1}, end{addr2} {}

bool IPAddressInterval::isAddressInInterval(string addr) {
    vector<int> address = getIPAddressFromString(addr);
    for (int i = 0; i < 4; i++) {
        // outside of bounds
        if (address[i] < start[i] || address[i] > end[i]) return false;
        // strictly inside bounds
        else if (address[i] > start[i] && address[i] < end[i]) return true;
        // octet values for start, end are the same
        else if (address[i] == start[i] && address[i] == end[i]) continue;
        // equal to octet of the starting interval address
        else if (address[i] == start[i]) {
            for (int j = i + 1; j < 4; j++) {
                if (address[j] < start[j]) return false;
            }
            return true;
        }
        // equal to the octet of the ending interval address
        else {
            for (int j = i + 1; j < 4; j++) {
                if (address[j] > end[j]) return false;
            }
            return true;
        }
    }
    return true;
}

vector<int> IPAddressInterval::getIPAddressFromString(string addr) {
    vector<int> address(4, 0);
    size_t pos = addr.find(".");
    address[0] = stoi(addr.substr(0, pos));
    addr = addr.substr(pos + 1);
    pos = addr.find(".");
    address[1] = stoi(addr.substr(0, pos));
    addr = addr.substr(pos + 1);
    pos = addr.find(".");
    address[2] = stoi(addr.substr(0, pos));
    addr = addr.substr(pos + 1);
    address[3] = stoi(addr);
    return address;
}

vector<int> IPAddressInterval::getStart() {
    return start;
}

vector<int> IPAddressInterval::getEnd() {
    return end;
}