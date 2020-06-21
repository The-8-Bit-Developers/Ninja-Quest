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

	// Position enemies and player
	m_Player.m_Position.x = -100.0f;
	for (int i = 0; i < 5; ++i) m_Enemies[i].m_Position.x = -50.0f + i*64.0f;

    // Add script to player and enemies
	m_Player.AddComponent(new LuaComponent("player.lua", "player", m_Player.m_ID)); // The 2nd and 3rd arumgnets give the script the ID of the player
    for (int i = 0; i < 5; ++i) m_Enemies[i].AddComponent(new LuaComponent("enemy.lua", "enemy", m_Enemies[i].m_ID));

	// Let's draw the colliders when in debug mode
#ifdef DEBUG
	m_Player.m_bDrawCollider = true;
	for (int i = 0; i < 5; ++i) m_Enemies[i].m_bDrawCollider = true;
	for (int x = 0; x < width; ++x) m_GroundSprites[x].m_bDrawCollider = true;
#endif
}

int TestGame::OnUpdate()
{
	return -1;
}

void TestGame::OnEnd()
{
	
}