#pragma once

#include <Sprite.h>

#include "Scene.h"
#include "Lua.h"

constexpr int width4 = 18;
constexpr int height4 = 8;
constexpr int enemies4 = 9;

class Level4 : public Scene
{
public:

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

	void NextLevel();

private:
    Sprite m_GroundSprites[width4*height4];
    Sprite m_Player;

    Sprite m_Enemies[enemies4];

	static int m_ShurikenType;

	static int GetType(lua_State* L);

	sf::Font m_Font;
	sf::Text m_Text;
	sf::Text m_LevelText;

	int m_Level = 4;
};