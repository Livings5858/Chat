#ifndef SERVER_MESSAGE_HANDLER_H
#define SERVER_MESSAGE_HANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "chat_message.h"

class ServerMessageHandler {
public:
    ServerMessageHandler();
    ~ServerMessageHandler();
    void HandleCommandMessage(int clientSocket, const ChatMessage& msg);
    void HandleTextMessage(int clientSocket, const ChatMessage& msg);
    void HandleFileHeaderMessage(int clientSocket, const ChatMessage& msg);
    void HandleFileDataMessage(int clientSocket, const ChatMessage& msg);
    void HandleLoginMessage(int clientSocket, const ChatMessage& msg);
    void HandleLogoutMessage(int clientSocket, const ChatMessage& msg);
    void HandleRegisterMessage(int clientSocket, const ChatMessage& msg);
    void HandleUnRegisterMessage(int clientSocket, const ChatMessage& msg);
    void HandleForgotPasswordMessage(int clientSocket, const ChatMessage& msg);
    void HandleMessage(int clientSocket, const ChatMessage& msg);
private:
    void SendMessage(int clientSocket, const ChatMessage& msg);
    void eraseFromOnlineMap(std::string username);
    static std::map<std::string, int> onlineMap_;
    static std::map<std::string, std::string> userMap_;
};

#endif // SERVER_MESSAGE_HANDLER_H
