#ifndef CLIENT_MESSAGE_HANDLER_H
#define CLIENT_MESSAGE_HANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include "chat_message.h"

class ClientMessageHandler {
public:
    void HandleCommandResponse(const ChatMessage& msg);
    void HandleTextResponse(const ChatMessage& msg);
    void HandleFileHeaderResponse(const ChatMessage& msg);
    void HandleFileDataResponse(const ChatMessage& msg);
    void HandleLoginResponse(const ChatMessage& msg);
    void HandleLogoutResponse(const ChatMessage& msg);
    void HandleRegisterResponse(const ChatMessage& msg);
    void HandleUnRegisterResponse(const ChatMessage& msg);
    void HandleForgotPasswordResponse(const ChatMessage& msg);
    void HandleMessage(const ChatMessage& msg);
private:
    void SendMessage(const ChatMessage& msg);
};


#endif // CLIENT_MESSAGE_HANDLER_H
