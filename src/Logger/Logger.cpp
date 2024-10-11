#include "Logger.h"
#include <iostream>

std::vector<LogEntry> Logger::_logs;
std::vector<LogEntry> Logger::_newLogs;

bool Logger::IsDirty() {
    return !_newLogs.empty();
}

const std::vector<LogEntry>& Logger::GetNewLogs() {
    return _newLogs;
}

void Logger::ClearNewLogs() {
    _newLogs.clear();
}

void Logger::Log(const std::string& message) {
    LogEntry entry;
    entry.type = LogType::LOG_TYPE_INFO;
    entry.message = message;
    _logs.push_back(entry);
    _newLogs.push_back(entry);
}

void Logger::Err(const std::string& message) {
    LogEntry entry;
    entry.type = LogType::LOG_TYPE_ERROR;
    entry.message = message;
    _logs.push_back(entry);
    _newLogs.push_back(entry);
}