#include "SplashScreen.h"

#include "Engine.h"

constexpr float nSplashScreenFrames = 2000;

void SplashScreen::OnStart()
{
	// Load splash screen sprite
	logo.SetTexture("splashscreen.png");
	logo.m_Scale = { 2.0f, 2.0f };
	logo.m_Position = { -189.0f * logo.m_Scale.x * 0.5f, 32.0f * logo.m_Scale.y * 0.5f };

	// Set background colour to white
	Engine::Get().SetBackgroundColour(255, 255, 255);
}

int SplashScreen::OnUpdate()
{
	m_fTimeCount += Engine::Get().m_Timer.m_fDelta;

	if (m_fTimeCount >= nSplashScreenFrames) return MENU;

	return -1;
}

void SplashScreen::OnEnd()
{

}