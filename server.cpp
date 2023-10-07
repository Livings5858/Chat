#include <iostream>
#include "tcp_server.h"

int main() {
    TCPServer server;
    if (server.Initialize()) {
        server.Run();
    }

    return 0;
}
