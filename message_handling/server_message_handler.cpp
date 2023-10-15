#include "server_message_handler.h"
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void ServerMessageHandler::HandleCommandMessage(int clientSocket, const ChatMessage& msg) {
    std::cout << "Received command msg from " << msg.from 
              << " to " << msg.to << " (ClientSocket: " << clientSocket << "): " 
              << msg.message << std::endl;
    // 在这里执行处理命令消息的逻辑
}

void ServerMessageHandler::HandleTextMessage(int clientSocket, const ChatMessage& msg) {
    std::cout << "Received text msg from " << msg.from 
              << " to " << msg.to << " (ClientSocket: " << clientSocket << "): " 
              << msg.message << std::endl;
    // 在这里执行处理文本消息的逻辑
}

void ServerMessageHandler::HandleFileHeaderMessage(int clientSocket, const ChatMessage& msg) {
    std::cout << "Received file header msg from " << msg.from 
              << " to " << msg.to << " (ClientSocket: " << clientSocket << "): " 
              << msg.message << std::endl;
    // 在这里执行处理文件名消息的逻辑
}

void ServerMessageHandler::HandleFileDataMessage(int clientSocket, const ChatMessage& msg) {
    std::cout << "Received file data msg from " << msg.from 
              << " to " << msg.to << " (ClientSocket: " << clientSocket << "): " 
              << msg.message << std::endl;
    // 在这里执行处理文件数据消息的逻辑
}

void ServerMessageHandler::HandleLoginMessage(int clientSocket, const ChatMessage& msg) {
    std::cout << "Received login msg from " << msg.from 
              << " (ClientSocket: " << clientSocket << "): " << msg.message << std::endl;
    // 在这里执行处理登录消息的逻辑
    ChatMessage replayMsg = {
        .type = MSG_LOGIN,
        .from = "server",
        .to = msg.from,
    };
    if (msg.from == "username" && msg.message == "password") {
        std::cout << "登录成功" << std::endl;
        replayMsg.message = "OK";
    } else {
        std::cout << "账号或密码错误" << std::endl;
        replayMsg.message = "Error:Please check username and password";
    }
    SendMessage(clientSocket, replayMsg);
}

void ServerMessageHandler::HandleLogoutMessage(int clientSocket, const ChatMessage& msg) {
    std::cout << "Received logout msg from " << msg.from 
              << " (ClientSocket: " << clientSocket << "): " << msg.message << std::endl;
    // 在这里执行处理登出消息的逻辑
}

void ServerMessageHandler::HandleRegisterMessage(int clientSocket, const ChatMessage& msg) {
    std::cout << "Received register msg from " << msg.from 
              << " (ClientSocket: " << clientSocket << "): " << msg.message << std::endl;
    // 在这里执行处理注册消息的逻辑
}

void ServerMessageHandler::HandleUnRegisterMessage(int clientSocket, const ChatMessage& msg) {
    std::cout << "Received unregister msg from " << msg.from 
              << " (ClientSocket: " << clientSocket << "): " << msg.message << std::endl;
    // 在这里执行处理注销消息的逻辑
}

void ServerMessageHandler::HandleForgotPasswordMessage(int clientSocket, const ChatMessage& msg) {
    std::cout << "Received forgot password msg from " << msg.from 
              << " (ClientSocket: " << clientSocket << "): " << msg.message << std::endl;
    // 在这里执行处理忘记密码消息的逻辑
}

void ServerMessageHandler::HandleMessage(int clientSocket, const ChatMessage& msg) {
    switch (msg.type) {
        case MessageType::MSG_COMMAND:
            HandleCommandMessage(clientSocket, msg);
            break;
        case MessageType::MSG_TEXT:
            HandleTextMessage(clientSocket, msg);
            break;
        case MessageType::MSG_FILEHEADER:
            HandleFileHeaderMessage(clientSocket, msg);
            break;
        case MessageType::MSG_FILEDATA:
            HandleFileDataMessage(clientSocket, msg);
            break;
        case MessageType::MSG_LOGIN:
            HandleLoginMessage(clientSocket, msg);
            break;
        case MessageType::MSG_LOGOUT:
            HandleLogoutMessage(clientSocket, msg);
            break;
        case MessageType::MSG_REGISTER:
            HandleRegisterMessage(clientSocket, msg);
            break;
        case MessageType::MSG_UNREGISTER:
            HandleUnRegisterMessage(clientSocket, msg);
            break;
        case MessageType::MSG_FORGOT_PASSWORD:
            HandleForgotPasswordMessage(clientSocket, msg);
            break;
        default:
            std::cout << "Received unsupported msg type" << std::endl;
            break;
    }
}

void ServerMessageHandler::SendMessage(int clientSocket, const ChatMessage& msg) {
    std::string message = SerializeChatMessage(msg);
    size_t message_len = message.length();
    if (message_len == 0) {
        return;
    }
    if (send(clientSocket, &message_len, sizeof(message_len), 0) == -1) {
        perror("发送消息长度失败");
        return;
    }
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
        perror("发送消息内容失败");
        return;
    }
}