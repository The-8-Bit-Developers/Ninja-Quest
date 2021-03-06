#ifdef DEBUG

#pragma once
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Window.h"
#include "Logger.h"
#include "Vec2.h"

/*
	The debugger should not be included in Release mode, but
	displays graphically some useful information.
*/
class Debugger
{
public:
	Debugger(Logger& _logger);
	~Debugger();

	void Draw(Window& window, float fElapsedTime, float physicsTime, bool invert, Vec2 cameraPos);

private:
	sf::Font m_Font;
	Logger* logger;
};

#endif

#endif