#include "MainMenu.h"

void MainMenu::OnStart()
{
	// Generate background with blue sky, flat ground and paralax clouds
	background = Sprite("player.png");
	//background.m_Position = { -200.0f, 120.2f * 2 };
	//background.m_Scale = { 0.1f, 0.1f };
}

int MainMenu::OnUpdate()
{
	return -1;
}

void MainMenu::OnEnd()
{

}