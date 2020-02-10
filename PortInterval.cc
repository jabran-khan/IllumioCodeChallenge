#include "PortInterval.h"
#include <stack>

using namespace std;

PortInterval::PortInterval(int port1, int port2, string addr1, string addr2):
    start{port1}, end{port2} {
    addAddressToInterval(addr1, addr2);
}

PortInterval::PortInterval(int port1, int port2, vector<IPAddressInterval> listAddresses):
    start{port1}, end{port2}, listAddresses{listAddresses} {}

bool PortInterval::isPortInInterval(int port, string addr) {
    if (port < start || port > end) return false;
    for (IPAddressInterval interval : listAddresses) {
        if (interval.isAddressInInterval(addr)) return true;
    }
    return false;
}

vector<IPAddressInterval> PortInterval::getAddresses() {
    return listAddresses;
}

int PortInterval::getStart() {
    return start;
}

int PortInterval::getEnd() {
    return end;
}

void PortInterval::addAddressToInterval(string addr1, string addr2) {
    IPAddressInterval interval(addr1, addr2);
    if (listAddresses.empty()) { listAddresses.push_back(interval); return;}

    // we want to store the intervals in sorted order using their starting intervals
    // we should ensure that we do not have overlapping intervals
    // use a stack to do this with each address in list address
    vector<IPAddressInterval> addressStack;
    addressStack.push_back(interval);
    for (unsigned int i = 0; i < listAddresses.size(); i++) {
        IPAddressInterval incomingInterval = listAddresses[i];
        while (true) {
            // top interval in stack is less than incoming interval and does not overlap
            if (addressStack.empty()) {
                addressStack.push_back(incomingInterval);
                break;
            }
            IPAddressInterval topInterval = addressStack.back();
            if (addressLessThan(topInterval.getEnd(), incomingInterval.getStart())) {
                addressStack.push_back(incomingInterval);
                break;
            }
            // top interval in stack is greater than incoming interval and does not overlap
            else if (addressGreaterThan(topInterval.getStart(), incomingInterval.getEnd())) {
                addressStack.pop_back();
                addressStack.push_back(incomingInterval);
                addressStack.push_back(topInterval);
                break;
            }
            // top interval and incoming interval overlap, replace both with one interval
            else {
                vector<int> newIntervalStart, newIntervalEnd;
                if (addressLessThan(topInterval.getStart(), incomingInterval.getStart())) {
                    newIntervalStart = topInterval.getStart();
                } else {
                    newIntervalStart = incomingInterval.getStart();
                } 

                if (addressGreaterThan(topInterval.getEnd(), incomingInterval.getEnd())) {
                    newIntervalEnd = topInterval.getEnd();
                } else {
                    newIntervalEnd = incomingInterval.getEnd();
                }

                addressStack.pop_back();
                incomingInterval = IPAddressInterval(newIntervalStart, newIntervalEnd);
            }
        }
    }

    listAddresses = addressStack;
}

bool PortInterval::addressLessThan(vector<int> addr1, vector<int> addr2) {
    for (int i = 0; i < 4; i++) {
        if (addr1[i] < addr2[i]) break;
        else if (addr1[i] > addr2[i]) return false;
    }
    return true;
}

bool PortInterval::addressGreaterThan(vector<int> addr1, vector<int> addr2) {
    for (int i = 0; i < 4; i++) {
        if (addr1[i] > addr2[i]) break;
        else if (addr1[i] < addr2[i]) return false;
    }
    return true;
}
