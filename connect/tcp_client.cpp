#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_client.h"
#include "client_message_handler.h"
#include <thread>

TCPClient::TCPClient(const char* serverIP, int serverPort) :
    serverIP_(serverIP), serverPort_(serverPort), clientSocket_(-1), stopRequested_(false) {}

TCPClient::~TCPClient() {
    std::cout << "client stoping..." << std::endl;
    stopRequested_.store(true);
    if (clientThread_.joinable()) {
        clientThread_.join();
    }
    if (clientSocket_ != -1) {
        close(clientSocket_);
    }
}

bool TCPClient::Initialize() {
    // 创建客户端套接字
    clientSocket_ = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket_ == -1) {
        perror("创建套接字失败");
        return false;
    }

    struct timeval timeout;
    timeout.tv_sec = 5;  // 设置超时时间为5秒
    timeout.tv_usec = 0;

    if (setsockopt(clientSocket_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
    }

    // 设置服务器地址
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(serverPort_);

    if (inet_pton(AF_INET, serverIP_, &serverAddr_.sin_addr) <= 0) {
        perror("设置服务器地址失败");
        close(clientSocket_);
        return false;
    }

    // 连接到服务器
    if (connect(clientSocket_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_)) == -1) {
        perror("连接到服务器失败");
        close(clientSocket_);
        return false;
    }

    std::cout << "已连接到服务器" << std::endl;
    stopRequested_.store(false);
    clientThread_ = std::thread(&TCPClient::OnRecvMessage, this);
    std::cout << "开启消息接收线程" << std::endl;

    return true;
}

void TCPClient::SendMessage() {
    char message[1024];

    while (true) {
        std::cout << "请输入消息: ";
        std::cin.getline(message, sizeof(message));

        if (send(clientSocket_, message, strlen(message), 0) == -1) {
            perror("发送消息失败");
            break;
        }
    }
}

int TCPClient::SendMessage(std::string message) {
    size_t message_len = message.length();
    if (message_len == 0) {
        return 0;
    }
    if (send(clientSocket_, &message_len, sizeof(message_len), 0) == -1) {
        perror("发送消息长度失败");
        return errno;
    }
    if (send(clientSocket_, message.c_str(), message.length(), 0) == -1) {
        perror("发送消息内容失败");
        return errno;
    }

    return 0;
}

void TCPClient::OnRecvMessage() {
    ClientMessageHandler clientMessageHandler;
    while (!stopRequested_.load()) {
        size_t message_len = 0;
        std::string message;
        try {
            // 接收消息大小
            int bytesRead = recv(clientSocket_, &message_len, sizeof(message_len), 0);
            if (bytesRead > 0) {
                char* buffer = new char[message_len + 1];
                bytesRead = recv(clientSocket_, buffer, message_len, 0);
                buffer[message_len] = '\0';
                message = buffer;
                delete buffer;
            }

            if (bytesRead <= 0) {
                close(clientSocket_);
                std::cout << "服务器断开连接" << std::endl;
                return;
            }

            clientMessageHandler.HandleMessage(DeserializeChatMessage(message));
        }
        catch(const std::exception& e) {
            std::cerr << e.what() << '\n';
        }
    }
}