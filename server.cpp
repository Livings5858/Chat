#include <iostream>
#include "tcp_server.h"
#include "chat_message.h"
#include "server_message_handler.h"

void OnRecvMessage(int clientSocket, const std::string& message) {
    ChatMessage msg = DeserializeChatMessage(message);
    std::cout << "OnRecvMessage:" << std::endl;
    std::cout << "MessageType:" << msg.type << std::endl;
    std::cout << "From:" << msg.from << std::endl;
    std::cout << "To:" << msg.to << std::endl;
    std::cout << "Message:" << msg.message << std::endl;
    ServerMessageHandler serverMessageHandler;
    serverMessageHandler.HandleMessage(clientSocket, msg);
}

int main() {
    TCPServer server(OnRecvMessage);
    if (server.Start()) {
        while(true);
    }

    return 0;
}
