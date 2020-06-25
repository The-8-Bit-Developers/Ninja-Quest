#pragma once
#ifndef LUA_H
#define LUA_H

#include <lua/lua.hpp>
#include <string>
#include <functional>

#include "Logger.h"

/*
	The Lua class serves as an iterface between Lua and the engine
*/
class Lua
{
public:
	Lua(Logger& _logger, lua_CFunction OnPrint, lua_CFunction OnError);
	~Lua();

	void ExecuteString(const std::string& string);
	void ExecuteFile(const std::string& fileName);

	const std::string GetGlobalString(const std::string& variable = "-1");
	int GetGlobalNumber(const std::string& variable = "-1");
	bool GetGlobalBool(const std::string& variable = "-1");

	void SetGlobalNumber(const std::string& variable, float number);
	void SetGlobalBool(const std::string& variable, bool b);

	int GetInt(int id);
	float GetFloat(int id);
	std::string GetString(int id);
	bool GetBool(int id);

	void PushNumber(int number);
	void PushNumber(float number);
	void PushString(const std::string& string);
	void PushBool(bool b);

	void CallGlobalFunction(const std::string& function, std::function<void()> ProvideArguments, int nProvidedArguments, int nExpectedArguments);
	void CallGlobalFunction(const std::string& function);

	void SetGlobalFunction(const std::string& name, int(*function)(lua_State*));

private:
	lua_State* L;
	Logger& logger;
};

#endif