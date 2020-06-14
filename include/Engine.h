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

	Lua m_Lua;
	static int OnLuaPrint(lua_State* L);
	static int OnLuaError(lua_State* L);

	std::vector<std::reference_wrapper<Sprite>> m_Sprites;

private:
	Engine();
};

#endif