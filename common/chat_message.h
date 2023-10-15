#ifndef CHAT_MESSAGE_H
#define CHAT_MESSAGE_H

#include <iostream>
#include <string>
#include <sstream>

enum MessageType {
    MSG_COMMAND,
    MSG_TEXT,
    MSG_FILEHEADER,
    MSG_FILEDATA,
    MSG_LOGIN,
    MSG_LOGOUT,
    MSG_REGISTER,
    MSG_UNREGISTER,
    MSG_FORGOT_PASSWORD,
    MSG_MAX,
};

struct ChatMessage {
    MessageType type;
    std::string from;
    std::string to;
    std::string message;
};

// 将 ChatMessage 结构体序列化为字符串
std::string SerializeChatMessage(const ChatMessage& msg);

// 从字符串反序列化为 ChatMessage 结构体
ChatMessage DeserializeChatMessage(const std::string& str);

#endif // CHAT_MESSAGE_H