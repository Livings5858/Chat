#include "client_message_handler.h"
#include "logger.h"

void ClientMessageHandler::HandleCommandResponse(const ChatMessage& msg) {
    // 实现处理命令回复消息的逻辑
}

void ClientMessageHandler::HandleTextResponse(const ChatMessage& msg) {
    LOGD("Received text message from %s ,message: %s", msg.from.c_str(), msg.message.c_str());
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
        LOGD("Login sucessful %s", msg.to.c_str());
        std::cout << "Login sucessful" << std::endl;
    } else {
        LOGE("Login failed %s, %s", msg.to.c_str(), msg.message.c_str());
        std::cout << "Login failed: " << msg.message << std::endl;
    }
}

void ClientMessageHandler::HandleLogoutResponse(const ChatMessage& msg) {
    if (msg.message == "OK") {
        LOGD("Logout sucessful %s", msg.to.c_str());
    } else {
        LOGE("Logout failed %s, %s", msg.to.c_str(), msg.message.c_str());
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
            LOGE("Received an unsupported message type");
            break;
    }
}

