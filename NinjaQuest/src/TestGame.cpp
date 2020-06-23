#include "TestGame.h"
#include <iostream>

#include "Engine.h"
#include "LuaComponent.h"

constexpr const char* level = // 0 - empty, 1 - ground, 2 - enemy
"0000000000"
"0000110000"
"0221111220"
"1111111111";

void TestGame::OnStart()
{
    Engine::Get().SetGravity(200.0f);

    // Generate ground and enemies from level string
    float groundX = 0.0f;
	float groundY = 0.0f;
	int enemyCount = 0;
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (level[y*width+x] == '0') continue;
			if (level[y*width+x] == '2')
			{
				m_Enemies[enemyCount].m_Position = { groundX + 32.0f * x, groundY - 32.0f * y};
				enemyCount++;
				continue;
			}
			m_GroundSprites[y*width+x].SetTexture("ground.png");
			m_GroundSprites[y*width+x].m_Position = { groundX + 32.0f * x, groundY - 32.0f * y};
			m_GroundSprites[y*width+x].AddStaticPhysics();
		}
	}

	// Position player
	m_Player.m_Position = {140.0f, 4.0f};

    // Add script to player and enemies
	m_Player.AddComponent(new LuaComponent("player.lua", "player", m_Player.m_ID)); // The 2nd and 3rd arumgnets give the script the ID of the player
    for (int i = 0; i < enemies; ++i) m_Enemies[i].AddComponent(new LuaComponent("enemy.lua", "enemy", m_Enemies[i].m_ID));

	// Let's draw the colliders when in debug mode
#ifdef DEBUG
	m_Player.m_bDrawCollider = true;
	for (int i = 0; i < enemies; ++i) m_Enemies[i].m_bDrawCollider = true;
	for (int x = 0; x < width*height; ++x) m_GroundSprites[x].m_bDrawCollider = true;
#endif

	// Attach camera script to player because each script must have a sprite
	m_Player.AddComponent(new LuaComponent("camera.lua", "player", m_Player.m_ID));
#ifdef DEBUG
	dynamic_cast<LuaComponent*>(m_Player.GetComponent(1))->m_Lua.SetGlobalBool("debug_mode", true);
#else
	dynamic_cast<LuaComponent*>(m_Player.GetComponent(1))->m_Lua.SetGlobalBool("debug_mode", false);
#endif
}

int TestGame::OnUpdate()
{
	// Part of player.lua's biggest hack ever
	m_Player.m_PhysicsBody->SetLinearVelocity(b2Vec2(0, m_Player.m_PhysicsBody->GetLinearVelocity().y));

	return -1;
}

void TestGame::OnEnd()
{
	
}
