#pragma once

#include <Sprite.h>

#include "Scene.h"
#include "Lua.h"

constexpr int width0 = 18;
constexpr int height0 = 6;
constexpr int enemies0 = 2;

class Level0 : public Scene
{
public:

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

	void NextLevel();

private:
    Sprite m_GroundSprites[width0*height0];
    Sprite m_Player;

    Sprite m_Enemies[enemies0];

	static int m_ShurikenType;

	static int GetType(lua_State* L);

	sf::Font m_Font;
	sf::Text m_Text;
	sf::Text m_LevelText;

	int m_Level = 0;
};