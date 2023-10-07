#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_client.h"

const char *SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;

int main() {
    TCPClient client(SERVER_IP, SERVER_PORT);
    if (client.Initialize()) {
        client.SendMessage();
    }

    return 0;
}