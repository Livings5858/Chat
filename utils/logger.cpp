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

        *outputStream << buffer << std::endl;
    }
}
