#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>


class TCPServer {
public:
    TCPServer();
    ~TCPServer();
    bool Initialize();
    void Run();

private:
    void HandleNewConnection();
    void HandleClientData(int clientSocket);

private:
    int serverSocket;
    struct sockaddr_in serverAddr;
    int epollFd;
};

#endif
