#include "Engine.h"

#include <lua/lua.hpp>
#include <box2d/box2d.h>

#include "Raycast.h"

int main()
{
	Engine::Get().Create("Engine test", 800, 600);

	Engine::Get().m_Lua.ExecuteFile("player.lua");
	Engine::Get().m_Lua.CallGlobalFunction("OnUserCreate");

	while (Engine::Get().Update())
	{
		Engine::Get().BeginFrame();

		//if (Engine::Get().GetSprite(2)) player.m_Scale = { 1.0f, 1.0f };

		Engine::Get().m_Lua.CallGlobalFunction("OnUserUpdate", []() { Engine::Get().m_Lua.PushNumber(Engine::Get().m_Timer.m_fDelta); }, 1, 0);

		Engine::Get().EndFrame();
	}

	return 0;

}