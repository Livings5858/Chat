#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "tcp_client.h"
#include "chat_message.h"

const char *SERVER_IP = "127.0.0.1";
const int SERVER_PORT = 8080;

int main() {
    TCPClient client(SERVER_IP, SERVER_PORT);
    if (!client.Initialize()) {
        return -1;
    }
    char message[1024];

    while (true) {
        std::cout << "请输入消息: ";
        std::cin.getline(message, sizeof(message));
        if (strlen(message) == 0) {
            continue;
        }
        ChatMessage msg {
            .type = MSG_TEXT,
            .from = "client",
            .to = "server",
            .message = message
        };

        if (client.SendMessage(SerializeChatMessage(msg)) != 0) {
            perror("发送消息失败");
            break;
        }
    }

    return 0;
}