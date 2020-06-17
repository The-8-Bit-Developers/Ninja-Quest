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
	void OnUpdate() override;
	
	static LuaComponent* s_CurrentInstance;

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

private:
	Lua m_Lua;
	
};

#endif