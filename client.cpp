#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

const char *SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char message[1024];

    // 创建客户端套接字
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("创建套接字失败");
        exit(EXIT_FAILURE);
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) <= 0) {
        perror("设置服务器地址失败");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    // 连接到服务器
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("连接到服务器失败");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    std::cout << "已连接到服务器" << std::endl;

    while (true) {
        std::cout << "请输入消息: ";
        std::cin.getline(message, sizeof(message));

        if (send(clientSocket, message, strlen(message), 0) == -1) {
            perror("发送消息失败");
            break;
        }
    }

    // 关闭套接字
    close(clientSocket);

    return 0;
}
