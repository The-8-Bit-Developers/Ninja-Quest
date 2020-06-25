#include <iostream>
#include <memory>

#include <Engine.h>
#include <LuaComponent.h>

#include "Scene.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "GameOver.h"
#include "Intro.h"
#include "Winning.h"
#include "Level0.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "Level4.h"


int main()
{
	Engine::Get().Create("Ninja Quest", 1000, 600);

	// Create array of scenes
	constexpr int nScenes = 10;
	Scene* scenes[nScenes];
	for (int i = 0; i < nScenes; ++i) scenes[i] = nullptr;
	unsigned int sceneID;

	// Scene clean up
	auto CleanScenes = [&]()
	{
		if (scenes[0] != nullptr) { delete (SplashScreen*)scenes[0];scenes[0] = nullptr; }
		if (scenes[1] != nullptr) { delete (MainMenu*)scenes[1]; 	scenes[1] = nullptr; }
		if (scenes[2] != nullptr) { delete (Level0*)  scenes[2]; 	scenes[2] = nullptr; }
		if (scenes[3] != nullptr) { delete (Level1*)  scenes[3]; 	scenes[3] = nullptr; }
		if (scenes[4] != nullptr) { delete (Level2*)  scenes[4]; 	scenes[4] = nullptr; }
		if (scenes[5] != nullptr) { delete (Level3*)  scenes[5]; 	scenes[5] = nullptr; }
		if (scenes[6] != nullptr) { delete (Level4*)  scenes[6]; 	scenes[6] = nullptr; }
		if (scenes[7] != nullptr) { delete (GameOver*)scenes[7]; 	scenes[7] = nullptr; }
		if (scenes[8] != nullptr) { delete (Intro*)	  scenes[8]; 	scenes[8] = nullptr; }
		if (scenes[9] != nullptr) { delete (Winning*)scenes[9]; 	scenes[9] = nullptr; }
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
			Engine::Get().Reset();

			sceneID = sceneStatus;
			if (sceneStatus == Scene::MENU) scenes[sceneStatus] = new MainMenu();
			if (sceneStatus == Scene::LEVEL0) scenes[sceneStatus] = new Level0();
			if (sceneStatus == Scene::LEVEL1) scenes[sceneStatus] = new Level1();
			if (sceneStatus == Scene::LEVEL2) scenes[sceneStatus] = new Level2();
			if (sceneStatus == Scene::LEVEL3) scenes[sceneStatus] = new Level3();
			if (sceneStatus == Scene::LEVEL4) scenes[sceneStatus] = new Level4();
			if (sceneStatus == Scene::GAME_OVER) scenes[sceneStatus] = new GameOver();
			if (sceneStatus == Scene::INTRO) scenes[sceneStatus] = new Intro();
			if (sceneStatus == Scene::WINNING) scenes[sceneStatus] = new Winning();

			scenes[sceneStatus]->OnStart();
		}

		Engine::Get().EndFrame();
	}

	CleanScenes();

	return 0;

}