#include <iostream>
#include <memory>

#include <Engine.h>
#include <LuaComponent.h>

#include "Scene.h"
#include "SplashScreen.h"
#include "MainMenu.h"

int main()
{
	Engine::Get().Create("Ninja Quest", 800, 600);

	// Create array of scenes
	constexpr int nScenes = 2;
	Scene* scenes[nScenes];
	for (int i = 0; i < nScenes; ++i) scenes[i] = nullptr;
	unsigned int sceneID;

	// Scene clean up
	auto CleanScenes = [&]()
	{
		if (scenes[0] != nullptr) { delete (SplashScreen*)scenes[0]; scenes[0] = nullptr; }
		if (scenes[1] != nullptr) { delete (MainMenu*)scenes[1]; scenes[1] = nullptr; }
	};

	// Start splash screen scene
	scenes[Scene::SPLASH] = new SplashScreen();
	scenes[Scene::SPLASH]->OnStart();
	sceneID = Scene::SPLASH;

	/*
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
	*/

	while (Engine::Get().Update())
	{
		Engine::Get().BeginFrame();

		int sceneStatus = scenes[sceneID]->OnUpdate();
		if (sceneStatus != -1)
		{
			// End scene, null pointer and delete objects
			scenes[sceneID]->OnEnd();
			CleanScenes();

			sceneID = sceneStatus;
			scenes[sceneStatus] = new MainMenu();
			scenes[sceneStatus]->OnStart();
		}

		Engine::Get().EndFrame();
	}

	CleanScenes();

	return 0;

}