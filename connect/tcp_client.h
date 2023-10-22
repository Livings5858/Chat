#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include <atomic>

class TCPClient {
public:
    TCPClient(const char* serverIP, int serverPort);
    ~TCPClient();
    bool Initialize();
    int SendMessage(std::string message);

private:
    void OnRecvMessage();
    const char* serverIP_;
    int serverPort_;
    int clientSocket_;
    struct sockaddr_in serverAddr_;
    std::atomic<bool> stopRequested_;
    std::thread clientThread_;
};

#endif
