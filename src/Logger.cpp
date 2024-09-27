#include "Logger.h"
#include <iostream>

std::vector<LogEntry> Logger::_logs;

void Logger::Log(const std::string& message) {
    LogEntry entry;
    entry.type = LogType::LOG_TYPE_INFO;
    entry.message = message;
    _logs.push_back(entry);

    std::cout << "\033[1;32m" << message << "\033[0m" << std::endl;
}

void Logger::Err(const std::string& message) {
    LogEntry entry;
    entry.type = LogType::LOG_TYPE_ERROR;
    entry.message = message;
    _logs.push_back(entry);

    std::cout << "\033[1;31m" << message << "\033[0m" << std::endl;
}