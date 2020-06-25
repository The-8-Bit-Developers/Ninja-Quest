#include "Winning.h"
#include "Engine.h"

#include "LuaComponent.h"

Winning::Winning()
{
	
}

void Winning::OnStart()
{
	Engine::Get().SetBackgroundColour(0, 0, 0);

	// Load fonts
	Engine::Get().LoadFont(m_Font, "ka1.ttf");

	// Setup text
	m_Text[0].setString("You won!");
	m_Text[0].setFont(m_Font);
	m_Text[0].setCharacterSize(40);
	m_Text[0].setStyle(sf::Text::Bold);
	m_Text[0].setFillColor(sf::Color::Green);

}

int Winning::OnUpdate()
{
	// Position text first
	m_Text[0].setPosition(sf::Vector2f(Engine::Get().GetTextCentreX(m_Text[0]), 200.0f + 0 * 50.0f));
	for (int i = 1; i < 4; ++i) m_Text[i].setPosition(sf::Vector2f(Engine::Get().GetTextCentreX(m_Text[i]), 230.0f + i * 50.0f));

	if (Engine::Get().GetKeyDown(sf::Keyboard::Space)) return MENU;

	return -1;
}

void Winning::OnEnd()
{
	
}