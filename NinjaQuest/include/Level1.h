#pragma once

#include <Sprite.h>

#include "Scene.h"
#include "Lua.h"

constexpr int width1 = 18;
constexpr int height1 = 6;
constexpr int enemies1 = 4;


class Level1 : public Scene
{
public:

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

	void NextLevel();

private:
    Sprite m_GroundSprites[width1*height1];
    Sprite m_Player;

    Sprite m_Enemies[enemies1];

	static int m_ShurikenType;

	static int GetType(lua_State* L);

	sf::Font m_Font;
	sf::Text m_Text;
	sf::Text m_LevelText;

	int m_Level = 1;
};