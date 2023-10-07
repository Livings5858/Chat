#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_client.h"

TCPClient::TCPClient(const char* server_ip, int server_port) :
    server_ip_(server_ip), server_port_(server_port), clientSocket(-1) {}

TCPClient::~TCPClient() {
    if (clientSocket != -1) {
        close(clientSocket);
    }
}

bool TCPClient::Initialize() {
    // 创建客户端套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("创建套接字失败");
        return false;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port_);

    if (inet_pton(AF_INET, server_ip_, &serverAddr.sin_addr) <= 0) {
        perror("设置服务器地址失败");
        close(clientSocket);
        return false;
    }

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("连接到服务器失败");
        close(clientSocket);
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

        if (send(clientSocket, message, strlen(message), 0) == -1) {
            perror("发送消息失败");
            break;
        }
    }
}
