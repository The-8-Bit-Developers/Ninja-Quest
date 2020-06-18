#include <iostream>
#include <memory>

#include <Engine.h>
#include <Raycast.h>
#include <LuaComponent.h>

int main()
{
	Engine::Get().Create("Engine test", 800, 600);

	// Set gravity
	Engine::Get().SetGravity(200.0f);

	// Add player
	Sprite player;
	player.AddComponent(new LuaComponent("player.lua"));

	// Generate ground
	std::unique_ptr<Sprite> ground[10];
	for (int i = 0; i < 10; ++i)
	{
		ground[i] = std::make_unique<Sprite>("ground.png");
		ground[i]->m_Position = Vec2(i * 32.0f - 5.0f*32.0f, -50.0f);
		ground[i]->AddStaticPhysics();
	}

	while (Engine::Get().Update())
	{
		Engine::Get().BeginFrame();

		Engine::Get().EndFrame();
	}

	return 0;

}