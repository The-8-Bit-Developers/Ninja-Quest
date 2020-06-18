#pragma once
#ifndef SPLASH_SCREEN_H
#define SPLASH_SCREEN_H

#include <Sprite.h>

#include "Scene.h"

class SplashScreen : public Scene
{
public:
	void OnStart() override;
	int OnUpdate() override;
	void OnEnd() override;

private:
	Sprite logo;
	float m_fTimeCount;
};

#endif