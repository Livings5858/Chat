#include "message_handler.h"

void MessageHandler::HandleCommandMessage(const ChatMessage& message) {
    std::cout << "Received command message from " << message.from << " to " << message.to << ": " << message.message << std::endl;
    // 在这里执行处理文本消息的逻辑
}

void MessageHandler::HandleTextMessage(const ChatMessage& message) {
    std::cout << "Received text message from " << message.from << " to " << message.to << ": " << message.message << std::endl;
    // 在这里执行处理文本消息的逻辑
}

void MessageHandler::HandleFileHeaderMessage(const ChatMessage& message) {
    std::cout << "Received file header message from " << message.from << " to " << message.to << ": " << message.message << std::endl;
    // 在这里执行处理文件名消息的逻辑
}


void MessageHandler::HandleFileDataMessage(const ChatMessage& message) {
    std::cout << "Received file data message from " << message.from << " to " << message.to << ": " << message.message << std::endl;
    // 在这里执行处理文件数据消息的逻辑
}

void MessageHandler::HandleMessage(const ChatMessage& message) {
    switch (message.type) {
        case MessageType::MSG_COMMAND:
            HandleCommandMessage(message);
            break;
        case MessageType::MSG_TEXT:
            HandleTextMessage(message);
            break;
        case MessageType::MSG_FILEHEADER:
            HandleFileHeaderMessage(message);
            break;
        case MessageType::MSG_FILEDATA:
            HandleFileDataMessage(message);
            break;
        default:
            std::cout << "Received unsupported message type" << std::endl;
            break;
    }
}
