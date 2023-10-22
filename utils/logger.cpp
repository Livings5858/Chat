#include "logger.h"

Logger::Logger() {
    if (logFileName.empty()) {
        outputStream = &std::cout;
    } else {
        logFile.open(logFileName);
        if (logFile.is_open()) {
            outputStream = &logFile;
        } else {
            outputStream = &std::cout;
        }
    }
}

Logger& Logger::GetInstance() {
    static Logger instance;
    return instance;
}

void Logger::SetLogLevel(LogLevel level) {
    logLevel = level;
}

void Logger::Log(LogLevel level, const char* format, ...) {
    if (level >= logLevel) {
        va_list args;
        va_start(args, format);

        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), format, args);

        va_end(args);
        std::lock_guard<std::mutex> lock(logMutex);
        *outputStream << buffer << std::endl;
    }
}

std::string Logger::GetCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();

    time_t t = std::chrono::system_clock::to_time_t(now);
    tm tm_info = *std::localtime(&t);

    char buffer[80];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm_info);

    char timestamp[100];
    snprintf(timestamp, sizeof(timestamp), "%s.%03ld", buffer, ms % 1000);

    return timestamp;
}