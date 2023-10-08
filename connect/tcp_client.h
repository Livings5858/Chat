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
    TCPClient(const char* serverIP, int serverPort);
    ~TCPClient();
    bool Initialize();
    void SendMessage();
    int SendMessage(std::string message);

private:
    const char* serverIP_;
    int serverPort_;
    int clientSocket_;
    struct sockaddr_in serverAddr_;
};

#endif
