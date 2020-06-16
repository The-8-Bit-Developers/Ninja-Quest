#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include <lua/lua.hpp>
#include <vector>

#include "Window.h"
#include "Timer.h"
#include "Logger.h"
#include "Sprite.h"
#include "Lua.h"
#include "Camera.h"
#include "Texture.h"

#ifdef DEBUG
#include "Debugger.h"
#endif

// Nice macro for logging
#ifdef DEBUG
#define Log(x) Engine::Get().m_Logger.Log(x)
#else
#define Log(x)
#endif

/*
	The Engine class is the main engine class. It is a singleton.
*/
class Engine
{
public:

	Engine(const Engine&) = delete; // Delete copy constructor
	static Engine& Get() { static Engine s_Instance; return s_Instance; }
	~Engine();

	void Create(const std::string& windowName, const int windowWidth, const int windowHeight);

	Sprite* GetSprite(unsigned int ID);

	bool Update();

	void BeginFrame();
	void EndFrame();

	inline bool GetKeyDown(sf::Keyboard::Key key) { return m_Window.GetKeyDown(key); }

	Window m_Window;
	Timer m_Timer;
	Logger m_Logger;

#ifdef DEBUG
	Debugger m_Debugger;
#endif

	Camera m_Camera;

	void SetGravity(float g);
	float m_Gravity = 10.0f;

	Lua m_Lua;
	static int OnLuaPrint(lua_State* L);
	static int OnLuaError(lua_State* L);

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

private:
	Engine();

	std::vector<Sprite*> m_luaSprites;

#ifdef DEBUG
	Camera m_DebugCamera;
#endif
};

#endif