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
#include <atomic>
#include <thread>


class TCPServer {
public:
    TCPServer();
    ~TCPServer();
    bool Start();
    void Stop();

private:
    bool Initialize();
    void Run();
    void HandleNewConnection();
    void HandleClientData(int clientSocket);

private:
    int serverSocket;
    struct sockaddr_in serverAddr;
    int epollFd;
    std::atomic<bool> stopRequested;
    std::thread serverThread;
};

#endif
