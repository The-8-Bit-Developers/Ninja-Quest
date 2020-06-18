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

	inline bool GetKeyDown(sf::Keyboard::Key key) { return m_Window.IsFocused() && m_Window.GetKeyDown(key); }

	Window m_Window;
	Timer m_Timer;
	Logger m_Logger;

#ifdef DEBUG
	Debugger m_Debugger;
#endif

	Camera m_Camera;

	void SetGravity(float g);
	float m_Gravity = 10.0f;

	static int OnLuaPrint(lua_State* L);
	static int OnLuaError(lua_State* L);
	std::vector<Sprite*> m_luaSprites;

	inline void SetBackgroundColour(float r, float g, float b)
	{
		m_BackgroundR = r;
		m_BackgroundG = g;
		m_BackgroundB = b;
	}

private:
	Engine();

	float m_BackgroundR = 0.0f;
	float m_BackgroundG = 0.0f;
	float m_BackgroundB = 0.0f;

#ifdef DEBUG
	Camera m_DebugCamera;
#endif
};

#endif