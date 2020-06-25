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
#include "Sound.h"

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

	inline void Reset()
	{
		for (unsigned int i = 0; i < m_luaSprites.size(); ++i)
		{
			if (m_luaSprites[i] != nullptr)
			delete m_luaSprites[i];
		}
		m_luaSprites.clear();
	}

	Sprite* GetSprite(unsigned int ID);

	bool Update();

	void BeginFrame();
	void EndFrame();

	inline bool GetKeyDown(sf::Keyboard::Key key) { return m_Window.IsFocused() && m_Window.GetKeyDown(key); }
	inline bool GetMouseDown(sf::Mouse::Button b) { return m_Window.IsFocused() && m_Window.GetMouseDown(b); }

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

	inline void DrawText(sf::Text& text) 
	{
		const_cast<sf::Texture&>(text.getFont()->getTexture(30)).setSmooth(false);
		m_Window.SetCamera(m_DebugCamera);
		m_Window.Draw(text); 
		m_Window.SetCamera(m_Camera);
	}
	inline float GetTextCentreX(sf::Text& text)
	{
		return m_Window.GetWidth() / 2.0f - (text.getLocalBounds().width / 2.0f);
	}
	inline float GetTextCentreY(sf::Text& text)
	{
		return m_Window.GetHeight() / 2.0f - (text.getLocalBounds().height / 2.0f);
	}

	inline void LoadFont(sf::Font& font, const std::string& fileName) { font.loadFromFile(RES_PATH + fileName); }

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

	Camera m_DebugCamera;
};

#endif