#include "Engine.h"

#include <iostream>

#include <lua/lua.hpp>
#include <box2d/box2d.h>

#include "Raycast.h"

#include "LuaComponent.h"

int main()
{
	Engine::Get().Create("Engine test", 800, 600);

	Sprite player;
	player.AddComponent(new LuaComponent("player.lua"));

	while (Engine::Get().Update())
	{
		Engine::Get().BeginFrame();

		Engine::Get().EndFrame();
	}

	return 0;

}