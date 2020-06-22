#include "TestGame.h"
#include <iostream>

#include "Engine.h"
#include "LuaComponent.h"

constexpr const char* level = "\
1000000001\
1000110001\
1001111001\
1111111111\
";

void TestGame::OnStart()
{
    Engine::Get().SetGravity(200.0f);

    // Generate ground from level string
    float groundX = -100.0f;
	float groundY = -20.0f;
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			if (level[y*height+x] == '0') continue;
			m_GroundSprites[y*height+x].SetTexture("ground.png");
			m_GroundSprites[y*height+x].m_Position = { groundX + 32.0f * x, groundY - 32.0f * y};
			m_GroundSprites[y*height+x].AddStaticPhysics();
		}
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
	return -1;
}

void TestGame::OnEnd()
{
	
}
