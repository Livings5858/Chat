#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "server_message_handler.h"
#include "logger.h"

void ServerMessageHandler::HandleCommandMessage(int clientSocket, const ChatMessage& msg) {

}

void ServerMessageHandler::HandleTextMessage(int clientSocket, const ChatMessage& msg) {

}

void ServerMessageHandler::HandleFileHeaderMessage(int clientSocket, const ChatMessage& msg) {

}

void ServerMessageHandler::HandleFileDataMessage(int clientSocket, const ChatMessage& msg) {

}

void ServerMessageHandler::HandleLoginMessage(int clientSocket, const ChatMessage& msg) {
    ChatMessage replayMsg = {
        .type = MSG_LOGIN,
        .from = "server",
        .to = msg.from,
    };
    if (msg.from == "username" && msg.message == "password") {
        LOGI("Login sucessful %s (ClientSocket: %d)", msg.from.c_str(), clientSocket);
        replayMsg.message = "OK";
    } else {
        LOGI("Login failed %s (ClientSocket: %d)", msg.from.c_str(), clientSocket);
        replayMsg.message = "Error:Please check username and password";
    }
    SendMessage(clientSocket, replayMsg);
}

void ServerMessageHandler::HandleLogoutMessage(int clientSocket, const ChatMessage& msg) {

}

void ServerMessageHandler::HandleRegisterMessage(int clientSocket, const ChatMessage& msg) {

}

void ServerMessageHandler::HandleUnRegisterMessage(int clientSocket, const ChatMessage& msg) {

}

void ServerMessageHandler::HandleForgotPasswordMessage(int clientSocket, const ChatMessage& msg) {

}

void ServerMessageHandler::HandleMessage(int clientSocket, const ChatMessage& msg) {
    LOGI("Received msg[%d] from %s to %s (ClientSocket: %d): %s",
        msg.type, msg.from.c_str(), msg.to.c_str(), clientSocket, msg.message.c_str());
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
            LOGE("Received unsupported msg type");
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
        LOGE("Send message length failed (ClientSocket: %d)", clientSocket);
        return;
    }
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
        LOGE("Send message content failed (ClientSocket: %d)", clientSocket);
        return;
    }
}