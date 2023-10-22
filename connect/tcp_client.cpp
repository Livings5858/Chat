#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <thread>
#include "tcp_client.h"
#include "client_message_handler.h"
#include "logger.h"

TCPClient::TCPClient(const char* serverIP, int serverPort) :
    serverIP_(serverIP), serverPort_(serverPort), clientSocket_(-1), stopRequested_(false) {}

TCPClient::~TCPClient() {
    LOGI("client stoping...");
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
        LOGE("Create socket failed: %s", strerror(errno));
        return false;
    }

    struct timeval timeout;
    timeout.tv_sec = 5;  // 设置超时时间为5秒
    timeout.tv_usec = 0;

    if (setsockopt(clientSocket_, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        LOGE("setsockopt failed: %s", strerror(errno));
        close(clientSocket_);
        return false;
    }

    // 设置服务器地址
    serverAddr_.sin_family = AF_INET;
    serverAddr_.sin_port = htons(serverPort_);

    if (inet_pton(AF_INET, serverIP_, &serverAddr_.sin_addr) <= 0) {
        LOGE("Set server addresss failed: %s", strerror(errno));
        close(clientSocket_);
        return false;
    }

    // 连接到服务器
    if (connect(clientSocket_, (struct sockaddr *)&serverAddr_, sizeof(serverAddr_)) == -1) {
        LOGE("Connect to server failed: %s", strerror(errno));
        close(clientSocket_);
        return false;
    }

    LOGI("Connect to server successfully, clientSocket:%d", clientSocket_);
    stopRequested_.store(false);
    clientThread_ = std::thread(&TCPClient::OnRecvMessage, this);
    LOGI("Start message handler thread");

    return true;
}

int TCPClient::SendMessage(std::string message) {
    size_t message_len = message.length();
    if (message_len == 0) {
        return 0;
    }
    if (send(clientSocket_, &message_len, sizeof(message_len), 0) == -1) {
        LOGE("Send message length failed (ClientSocket: %d). %s", clientSocket_, strerror(errno));
        return errno;
    }
    if (send(clientSocket_, message.c_str(), message.length(), 0) == -1) {
        LOGE("Send message content failed (ClientSocket: %d). %s", clientSocket_, strerror(errno));
        return errno;
    }

    return 0;
}

void TCPClient::OnRecvMessage() {
    ClientMessageHandler clientMessageHandler;
    while (!stopRequested_.load()) {
        size_t message_len = 0;
        std::string message;
        // 接收消息大小
        int bytesRead = recv(clientSocket_, &message_len, sizeof(message_len), 0);
        if (bytesRead == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            } else if (errno == EINTR) {
                continue;
            }
            LOGE("Recv message length failed (ClientSocket: %d) %s", clientSocket_, strerror(errno));
            close(clientSocket_);
            return;
        } else if (bytesRead == 0) {
            LOGI("Server is disconnected (ClientSocket: %d) %s", clientSocket_, strerror(errno));
            close(clientSocket_);
            return;
        }

        // 读取消息内容
        char* buffer = new char[message_len + 1];
        bytesRead = recv(clientSocket_, buffer, message_len, 0);

        if (bytesRead == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                delete[] buffer;
                continue;
            } else if (errno == EINTR) {
                delete[] buffer;
                continue;
            }
            LOGE("recv message failed (ClientSocket: %d) %s", clientSocket_, strerror(errno));
            delete[] buffer;
            close(clientSocket_);
            return;
        } else if (bytesRead == 0) {
            LOGI("Server is disconnected (ClientSocket: %d)  %s", clientSocket_, strerror(errno));
            delete[] buffer;
            close(clientSocket_);
            return;
        }

        buffer[message_len] = '\0';
        message = buffer;
        delete[] buffer;

        clientMessageHandler.HandleMessage(DeserializeChatMessage(message));
    }
}