#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdarg>
#include <chrono>
#include <thread>
#include <sys/syscall.h>
#include <unistd.h>
#include <ctime>
#include <mutex>

enum class LogLevel {
    VERBOSE,
    DEBUG,
    WARNING,
    INFO,
    ERROR
};

class Logger {
public:
    static Logger& GetInstance();

    void SetLogLevel(LogLevel level);

    void Log(LogLevel level, const char* format, ...);

    static std::string GetCurrentTimestamp();
private:
    Logger();

    LogLevel logLevel = LogLevel::INFO;
    std::ostream* outputStream;
    std::ofstream logFile;
    std::mutex logMutex;
    std::string logFileName = "chat.log";
};

#define LOG_LEVEL_STR(level) \
    (level == LogLevel::VERBOSE ? "V" : \
     level == LogLevel::DEBUG ? "D" : \
     level == LogLevel::WARNING ? "W" : \
     level == LogLevel::INFO ? "I" : \
     level == LogLevel::ERROR ? "E" : "U")

#define GETTID() syscall(SYS_gettid)
#define GETPID() getpid()
#define LOG(level, format, ...) \
    Logger::GetInstance().Log(level, "%s %6d %6d [%s] %s:%d %s() " \
    format, Logger::GetCurrentTimestamp().c_str(), GETPID(), GETTID(), LOG_LEVEL_STR(level), \
    __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__)


#define LOGV(format, ...) LOG(LogLevel::VERBOSE, format, ##__VA_ARGS__)
#define LOGD(format, ...) LOG(LogLevel::DEBUG, format, ##__VA_ARGS__)
#define LOGI(format, ...) LOG(LogLevel::INFO, format, ##__VA_ARGS__)
#define LOGW(format, ...) LOG(LogLevel::WARNING, format, ##__VA_ARGS__)
#define LOGE(format, ...) LOG(LogLevel::ERROR, format, ##__VA_ARGS__)

#endif // LOGGER_H
