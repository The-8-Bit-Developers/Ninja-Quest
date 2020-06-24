#include "MainMenu.h"
#include "Engine.h"

#include "LuaComponent.h"



void MainMenu::OnStart()
{
	Engine::Get().SetBackgroundColour(0, 0, 0);

	
	

	
	// Load fonts
	Engine::Get().LoadFont(m_Font, "ka1.ttf");
	Engine::Get().LoadFont(m_AboutFont, "Munro.ttf");

	// Setup text
	m_Title.setString("Ninja Quest");
	m_Title.setFont(m_Font);
	m_Title.setCharacterSize(100);
	m_Title.setStyle(sf::Text::Bold);
	m_Title.setFillColor(sf::Color::Red);

	m_Menu[0].setString("Play Again?");
	m_Menu[1].setString("Quit?");
	for (int i = 0; i < menuItems; ++i)
	{
		m_Menu[i].setFont(m_Font);
		m_Menu[i].setCharacterSize(25);
		m_Menu[i].setFillColor(i == m_nMenuItem ? selectedColour : secondaryColour);
	}

	m_Music.Play();

	m_Screen = MAIN;


}

int MainMenu::OnUpdate()
{



	// Draw text and UI
	m_Title.setPosition(sf::Vector2f(Engine::Get().GetTextCentreX(m_Title), 100));
	Engine::Get().DrawText(m_Title);

	// Main menu
	if (m_Screen == MAIN)
	{
		// Draw options
		for (int i = 0; i < menuItems; ++i)
		{
			m_Menu[i].setPosition(sf::Vector2f(Engine::Get().GetTextCentreX(m_Menu[i]), 200.0f + i * 50.0f));
			m_Menu[i].setFillColor(i == m_nMenuItem ? selectedColour : secondaryColour);
			Engine::Get().DrawText(m_Menu[i]);
		}

		// If down or up is pressed and was not last frame, change item
		if (!m_KeysPressed[0] && Engine::Get().GetKeyDown(sf::Keyboard::Up))	m_nMenuItem--;
		if (!m_KeysPressed[1] && Engine::Get().GetKeyDown(sf::Keyboard::Down))	m_nMenuItem++;

		if (m_nMenuItem < 0) m_nMenuItem = menuItems-1;
		if (m_nMenuItem > menuItems-1) m_nMenuItem = 0;

		m_KeysPressed[0] = Engine::Get().GetKeyDown(sf::Keyboard::Up);
		m_KeysPressed[1] = Engine::Get().GetKeyDown(sf::Keyboard::Down);

		if (Engine::Get().GetKeyDown(sf::Keyboard::Enter) && m_nMenuItem == 0) return GAME_TEST;
		if (Engine::Get().GetKeyDown(sf::Keyboard::Enter) && m_nMenuItem == 1) Engine::Get().m_Window.Close();
	}

	

	return -1;
}

void MainMenu::OnEnd()
{
	// Delete sprites
	delete[] m_GroundSprites;
}