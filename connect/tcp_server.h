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

typedef void (*RecvCallbackFunction)(int, const std::string&);

class TCPServer {
public:
    TCPServer();
    TCPServer(RecvCallbackFunction callback);
    ~TCPServer();
    bool Start();
    void Stop();
    void SetCallback(RecvCallbackFunction callback);

private:
    bool Initialize();
    void Run();
    void HandleNewConnection();
    void HandleClientData(int clientSocket);

private:
    int serverSocket_;
    struct sockaddr_in serverAddr_;
    int epollFd_;
    std::atomic<bool> stopRequested_;
    std::thread serverThread_;
    RecvCallbackFunction recvCallback_;
};

#endif
