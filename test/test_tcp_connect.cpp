#include <gtest/gtest.h>
#include "tcp_server.h"
#include "tcp_client.h"
#include "chat_message.h"

class TCPConnectTest : public ::testing::Test {
protected:
    static TCPServer server;

    static void SetUpTestSuite() {
        if (server.Start()) {
            std::cout << "test tcp server starting..." << std::endl;
        }
    }

    static void TearDownTestSuite() {
        server.Stop();
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

    ChatMessage msg{
        .type = MSG_TEXT,
        .from = "test",
        .to = "server",
        .message = "hello world"
    };
    int ret = client.SendMessage(SerializeChatMessage(msg));
    EXPECT_EQ(ret, 0);
}

TCPServer TCPConnectTest::server;

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
