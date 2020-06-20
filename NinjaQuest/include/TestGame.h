#pragma once
#ifndef TEST_GAME_H
#define TEST_GAME_H

#include <Sprite.h>

#include "Scene.h"

class TestGame : public Scene
{
public:

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:
    Sprite m_Player;
    Sprite m_Enemies[5];
    Sprite m_GroundSprites[10];
};

#endif