#include "logger.h"

Logger::Logger() {
    if (logFileName.empty()) {
        // 输出到控制台
        outputStream = &std::cout;
    } else {
        // 输出到文件
        logFile.open(logFileName);
        if (logFile.is_open()) {
            outputStream = &logFile;
        } else {
            // 文件打开失败，回退到控制台
            outputStream = &std::cout;
        }
    }
}

Logger& Logger::GetInstance() {
    static Logger instance; // 延迟初始化
    return instance;
}

void Logger::SetLogLevel(LogLevel level) {
    logLevel = level;
}

void Logger::Log(LogLevel level, const char* format, ...) {
    if (level >= logLevel) {
        va_list args;
        va_start(args, format);

        char buffer[1024]; // 适当选择足够大的缓冲区
        vsnprintf(buffer, sizeof(buffer), format, args);

        va_end(args);

        // 输出到流（控制台或文件）
        *outputStream << LogLevelStr[static_cast<int>(level)] << ": " << buffer << std::endl;
    }
}
