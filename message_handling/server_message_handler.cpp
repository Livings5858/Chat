#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "server_message_handler.h"
#include "logger.h"

std::map<std::string, int> ServerMessageHandler::onlineMap_;

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

    // Generate Fake User List
    std::map<std::string, std::string> usermap;
    std::string username = "test";
    std::string password = "password";
    for (int i = 0; i < 10; i++) {
        usermap[username + std::to_string(i)] = password;
    }

    bool loginOK = false;
    auto it = usermap.find(msg.from);
    if (it != usermap.end()) {
        if (it->second == msg.message) {
            LOGI("Login sucessful %s (ClientSocket: %d)", msg.from.c_str(), clientSocket);
            loginOK = true;
        } else {
            LOGI("Login failed %s (ClientSocket: %d), incorrect password.",
                msg.from.c_str(), clientSocket);
        }
    } else {
        LOGI("Login failed %s (ClientSocket: %d), username not found.",
            msg.from.c_str(), clientSocket);
    }

    if (loginOK) {
        LOGI("Login map add %s : %d", msg.from.c_str(), clientSocket);
        onlineMap_[msg.from] = clientSocket;
    }
    replayMsg.message = loginOK ? "OK" : "Error:Please check username and password";
    SendMessage(clientSocket, replayMsg);
}

void ServerMessageHandler::HandleLogoutMessage(int clientSocket, const ChatMessage& msg) {
    auto it = onlineMap_.find(msg.from);
    if (it != onlineMap_.end()) {
        if (it->second == clientSocket) {
            eraseFromOnlineMap(msg.from);
        } else {
            LOGE("Log out failed, clientSocket [%d/%d] not matched (username: %s)",
                clientSocket, it->second, msg.from.c_str());
        }
    } else {
        LOGE("Log out failed, username [%s] not found", msg.from.c_str());
    }
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
        eraseFromOnlineMap(msg.to);
        return;
    }
    if (send(clientSocket, message.c_str(), message.length(), 0) == -1) {
        LOGE("Send message content failed (ClientSocket: %d)", clientSocket);
        eraseFromOnlineMap(msg.to);
        return;
    }
}

void ServerMessageHandler::eraseFromOnlineMap(std::string username) {
    auto it = onlineMap_.find(username);
    if (it != onlineMap_.end()) {
        onlineMap_.erase(it);
        LOGI("User log out: %s", username.c_str());
    }
}
