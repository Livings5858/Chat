#include "client_message_handler.h"

void ClientMessageHandler::HandleCommandResponse(const ChatMessage& msg) {
    // 实现处理命令回复消息的逻辑
}

void ClientMessageHandler::HandleTextResponse(const ChatMessage& msg) {
    std::cout << "Received text message from " << msg.from << ": " << msg.message << std::endl;
    // 实现处理文本回复消息的逻辑
}

void ClientMessageHandler::HandleFileHeaderResponse(const ChatMessage& msg) {
    // 实现处理文件头回复消息的逻辑
}

void ClientMessageHandler::HandleFileDataResponse(const ChatMessage& msg) {
    // 实现处理文件数据回复消息的逻辑
}

void ClientMessageHandler::HandleLoginResponse(const ChatMessage& msg) {
    if (msg.message == "OK") {
        std::cout << "登录成功" << std::endl;
    } else {
        std::cout << "登录失败: " << msg.message << std::endl;
    }
}

void ClientMessageHandler::HandleLogoutResponse(const ChatMessage& msg) {
    if (msg.message == "OK") {
        std::cout << "Logout成功" << std::endl;
    } else {
        std::cout << "Logout失败: " << msg.message << std::endl;
    }
}

void ClientMessageHandler::HandleRegisterResponse(const ChatMessage& msg) {
    // 实现处理注册回复消息的逻辑
}

void ClientMessageHandler::HandleUnRegisterResponse(const ChatMessage& msg) {
    // 实现处理注销注册回复消息的逻辑
}

void ClientMessageHandler::HandleForgotPasswordResponse(const ChatMessage& msg) {
    // 实现处理忘记密码回复消息的逻辑
}

void ClientMessageHandler::HandleMessage(const ChatMessage& msg) {
    switch (msg.type) {
        case MessageType::MSG_COMMAND:
            HandleCommandResponse(msg);
            break;
        case MessageType::MSG_TEXT:
            HandleTextResponse(msg);
            break;
        case MessageType::MSG_FILEHEADER:
            HandleFileHeaderResponse(msg);
            break;
        case MessageType::MSG_FILEDATA:
            HandleFileDataResponse(msg);
            break;
        case MessageType::MSG_LOGIN:
            HandleLoginResponse(msg);
            break;
        case MessageType::MSG_LOGOUT:
            HandleLogoutResponse(msg);
            break;
        case MessageType::MSG_REGISTER:
            HandleRegisterResponse(msg);
            break;
        case MessageType::MSG_UNREGISTER:
            HandleUnRegisterResponse(msg);
            break;
        case MessageType::MSG_FORGOT_PASSWORD:
            HandleForgotPasswordResponse(msg);
            break;
        default:
            std::cout << "Received an unsupported message type." << std::endl;
            break;
    }
}

