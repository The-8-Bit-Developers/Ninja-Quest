#pragma once
#ifndef TEST_GAME_H
#define TEST_GAME_H

#include <Sprite.h>

#include "Scene.h"

constexpr int width = 10;
constexpr int height = 4;

class TestGame : public Scene
{
public:

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:
    Sprite m_GroundSprites[width*height];
    Sprite m_Player;
    Sprite m_Enemies[5];
};

#endif