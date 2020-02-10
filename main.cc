#include <iostream>
#include "Firewall.h"

using namespace std;

int main() {
    Firewall fw("./fw.csv");
    if (fw.acceptPacket("inbound", "tcp", 80, "192.168.1.2")) {
        cout << true << endl;
    } else {
        cout << false << endl;
    }

    if (fw.acceptPacket("inbound", "udp", 53, "192.168.2.4")) {
        cout << true << endl;
    } else {
        cout << false << endl;
    }
}