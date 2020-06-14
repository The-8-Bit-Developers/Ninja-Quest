#include "Lua.h"

Lua::Lua(Logger& _logger, lua_CFunction OnPrint, lua_CFunction OnError) : logger(_logger)
{
	L = luaL_newstate();
	luaL_openlibs(L);

	// Setup custom print function
	const struct luaL_Reg printlib[] = { {"print", OnPrint}, {"error", OnError}, {NULL, NULL} }; // Null indicates end of array
	lua_getglobal(L, "_G");
	luaL_setfuncs(L, printlib, 0);  // for Lua versions 5.2 or greater
	lua_pop(L, 1);
}

void Lua::ExecuteString(const std::string& string)
{
	if (luaL_dostring(L, string.c_str()) != LUA_OK)
		logger.Err("Lua error with line " + string + ": " + std::string(lua_tostring(L, -1)));
}

void Lua::ExecuteFile(const std::string& fileName)
{
	if (luaL_dofile(L, std::string("../res/" + fileName).c_str()) != LUA_OK)
		logger.Err("Lua error with file " + fileName + ": " + std::string(lua_tostring(L, -1)));
}

const std::string Lua::GetGlobalString(const std::string& variable)
{
	if (variable != "-1") lua_getglobal(L, variable.c_str());
	return lua_tostring(L, -1);
}

int Lua::GetGlobalNumber(const std::string& variable)
{
	if (variable != "-1") lua_getglobal(L, variable.c_str());
	return (int) lua_tonumber(L, -1);
}

void Lua::PushNumber(int number)	{ lua_pushnumber(L, number); }
void Lua::PushNumber(float number)	{ lua_pushnumber(L, number); }

void Lua::CallGlobalFunction(const std::string& function, std::function<void()> ProvideArguments, int nProvidedArguments, int nExpectedArguments)
{
	lua_getglobal(L, function.c_str());
	if (lua_isfunction(L, -1))
	{
		ProvideArguments();
		if (lua_pcall(L, nProvidedArguments, nExpectedArguments, 0) != LUA_OK)
			logger.Err("Lua error with calling function " + function + ":" + std::string(lua_tostring(L, -1)));
	}
	else logger.Err("Lua eror - " + function + " is not a valid function!");
}

void Lua::CallGlobalFunction(const std::string& function) { CallGlobalFunction(function, []() {}, 0, 0); }

Lua::~Lua()
{
	lua_close(L);
}