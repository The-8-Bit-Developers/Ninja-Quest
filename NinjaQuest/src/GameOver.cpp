#include "GameOver.h"
#include "Engine.h"

#include "LuaComponent.h"

GameOver::GameOver()
{
	
}

void GameOver::OnStart()
{
	Engine::Get().SetBackgroundColour(0, 0, 0);

	// Load fonts
	Engine::Get().LoadFont(m_Font, "ka1.ttf");
	Engine::Get().LoadFont(m_OtherFont, "Munro.ttf");

	// Setup text
	m_Text[0].setString("Game over!");
	m_Text[0].setFont(m_Font);
	m_Text[0].setCharacterSize(40);
	m_Text[0].setStyle(sf::Text::Bold);
	m_Text[0].setFillColor(sf::Color::Red);

	m_Text[1].setString("It would appear you have succumbed");
	m_Text[2].setString("to the airbone ninjas...");
	m_Text[3].setString("Press space to restart");
	for (int i = 1; i < 4; ++i)
	{
		m_Text[i].setFont(m_OtherFont);
		m_Text[i].setCharacterSize(30);
		m_Text[i].setStyle(sf::Text::Bold);
		m_Text[i].setFillColor(sf::Color::White);
	}
	

}

int GameOver::OnUpdate()
{
	// Position text first
	m_Text[0].setPosition(sf::Vector2f(Engine::Get().GetTextCentreX(m_Text[0]), 200.0f + 0 * 50.0f));
	for (int i = 1; i < 4; ++i) m_Text[i].setPosition(sf::Vector2f(Engine::Get().GetTextCentreX(m_Text[i]), 230.0f + i * 50.0f));

	Engine::Get().DrawText(m_Text[0]);
	for (int i = 1; i < 4; ++i) Engine::Get().DrawText(m_Text[i]);

	if (Engine::Get().GetKeyDown(sf::Keyboard::Space)) return MENU;

	return -1;
}

void GameOver::OnEnd()
{
	
}