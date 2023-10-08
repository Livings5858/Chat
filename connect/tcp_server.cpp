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

const int MAX_EVENTS = 10;
const int PORT = 8080;

TCPServer::TCPServer() :
    serverSocket(-1),
    epollFd(-1),
    stopRequested(false),
    recvCallback(nullptr) {}

TCPServer::TCPServer(RecvCallbackFunction callback) :
    serverSocket(-1),
    epollFd(-1),
    stopRequested(false),
    recvCallback(callback) {}

TCPServer::~TCPServer() {
    if (serverSocket != -1) {
        close(serverSocket);
    }
}

bool TCPServer::Initialize() {
    // 创建监听套接字
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("创建套接字失败");
        return false;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 绑定地址和端口
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("绑定失败");
        close(serverSocket);
        return false;
    }

    // 开始监听
    if (listen(serverSocket, 5) == -1) {
        perror("监听失败");
        close(serverSocket);
        return false;
    }

    // 创建 epoll 实例
    epollFd = epoll_create(1);
    if (epollFd == -1) {
        perror("创建 epoll 失败");
        close(serverSocket);
        return false;
    }

    // 添加监听套接字到 epoll 实例
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = serverSocket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, serverSocket, &event) == -1) {
        perror("添加监听套接字到 epoll 失败");
        close(serverSocket);
        close(epollFd);
        return false;
    }

    std::cout << "等待客户端连接..." << std::endl;

    return true;
}

void TCPServer::Run() {
    while (!stopRequested.load()) {
        struct epoll_event events[MAX_EVENTS];
        int numEvents = epoll_wait(epollFd, events, MAX_EVENTS, 3);
        if (numEvents == -1) {
            perror("等待事件失败");
            break;
        }

        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == serverSocket) {
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
    int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        perror("接受连接失败");
        return;
    }

    // 添加客户端套接字到 epoll 实例
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = clientSocket;
    if (epoll_ctl(epollFd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
        perror("添加客户端套接字到 epoll 失败");
        close(clientSocket);
    }

    std::cout << "客户端 " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port)
                << " 连接成功" << std::endl;
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
        epoll_ctl(epollFd, EPOLL_CTL_DEL, clientSocket, NULL);
        close(clientSocket);
        std::cout << "客户端断开连接" << std::endl;
        return;
    }

    if (recvCallback) {
        recvCallback(message);
    }
}

bool TCPServer::Start() {
    stopRequested.store(false);
    if (!Initialize()) {
        return false;
    }
    serverThread = std::thread(&TCPServer::Run, this);

    return true;
}

void TCPServer::Stop() {
    stopRequested.store(true); // 设置标志变量以请求线程停止
    if (serverThread.joinable()) {
        serverThread.join();
    }
    std::cout << "server stoping..." << std::endl;
}

void TCPServer::SetCallback(RecvCallbackFunction callback) {
    recvCallback = callback;
}
