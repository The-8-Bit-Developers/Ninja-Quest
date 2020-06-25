#pragma once

#include <Sprite.h>

#include "Scene.h"
#include "Lua.h"

constexpr int width3 = 18;
constexpr int height3 = 7;
constexpr int enemies3 = 8;

class Level3 : public Scene
{

public:

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

	void NextLevel();

private:
    Sprite m_GroundSprites[width3*height3];
    Sprite m_Player;

    Sprite m_Enemies[enemies3];

	static int m_ShurikenType;

	static int GetType(lua_State* L);

	sf::Font m_Font;
	sf::Text m_Text;
	sf::Text m_LevelText;

	int m_Level = 3;
};