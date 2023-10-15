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
private:
    Logger();

    LogLevel logLevel = LogLevel::INFO;
    std::ostream* outputStream;
    std::ofstream logFile;
    std::string logFileName = "log.txt";
};

#define LOG_LEVEL_STR(level) \
    (level == LogLevel::VERBOSE ? "VERBOSE" : \
     level == LogLevel::DEBUG ? "DEBUG" : \
     level == LogLevel::WARNING ? "WARNING" : \
     level == LogLevel::INFO ? "INFO" : \
     level == LogLevel::ERROR ? "ERROR" : "UNKNOWN")

#define GETTID() syscall(SYS_gettid)
#define GETPID() getpid()
#define LOG(level, format, ...) \
    Logger::GetInstance().Log(level, "%s %d %d [%s] %s:%d " \
    format, __TIME__, GETPID(), GETTID(), LOG_LEVEL_STR(level), \
    __FUNCTION__, __LINE__, ##__VA_ARGS__)


#define LOGV(format, ...) LOG(LogLevel::VERBOSE, format, ##__VA_ARGS__)
#define LOGD(format, ...) LOG(LogLevel::DEBUG, format, ##__VA_ARGS__)
#define LOGI(format, ...) LOG(LogLevel::INFO, format, ##__VA_ARGS__)
#define LOGW(format, ...) LOG(LogLevel::WARNING, format, ##__VA_ARGS__)
#define LOGE(format, ...) LOG(LogLevel::ERROR, format, ##__VA_ARGS__)

#endif // LOGGER_H
