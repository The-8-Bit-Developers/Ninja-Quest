#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Scene.h"

class TestGame : public Scene
{
public:

	TestGame();

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:

};

#endif