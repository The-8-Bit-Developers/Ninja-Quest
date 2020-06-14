#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "Window.h"
#include "Timer.h"
#include "Logger.h"

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

	Window m_Window;
	Timer m_Timer;
	Logger m_Logger;

#ifdef DEBUG
	Debugger m_Debugger;
#endif

private:
	Engine();
};

#endif