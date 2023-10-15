#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg>

// 日志级别
enum class LogLevel {
    VERBOSE,
    DEBUG,
    WARNING,
    INFO,
    ERROR
};

class Logger {
public:
    // 获取日志单例
    static Logger& GetInstance();

    // 设置日志级别
    void SetLogLevel(LogLevel level);

    // 记录日志
    void Log(LogLevel level, const char* format, ...);

private:
    Logger();

    // 日志级别字符串映射
    const char* LogLevelStr[5] = {
        "VERBOSE",
        "DEBUG",
        "WARNING",
        "INFO",
        "ERROR"
    };

    LogLevel logLevel = LogLevel::INFO;
    std::ostream* outputStream;
    std::ofstream logFile;
    std::string logFileName = "log.txt"; // 可以根据需要更改日志文件名
};

#define LOG(level, format, ...) Logger::GetInstance().Log(level, format, ##__VA_ARGS__)

#define LOGV(format, ...) LOG(LogLevel::VERBOSE, format, ##__VA_ARGS__)
#define LOGD(format, ...) LOG(LogLevel::DEBUG, format, ##__VA_ARGS__)
#define LOGI(format, ...) LOG(LogLevel::INFO, format, ##__VA_ARGS__)
#define LOGW(format, ...) LOG(LogLevel::WARNING, format, ##__VA_ARGS__)
#define LOGE(format, ...) LOG(LogLevel::ERROR, format, ##__VA_ARGS__)

#endif // LOGGER_H
