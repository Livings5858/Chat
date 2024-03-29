#include <gtest/gtest.h>
#include "tcp_server.h"
#include "tcp_client.h"
#include "chat_message.h"
#include "server_message_handler.h"
#include "client_message_handler.h"

class TCPConnectTest : public ::testing::Test {
protected:
    static TCPServer server_;

    static void SetUpTestSuite() {
        server_.SetCallback(OnRecvMessage);
        if (server_.Start()) {
            std::cout << "test tcp server starting..." << std::endl;
        }
    }

    static void TearDownTestSuite() {
        server_.Stop();
    }

    static void OnRecvMessage(int clientSocket, const std::string& message) {
        ChatMessage msg = DeserializeChatMessage(message);
        ServerMessageHandler handler;
        handler.HandleMessage(clientSocket, msg);
    }
};

// 测试正常情况下建立TCP连接是否成功
TEST_F(TCPConnectTest, ConnectSuccess) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_TRUE(client.Initialize());
}

// 测试连接到无效的服务器地址是否失败
TEST_F(TCPConnectTest, ConnectToInvalidServer) {
    const char *SERVER_IP = "xxxxx";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_FALSE(client.Initialize());
}

// 测试连接到错误的端口是否失败
TEST_F(TCPConnectTest, ConnectToWrongPort) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8081;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_FALSE(client.Initialize());
}

// 测试连接到服务器后连接是否正常发送消息
TEST_F(TCPConnectTest, SendMessageSuccess) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_TRUE(client.Initialize());

    ChatMessage msg {
        .type = MSG_TEXT,
        .from = "test",
        .to = "server",
        .message = "hello world"
    };
    int ret = client.SendMessage(SerializeChatMessage(msg));
    EXPECT_EQ(ret, 0);

    // sleep 1s ：保证最后一条消息被接收之后再关闭client
    sleep(1);
}

// 测试连接到服务器后正常发送消息10次
TEST_F(TCPConnectTest, SendMessage10Success) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_TRUE(client.Initialize());

    for (int i = 0; i < 10; i++) {
        ChatMessage msg {
            .type = MSG_TEXT,
            .from = "test",
            .to = "server",
            .message = "hello world "+std::to_string(i+1)
        };
        int ret = client.SendMessage(SerializeChatMessage(msg));
        EXPECT_EQ(ret, 0);
    }

    // sleep 1s ：保证最后一条消息被接收之后再关闭client
    sleep(1);
}

// 测试服务端处理命令消息
TEST_F(TCPConnectTest, TestCommandMessageHandling) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_TRUE(client.Initialize());

    ChatMessage msg {
        .type = MSG_COMMAND,
        .from = "test",
        .to = "server",
        .message = "LOGIN"
    };
    int ret = client.SendMessage(SerializeChatMessage(msg));
    EXPECT_EQ(ret, 0);

    sleep(1);
}

// 测试服务端处理文本消息
TEST_F(TCPConnectTest, TestTextMessageHandling) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_TRUE(client.Initialize());

    ChatMessage msg {
        .type = MSG_TEXT,
        .from = "test",
        .to = "server",
        .message = "hello world"
    };
    int ret = client.SendMessage(SerializeChatMessage(msg));
    EXPECT_EQ(ret, 0);

    sleep(1);
}

// 测试服务端处理文件头消息
TEST_F(TCPConnectTest, TestFileHeaderMessageHandling) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_TRUE(client.Initialize());

    ChatMessage msg {
        .type = MSG_FILEHEADER,
        .from = "test",
        .to = "server",
        .message = "test.jpg:128"
    };
    int ret = client.SendMessage(SerializeChatMessage(msg));
    EXPECT_EQ(ret, 0);

    sleep(1);
}

// 测试服务端处理文件主体消息
TEST_F(TCPConnectTest, TestFileDataMessageHandling) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_TRUE(client.Initialize());

    ChatMessage msg {
        .type = MSG_FILEDATA,
        .from = "test",
        .to = "server",
        .message = "1234561wedwndjahsgaygyagsdyag"
    };
    int ret = client.SendMessage(SerializeChatMessage(msg));
    EXPECT_EQ(ret, 0);

    sleep(1);
}

// 测试服务端处理其他未定义消息
TEST_F(TCPConnectTest, TestUnsupportedMessageHandling) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_TRUE(client.Initialize());

    ChatMessage msg {
        .type = MSG_MAX,
        .from = "test",
        .to = "server",
        .message = "unsupport message"
    };
    int ret = client.SendMessage(SerializeChatMessage(msg));
    EXPECT_EQ(ret, 0);

    sleep(1);
}

// 测试登录
TEST_F(TCPConnectTest, TestLogin) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    TCPClient client(SERVER_IP, SERVER_PORT);
    ASSERT_TRUE(client.Initialize());

    ChatMessage msg {
        .type = MSG_LOGIN,
        .from = "test0",
        .to = "server",
        .message = "password"
    };
    int ret = client.SendMessage(SerializeChatMessage(msg));
    EXPECT_EQ(ret, 0);
    sleep(1);
    msg.type = MSG_LOGOUT;
    ret = client.SendMessage(SerializeChatMessage(msg));
    EXPECT_EQ(ret, 0);
    sleep(1);
}

// 测试10个用户同时登录
TEST_F(TCPConnectTest, TestLogin10User) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    std::vector<std::thread> threads;
    const int numUsers = 10;

    for (int i = 0; i < numUsers; ++i) {
        threads.emplace_back([i, SERVER_IP, SERVER_PORT] {
            TCPClient client(SERVER_IP, SERVER_PORT);
            ASSERT_TRUE(client.Initialize());
            std::string username = "test" + std::to_string(i);

            ChatMessage msg {
                .type = MSG_LOGIN,
                .from = username,
                .to = "server",
                .message = "password"
            };
            int ret = client.SendMessage(SerializeChatMessage(msg));
            EXPECT_EQ(ret, 0);

            // Sleep for some time to allow the user to login
            std::this_thread::sleep_for(std::chrono::seconds(1));

            msg.type = MSG_LOGOUT;
            ret = client.SendMessage(SerializeChatMessage(msg));
            EXPECT_EQ(ret, 0);
            // Sleep for some time to allow the user to logout
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

// 测试10个用户相互发消息
TEST_F(TCPConnectTest, Test10UserChat) {
    const char *SERVER_IP = "127.0.0.1";
    const int SERVER_PORT = 8080;
    std::vector<std::thread> threads;
    const int numUsers = 10;

    for (int i = 0; i < numUsers; ++i) {
        threads.emplace_back([i, SERVER_IP, SERVER_PORT] {
            TCPClient client(SERVER_IP, SERVER_PORT);
            ASSERT_TRUE(client.Initialize());
            std::string username = "test" + std::to_string(i);
            ChatMessage msg {
                .type = MSG_LOGIN,
                .from = username,
                .to = "server",
                .message = "password"
            };
            int ret = client.SendMessage(SerializeChatMessage(msg));
            EXPECT_EQ(ret, 0);

            // wait for others log in
            std::this_thread::sleep_for(std::chrono::seconds(1));

            std::string fromUser = username;
            std::string toUser = "test" + std::to_string((i + 1) % numUsers);
            std::string message = "Hello " + toUser + ", I am " + fromUser + ".";
            ChatMessage textmsg {
                .type = MSG_TEXT,
                .from = fromUser,
                .to = toUser,
                .message = message
            };
            ret = client.SendMessage(SerializeChatMessage(textmsg));
            EXPECT_EQ(ret, 0);

            // Sleep for some time to allow the others to chat
            std::this_thread::sleep_for(std::chrono::seconds(1));

            msg.type = MSG_LOGOUT;
            ret = client.SendMessage(SerializeChatMessage(msg));
            EXPECT_EQ(ret, 0);

            // Sleep for some time to allow the user to logout
            std::this_thread::sleep_for(std::chrono::seconds(1));
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }
}

TCPServer TCPConnectTest::server_;

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
