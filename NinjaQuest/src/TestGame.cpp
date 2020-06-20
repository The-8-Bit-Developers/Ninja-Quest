#include "TestGame.h"
#include <iostream>

#include "Engine.h"
#include "LuaComponent.h"

void TestGame::OnStart()
{
    Engine::Get().SetGravity(200.0f);

    // Generate ground
    float groundX = -100.0f;
	float groundY = -20.0f;
    int width = 10;
	for (int x = 0; x < width; ++x)
	{
		m_GroundSprites[x].SetTexture("ground.png");
		m_GroundSprites[x].m_Position = { groundX + 32.0f * x, groundY - 32.0f };
        m_GroundSprites[x].AddStaticPhysics();
	}

    // Add script to player and enemies
	m_Player.AddComponent(new LuaComponent("player.lua"));
    for (int i = 0; i < 5; ++i) m_Enemies[i].AddComponent(new LuaComponent("enemy.lua"));
}

int TestGame::OnUpdate()
{
	return -1;
}

void TestGame::OnEnd()
{
	
}