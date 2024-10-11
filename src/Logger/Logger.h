#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <stdio.h>
#include <chrono>
#include <ctime>

enum LogType {
    LOG_TYPE_INFO,
    LOG_TYPE_ERROR
};

struct LogEntry {
    LogType type;
    std::string message;
};

class Logger {
private:
    static std::vector<LogEntry> _logs;
    static std::vector<LogEntry> _newLogs;
    
public:
    static void Log(const std::string& message);
    static void Err(const std::string& message);

    static bool IsDirty();
    static const std::vector<LogEntry>& GetNewLogs();
    static void ClearNewLogs();

    static const std::vector<LogEntry>& GetLogs();
};

#define LOG(fmt, ...) \
    { \
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); \
        std::string dateStr(30, 0); \
        std::strftime(&dateStr[0], dateStr.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now)); \
        char buffer[256]; \
        snprintf(buffer, sizeof(buffer), "%s [%s: %d][%s][Info] " fmt, \
        dateStr.c_str(), __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        Logger::Log(buffer); \
    }

#define LOG_ERR(fmt, ...) \
    { \
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); \
        std::string dateStr(30, 0); \
        std::strftime(&dateStr[0], dateStr.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now)); \
        char buffer[256]; \
        snprintf(buffer, sizeof(buffer), "[%s: %d][%s][Error] " fmt, \
        __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
        Logger::Err(buffer); \
    }

#endif