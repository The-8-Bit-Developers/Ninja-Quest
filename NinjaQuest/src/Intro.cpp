#include "Intro.h"
#include "Engine.h"
#include "LuaComponent.h"

Intro::Intro()
{
	
}

void Intro::OnStart()
{
	Engine::Get().SetBackgroundColour(0, 0, 0);

	// Load fonts
	Engine::Get().LoadFont(m_Font, "ka1.ttf");
	Engine::Get().LoadFont(m_OtherFont, "Munro.ttf");

	// Setup text
	m_Text[0].setString("Oh dear!");
	m_Text[0].setFont(m_Font);
	m_Text[0].setCharacterSize(40);
	m_Text[0].setStyle(sf::Text::Bold);
	m_Text[0].setFillColor(sf::Color::Red);

	m_Text[1].setString("Due to a rather unfortunate misunderstanding regarding");
	m_Text[2].setString("reallocation of orphanage funds, you find yourself amongst");
	m_Text[3].setString("the clouds, fighting for you life against a series of air-borne Ninjas.");
	m_Text[4].setString("It would appear that by using 1, 2 or 3, you can kill specific Ninjas,");
	m_Text[5].setString("But be warned, for some are impervious to certain weapons!");
	m_Text[5].setString("Hopefully this confusion can come to a swift, non-fatal end...");
	m_Text[6].setString("Press space to continue");
	for (int i = 1; i < 7; ++i)
	{
		m_Text[i].setFont(m_OtherFont);
		m_Text[i].setCharacterSize(30);
		m_Text[i].setStyle(sf::Text::Bold);
		m_Text[i].setFillColor(sf::Color::White);
	}
	

}

int Intro::OnUpdate()
{
	// Position text first
	m_Text[0].setPosition(sf::Vector2f(Engine::Get().GetTextCentreX(m_Text[0]), 100.0f + 0 * 50.0f));
	for (int i = 1; i < 7; ++i) m_Text[i].setPosition(sf::Vector2f(Engine::Get().GetTextCentreX(m_Text[i]), 130.0f + i * 50.0f));

	Engine::Get().DrawText(m_Text[0]);
	for (int i = 1; i < 7; ++i) Engine::Get().DrawText(m_Text[i]);

	if (Engine::Get().GetKeyDown(sf::Keyboard::Space)) return LEVEL0;

	return -1;
}

void Intro::OnEnd()
{
	
}