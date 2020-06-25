#pragma once
#ifndef INTRO_H
#define INTRO_H

#include <Sprite.h>

#include "Scene.h"
#include "Sound.h"

class Intro : public Scene
{
public:

	Intro();

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:

	sf::Font m_Font;
	sf::Font m_OtherFont;
	sf::Text m_Text[8];

};

#endif