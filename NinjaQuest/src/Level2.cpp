#include "Level2.h"
#include <iostream>

#include "Engine.h"
#include "LuaComponent.h"


constexpr const char* level =
"000220000220000022"
"111111100001111111"
"333333300003333333"
"333333333333333333"
"333333333333333333"
"333333333333333333";

int Level2::m_ShurikenType = 0;

void Level2::OnStart()
{
	Engine::Get().SetBackgroundColour(30, 144, 255);

	srand((unsigned int)time(0));
	LuaComponent::s_CustomFunction = LuaComponent::CustomFunction("GetType", (int*)GetType);

    Engine::Get().SetGravity(200.0f);

    // Generate ground and enemies2 from level string
    float groundX = 0.0f;
	float groundY = 0.0f;
	int enemyCount = 0;
	for (int x = 0; x < width2; ++x)
	{
		for (int y = 0; y < height2; ++y)
		{
			if (level[y*width2+x] == '0') continue;
			if (level[y*width2+x] == '2')
			{
				m_Enemies[enemyCount].m_Position = { groundX + 32.0f * x, groundY - 32.0f * y};
				enemyCount++;
				continue;
			}
			m_GroundSprites[y*width2+x].SetTexture(level[y*width2 + x] == '1' ? "ground.png" : "dirt.png");
			m_GroundSprites[y*width2+x].m_Position = { groundX + 32.0f * x, groundY - 32.0f * y};
			m_GroundSprites[y*width2+x].AddStaticPhysics();
		}
	}

	// Position player
	m_Player.m_Position = {140.0f, 4.0f};

    // Add script to player and enemies2
	m_Player.AddComponent(new LuaComponent("player.lua", "player", m_Player.m_ID)); // The 2nd and 3rd arumgnets give the script the ID of the player
    for (int i = 0; i < enemies2; ++i) 
	{
		m_Enemies[i].AddComponent(new LuaComponent("enemy.lua", "enemy", m_Enemies[i].m_ID));
		dynamic_cast<LuaComponent*>(m_Enemies[i].GetComponent(0))->m_Lua.SetGlobalNumber("player", (float) m_Player.m_ID);
	}

	// Let's draw the colliders when in debug mode
#ifdef DEBUG
	m_Player.m_bDrawCollider = true;
	for (int i = 0; i < enemies2; ++i) m_Enemies[i].m_bDrawCollider = true;
	for (int x = 0; x < width2*height2; ++x) m_GroundSprites[x].m_bDrawCollider = true;
#endif

	// Attach camera script to player because each script must have a sprite
	m_Player.AddComponent(new LuaComponent("camera.lua", "player", m_Player.m_ID));
#ifdef DEBUG
	dynamic_cast<LuaComponent*>(m_Player.GetComponent(1))->m_Lua.SetGlobalBool("debug_mode", true);
#else
	dynamic_cast<LuaComponent*>(m_Player.GetComponent(1))->m_Lua.SetGlobalBool("debug_mode", false);
#endif

	// Set enemy types
	for (int i = 0; i < enemies2; ++i)
	{
		dynamic_cast<LuaComponent*>(m_Enemies[i].GetComponent(0))->m_Lua.SetGlobalNumber("type", (float)(int)(rand() % 2));
	}

	m_ShurikenType = 0;

	// Load fonts
	Engine::Get().LoadFont(m_Font, "Munro.ttf");

	// Setup text
	m_Text.setFont(m_Font);
	m_Text.setCharacterSize(30);
	m_Text.setStyle(sf::Text::Bold);
	m_Text.setFillColor(sf::Color::White);
	m_Text.setPosition(10, 10);

	// Setup text
	m_LevelText.setFont(m_Font);
	m_LevelText.setCharacterSize(30);
	m_LevelText.setStyle(sf::Text::Bold);
	m_LevelText.setFillColor(sf::Color::White);
	m_LevelText.setPosition(10, 40);
}

int Level2::GetType(lua_State* L)
{
	if (lua_gettop(L) != 0) { Log("Invalid number of arguments in function GetType"); return 0; }
	LuaComponent::s_CurrentInstance->m_Lua.PushNumber(m_ShurikenType);
	return 1;
}

int Level2::OnUpdate()
{

	if (Engine::Get().GetKeyDown(sf::Keyboard::Key::Num1)) m_ShurikenType = 0;
	if (Engine::Get().GetKeyDown(sf::Keyboard::Key::Num2)) m_ShurikenType = 1;
	if (Engine::Get().GetKeyDown(sf::Keyboard::Key::Num3)) m_ShurikenType = 2;

	std::string type;
	if (m_ShurikenType == 0) type = "Wheel";
	if (m_ShurikenType == 1) type = "Rock";
	if (m_ShurikenType == 2) type = "Knife";
	m_Text.setString("Shuriken type: " + type);
	Engine::Get().DrawText(m_Text);

	m_LevelText.setString("Level: " + std::to_string(m_Level));
	Engine::Get().DrawText(m_LevelText);

	// Part of player.lua's biggest hack ever
	m_Player.m_PhysicsBody->SetLinearVelocity(b2Vec2(0, m_Player.m_PhysicsBody->GetLinearVelocity().y));

	for (int i = 0; i < enemies2; ++i)
	{
		if (m_Enemies[i].m_bDisabled) continue;

		// Game over
		if (dynamic_cast<LuaComponent*>(m_Enemies[i].GetComponent(0))->m_Lua.GetGlobalBool("game_over")) return GAME_OVER;

		// If hit by a shuriken
		if (dynamic_cast<LuaComponent*>(m_Enemies[i].GetComponent(0))->m_Lua.GetGlobalNumber("shuriken"))
		{
			// Shuriken type
			int data = dynamic_cast<LuaComponent*>(m_Enemies[i].GetComponent(0))->m_Lua.GetGlobalNumber("shuriken");
			int shuriken_type;
			switch (data)
			{
				case 2: shuriken_type = 0; break;
				case 3: shuriken_type = 1; break;
				default: shuriken_type = 3; break;
			}

			int enemyType = dynamic_cast<LuaComponent*>(m_Enemies[i].GetComponent(0))->m_Lua.GetGlobalNumber("type");

			if (enemyType == shuriken_type) m_Enemies[i].Disable(); // delete old sprite
			
		}
		
	}

	// If all enemies2 hit, next level!
	bool x = true;
	for (int i = 0; i < enemies2; ++i)
	{
		if (!m_Enemies[i].m_bDisabled) x = false;
	}
	if (x) return LEVEL3;

	// If the shuriken didn't collide, it's game over
	//if (enemyCollided && shurikenCollided) return GAME_OVER;

	if (m_Player.m_Position.y < -1000) return GAME_OVER;

	return -1;
}

void Level2::OnEnd()
{
	
}
