#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class TCPClient {
public:
    TCPClient(const char* server_ip, int server_port);
    ~TCPClient();
    bool Initialize();
    void SendMessage();
    int SendMessage(std::string message);

private:
    const char* server_ip_;
    int server_port_;
    int clientSocket;
    struct sockaddr_in serverAddr;
};

#endif
