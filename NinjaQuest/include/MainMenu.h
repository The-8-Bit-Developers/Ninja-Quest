#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <Sprite.h>

#include "Scene.h"
#include "Sound.h"

constexpr int menuItems = 3;
constexpr int aboutItems = 4;

class MainMenu : public Scene
{
public:

	MainMenu();

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:

	Sprite* m_GroundSprites;
	Sprite m_Player;

	sf::Font m_Font;
	sf::Font m_AboutFont;

	sf::Text m_Title;

	sf::Text m_Menu[menuItems];
	int m_nMenuItem = 0;

	sf::Color selectedColour =  sf::Color(255, 255, 255);
	sf::Color secondaryColour = sf::Color(200, 200, 200);

	int m_KeysPressed[2];

	Sound m_Music;

	enum Screen
	{
		MAIN = 0,
		ABOUT = 1
	};
	Screen m_Screen;

	sf::Text m_Abouts[aboutItems];

};

#endif