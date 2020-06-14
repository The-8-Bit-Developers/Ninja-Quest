#include "Logger.h"

Logger::Logger() {}

void Logger::Log(const std::string& string) { m_Messages.push_back(string); }
void Logger::Err(const std::string& string) { m_Messages.push_back(string); }

Logger::~Logger() {}