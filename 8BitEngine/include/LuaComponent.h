#pragma once
#ifndef LUA_COMPONENT_H
#define LUA_COMPONENT_H

#include <string>

#include "Component.h"
#include "Lua.h"
#include "Logger.h"
#include "Raycast.h"

class LuaComponent : public Component
{
	
public:
	LuaComponent(const std::string& fileName);
	LuaComponent(const std::string& fileName, const std::string& variable, int number);
	~LuaComponent() override {}
	void OnUpdate() override;
	
	void RegisterFunctions();

	static LuaComponent* s_CurrentInstance;

	struct CustomFunction
	{
		CustomFunction()
		{
			name = "null";
			function = 0;
		}

		CustomFunction(const std::string _name, int* _function)
		{
			name = _name;
			function = _function;
		}

		std::string name;
		int* function;
	};
	static CustomFunction s_CustomFunction;

	// Lua functions
	static int lua_CreateSprite(lua_State* L);
	static int lua_GetKeyDown(lua_State* L);
	static int lua_AddX(lua_State* L);
	static int lua_AddY(lua_State* L);
	static int lua_GetX(lua_State* L);
	static int lua_GetY(lua_State* L);
	static int lua_SetX(lua_State* L);
	static int lua_SetY(lua_State* L);
	static int lua_SetGravity(lua_State* L);
	static int lua_GetGravity(lua_State* L);
	static int lua_AddPhysics(lua_State* L);
	static int lua_AddSpherePhysics(lua_State* L);
	static int lua_RemovePhysics(lua_State* L);
	static int lua_GetPhysics(lua_State* L);
	static int lua_AddForce(lua_State* L);
	static int lua_SetDensity(lua_State* L);
	static int lua_GetDensity(lua_State* L);
	static int lua_SetVelocityX(lua_State* L);
	static int lua_GetVelocityX(lua_State* L);
	static int lua_SetVelocityY(lua_State* L);
	static int lua_GetVelocityY(lua_State* L);
	static int lua_SetLayer(lua_State* L);
	static int lua_GetLayer(lua_State* L);
	static int lua_LoadTexture(lua_State* L);
	static int lua_SetTexture(lua_State* L);
	static int lua_GetTexture(lua_State* L);
	static int lua_SetFriction(lua_State* L);
	static int lua_GetFriction(lua_State* L);
	static int lua_RayCast(lua_State* L);
	static int lua_Delete(lua_State* L);
	static int lua_AddScript(lua_State* L);
	static int lua_GetMouseDown(lua_State* L);
	static int lua_GetMouseX(lua_State* L);
	static int lua_GetMouseY(lua_State* L);
	static int lua_SetTrigger(lua_State* L);
	static int lua_IsTriggered(lua_State* L);
	static int lua_SetBodyData(lua_State* L);
	static int lua_GetTriggeredData(lua_State* L);

	Lua m_Lua;
	
};

#endif