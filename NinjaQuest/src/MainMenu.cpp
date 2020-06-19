#include "MainMenu.h"
#include "Engine.h"

#include "LuaComponent.h"

constexpr int groundWidth = 50;
constexpr int groundHeight = 10;

MainMenu::MainMenu() : m_Music("disco.wav")
{
	m_Music.SetVolume(0.5f);
	m_Music.SetLoop(true);
}

void MainMenu::OnStart()
{
	Engine::Get().SetBackgroundColour(30, 144, 255);

	// Generate ground sprites
	constexpr float groundX = -200.f;
	float groundY = -100.0f * (400.0f / Engine::Get().m_Window.GetHeight());
	m_GroundSprites = new Sprite[groundWidth*groundHeight];
	for (int x = 0; x < groundWidth; ++x)
	{
		for (int y = 0; y < groundHeight; ++y)
		{
			//if (y == 0) m_GroundSprites[y*groundWidth + x]
			m_GroundSprites[y*groundWidth + x].SetTexture(y == 0 ? "ground.png" : "dirt.png");
			m_GroundSprites[y*groundWidth + x].m_Position = { groundX + 32.0f * x, groundY - 32.0f * y };
		}
	}

	// Position player
	m_Player.SetTexture("player.png");
	m_Player.m_Position = { -170.0f, groundY + 36.0f };
	m_Player.m_Layer = 1;

	// Load fonts
	Engine::Get().LoadFont(m_Font, "ka1.ttf");

	// Setup text
	m_Title.setString("Ninja Quest");
	m_Title.setFont(m_Font);
	m_Title.setCharacterSize(40);
	m_Title.setStyle(sf::Text::Bold);
	m_Title.setFillColor(sf::Color::White);

	m_Menu[0].setString("Start game");
	m_Menu[1].setString("Credits");
	m_Menu[2].setString("Quit");
	for (int i = 0; i < menuItems; ++i)
	{
		m_Menu[i].setFont(m_Font);
		m_Menu[i].setCharacterSize(25);
		m_Menu[i].setFillColor(i == m_nMenuItem ? selectedColour : secondaryColour);
	}

	m_Music.Play();
}

int MainMenu::OnUpdate()
{
	// Move all ground tiles to bottom(ish) of window
	float groundY = -100.0f * (400.0f / Engine::Get().m_Window.GetHeight());
	for (int x = 0; x < groundWidth; ++x)
	{
		for (int y = 0; y < groundHeight; ++y)
		{
			m_GroundSprites[y*groundWidth + x].m_Position.y = groundY - 32.0f * y;
		}
	}

	m_Player.m_Position = { -170.0f, groundY + 36.0f };

	// Draw text and UI
	m_Title.setPosition(sf::Vector2f(Engine::Get().GetTextCentreX(m_Title), 100));
	Engine::Get().DrawText(m_Title);

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

	return -1;
}

void MainMenu::OnEnd()
{
	// Delete sprites
	delete[] m_GroundSprites;
}