#pragma once
#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Camera.h"

/*
	The purpose of the window class is to provide an abstraction from
	SFML, so that ports to other platforms should be trivial
*/
class Window
{
public:
	Window();
	void Create(const std::string& title, const int width, const int height);
	~Window();

	void SetPosition(int x, int y);
	void SetSize(unsigned int width, unsigned int height);
	void SetTitle(const std::string& title);

	int GetWidth();
	int GetHeight();
	
	bool GetKeyDown(sf::Keyboard::Key key);
	bool GetEvent(sf::Event& event);

	void Clear(sf::Color colour);

	void Draw(sf::Sprite& sprite);
	void Draw(sf::Text& text);

	void SetCamera(Camera& camera);

	void Display();

	bool ShouldClose();
	void Close();

private:
	sf::RenderWindow m_Window;

	int m_Width;
	int m_Height;
};

#endif