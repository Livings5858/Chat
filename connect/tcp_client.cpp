#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_client.h"

TCPClient::TCPClient(const char* serverIP, int serverPort) :
    serverIP_(serverIP), serverPort_(serverPort), clientSocket_(-1) {}

TCPClient::~TCPClient() {
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
