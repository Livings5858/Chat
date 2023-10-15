#include "logger.h"
#include "gtest/gtest.h"

class LoggerTest : public testing::Test {
protected:
    void SetUp() override {
        // 在测试开始前设置日志级别为 DEBUG
        Logger::GetInstance().SetLogLevel(LogLevel::DEBUG);
    }

    void TearDown() override {
        // 在测试结束后重置日志级别为 INFO
        Logger::GetInstance().SetLogLevel(LogLevel::INFO);
    }
};

TEST_F(LoggerTest, LogMessagesTest) {
    // 获取日志单例
    Logger& logger = Logger::GetInstance();

    // 设置日志级别为 DEBUG
    logger.SetLogLevel(LogLevel::DEBUG);

    int num = 12;
    char str[] = "Hello world";

    // 用 LOGE 和 LOGI 宏记录日志
    LOGE("This is an error message: %s %d", str, num);
    LOGI("This is an informational message: %s %d", str, num);
    LOGW("This is an informational message: %s %d", str, num);
    LOGD("This is an informational message: %s %d", str, num);
    LOGV("This is an informational message: %s %d", str, num);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
