#include <iostream>
#include <string>
#include <sstream>
#include "chat_message.h"

// 将 ChatMessage 结构体序列化为字符串
std::string SerializeChatMessage(const ChatMessage& msg) {
    std::ostringstream oss;
    oss << "MessageType:" << msg.type << "\n";
    oss << "From:" << msg.from << "\n";
    oss << "To:" << msg.to << "\n";
    oss << "Message:" << msg.message << "\n";
    return oss.str();
}

// 从字符串反序列化为 ChatMessage 结构体
ChatMessage DeserializeChatMessage(const std::string& str) {
    ChatMessage msg;
    std::istringstream iss(str);
    std::string line;
    while (std::getline(iss, line)) {
        size_t colonPos = line.find(":");
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            if (key == "MessageType") {
                msg.type = static_cast<MessageType>(std::stoi(value));
            } else if (key == "From") {
                msg.from = value;
            } else if (key == "To") {
                msg.to = value;
            } else if (key == "Message") {
                msg.message = value;
            }
        }
    }
    return msg;
}
