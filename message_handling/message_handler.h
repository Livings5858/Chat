#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <iostream>
#include <string>
#include <vector>
#include "chat_message.h"

class MessageHandler {
public:
    void HandleCommandMessage(const ChatMessage& message);
    void HandleTextMessage(const ChatMessage& message);
    void HandleFileHeaderMessage(const ChatMessage& message);
    void HandleFileDataMessage(const ChatMessage& message);
    void HandleMessage(const ChatMessage& message);
};

#endif
