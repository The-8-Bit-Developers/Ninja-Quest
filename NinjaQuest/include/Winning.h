#pragma once

#include <Sprite.h>

#include "Scene.h"
#include "Sound.h"

class Winning : public Scene
{
public:

	Winning();

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:
	sf::Text m_Text[1];
	sf::Font m_Font;

};