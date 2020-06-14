#include "Engine.h"

#include <lua/lua.hpp>

int main()
{

	Engine::Get().Create("Engine test", 800, 600);

	Engine::Get().m_Lua.ExecuteFile("game.lua");
	Engine::Get().m_Lua.CallGlobalFunction("OnUserCreate");
	//Log(Engine::Get().m_Lua.GetGlobalNumber());


	Sprite player("boomer.jpg", Engine::Get().m_Logger);
	player.m_Scale = { 0.5f, 0.5f };
	player.m_Position = { 100.0f, 0.5f };
	Engine::Get().m_Sprites.push_back(player);

	while (Engine::Get().Update())
	{
		Engine::Get().BeginFrame();

		const float speed = 1.0f;
		if (Engine::Get().GetKeyDown(sf::Keyboard::Key::W)) player.m_Position.y -= speed * Engine::Get().m_Timer.m_fDelta;
		if (Engine::Get().GetKeyDown(sf::Keyboard::Key::A)) player.m_Position.x -= speed * Engine::Get().m_Timer.m_fDelta;
		if (Engine::Get().GetKeyDown(sf::Keyboard::Key::S)) player.m_Position.y += speed * Engine::Get().m_Timer.m_fDelta;
		if (Engine::Get().GetKeyDown(sf::Keyboard::Key::D)) player.m_Position.x += speed * Engine::Get().m_Timer.m_fDelta;

		Engine::Get().EndFrame();
	}

	return 0;

}