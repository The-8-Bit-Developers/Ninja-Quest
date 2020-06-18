#include "Logger.h"

Logger::Logger() {}

void Logger::Log(const std::string& string) { m_Messages.push_back(string); }
void Logger::Err(const std::string& string) { m_Messages.push_back(string); }

void Logger::Log(const int number) { m_Messages.push_back(std::to_string(number)); }

Logger::~Logger() {}