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

void Lua::SetGlobalNumber(const std::string& variable, float number)
{
	lua_pushnumber(L, number);
	lua_setglobal(L, variable.c_str());
}

int Lua::GetInt(int id)				{ if (lua_isnoneornil(L, id)) { logger.Err("Nill value!"); return -1; }			return (int)lua_tonumber(L, id); }
float Lua::GetFloat(int id)			{ if (lua_isnoneornil(L, id)) { logger.Err("Nill value!"); return -1.0f; }		return (float)lua_tonumber(L, id); }
std::string Lua::GetString(int id)	{ if (lua_isnoneornil(L, id)) { logger.Err("Nill value!"); return ""; }			return lua_tostring(L, id); }
bool Lua::GetBool(int id)			{ if (lua_isnoneornil(L, id)) { logger.Err("Nill value!"); return false; }		return lua_toboolean(L, id); }

void Lua::PushNumber(int number)	{ lua_pushnumber(L, number); }
void Lua::PushNumber(float number)	{ lua_pushnumber(L, number); }
void Lua::PushString(const std::string& string) { lua_pushstring(L, string.c_str()); }
void Lua::PushBool(bool b) { lua_pushboolean(L, b); }

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

void Lua::SetGlobalFunction(const std::string& name, int(*function)(lua_State*))
{
	lua_register(L, name.c_str(), function);
}

Lua::~Lua()
{
	lua_close(L);
}