#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include "tcp_server.h"
#include "chat_message.h"
#include "logger.h"

const int MAX_EVENTS = 10;
const int PORT = 8080;

TCPServer::TCPServer() :
    serverSocket_(-1),
    epollFd_(-1),
    stopRequested_(false),
    recvCallback_(nullptr) {}

TCPServer::TCPServer(RecvCallbackFunction callback) :
    serverSocket_(-1),
    epollFd_(-1),
    stopRequested_(false),
    recvCallback_(callback) {}

TCPServer::~TCPServer() {
    if (serverSocket_ != -1) {
        close(serverSocket_);
    }
}

bool TCPServer::Initialize() {
    // 创建监听套接字
    serverSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket_ == -1) {
        LOGE("Create socket failed: %s", strerror(errno));
        return false;
    }

    // 设置服务器地址
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_addr.s_addr = INADDR_ANY;
    serverAddr_.sin_port = htons(PORT);

    // 绑定地址和端口
    if (bind(serverSocket_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_)) == -1) {
        LOGE("Bind failed: %s", strerror(errno));
        close(serverSocket_);
        return false;
    }

    // 开始监听
    if (listen(serverSocket_, 5) == -1) {
        LOGE("Listen Failed: %s", strerror(errno));
        close(serverSocket_);
        return false;
    }

    // 创建 epoll 实例
    epollFd_ = epoll_create(1);
    if (epollFd_ == -1) {
        LOGE("Create epoll failed: %s", strerror(errno));
        close(serverSocket_);
        return false;
    }

    // 添加监听套接字到 epoll 实例
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serverSocket_;
    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, serverSocket_, &event) == -1) {
        LOGE("Add server socket event for epoll failed: %s", strerror(errno));
        close(serverSocket_);
        close(epollFd_);
        return false;
    }

    LOGI("Waitting for client...");

    return true;
}

void TCPServer::Run() {
    while (!stopRequested_.load()) {
        struct epoll_event events[MAX_EVENTS];
        int numEvents = epoll_wait(epollFd_, events, MAX_EVENTS, 3);
        if (numEvents == -1) {
            LOGE("Waitting event for epoll failed: %s", strerror(errno));
            break;
        }

        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == serverSocket_) {
                // 有新的连接请求
                HandleNewConnection();
            } else {
                // 有数据到达客户端
                HandleClientData(events[i].data.fd);
            }
        }
    }
}

void TCPServer::HandleNewConnection() {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(serverSocket_, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        LOGE("Accept failed: %s", strerror(errno));
        return;
    }

    // 添加客户端套接字到 epoll 实例
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = clientSocket;
    if (epoll_ctl(epollFd_, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
        LOGE("Add client socket event for epoll failed: %s", strerror(errno));
        close(clientSocket);
    }

    LOGI("Client [%d] %s:%d connected.", clientSocket, inet_ntoa(clientAddr.sin_addr),
        ntohs(clientAddr.sin_port));
}

void TCPServer::HandleClientData(int clientSocket) {
    size_t message_len = 0;
    std::string message;

    // 接收消息大小
    int bytesRead = recv(clientSocket, &message_len, sizeof(message_len), 0);
    if (bytesRead > 0) {
        char* buffer = new char[message_len + 1];
        bytesRead = recv(clientSocket, buffer, message_len, 0);
        buffer[message_len] = '\0';
        message = buffer;
        delete buffer;
    }

    if (bytesRead <= 0) {
        epoll_ctl(epollFd_, EPOLL_CTL_DEL, clientSocket, NULL);
        close(clientSocket);
        LOGI("Client [%d] disconnected.", clientSocket);
        return;
    }

    if (recvCallback_) {
        recvCallback_(clientSocket, message);
    }
}

bool TCPServer::Start() {
    stopRequested_.store(false);
    if (!Initialize()) {
        return false;
    }
    serverThread_ = std::thread(&TCPServer::Run, this);

    return true;
}

void TCPServer::Stop() {
    stopRequested_.store(true); // 设置标志变量以请求线程停止
    if (serverThread_.joinable()) {
        serverThread_.join();
    }
    LOGI("server stoping...");
}

void TCPServer::SetCallback(RecvCallbackFunction callback) {
    recvCallback_ = callback;
}
