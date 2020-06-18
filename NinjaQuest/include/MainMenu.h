#pragma once
#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <Sprite.h>

#include "Scene.h"

class MainMenu : public Scene
{
public:

	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:

	Sprite background;

};

#endif