#pragma once
#ifndef GAME_OVER_H
#define GAME_OVER_H

#include <Sprite.h>

#include "Scene.h"
#include "Sound.h"

class GameOver : public Scene
{
public:

	GameOver();

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:

	sf::Font m_Font;
	sf::Font m_OtherFont;
	sf::Text m_Text[4];

};

#endif