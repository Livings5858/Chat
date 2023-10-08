#include <iostream>
#include "tcp_server.h"

int main() {
    TCPServer server;
    if (server.Start()) {
        while(true);
    }

    return 0;
}
