#pragma once
#ifndef TEST_GAME_H
#define TEST_GAME_H

#include <Sprite.h>

#include "Scene.h"
#include "Lua.h"

constexpr int width = 18;
constexpr int height = 8;
constexpr int enemies = 4;

class TestGame : public Scene
{
public:

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:
    Sprite m_GroundSprites[width*height];
    Sprite m_Player;

    Sprite m_Enemies[enemies];
};

#endif