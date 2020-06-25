#pragma once

#include <Sprite.h>

#include "Scene.h"
#include "Lua.h"

constexpr int width2 = 18;
constexpr int height2 = 6;
constexpr int enemies2 = 6;


class Level2 : public Scene
{
public:

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

	void NextLevel();

private:
    Sprite m_GroundSprites[width2*height2];
    Sprite m_Player;

    Sprite m_Enemies[enemies2];

	static int m_ShurikenType;

	static int GetType(lua_State* L);

	sf::Font m_Font;
	sf::Text m_Text;
	sf::Text m_LevelText;

	int m_Level = 2;
};