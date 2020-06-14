#pragma once
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <vector>

/*
	The Logger class keeps track of logging
*/
class Logger
{
public:
	Logger();
	~Logger();

	void Log(const std::string& string);
	void Err(const std::string& string);

	std::vector<std::string> m_Messages;
};

#endif