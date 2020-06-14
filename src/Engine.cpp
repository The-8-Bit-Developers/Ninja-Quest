#include "Engine.h"

#if DEBUG
Engine::Engine() : m_Debugger(m_Logger) {}
#else
Engine::Engine() {}
#endif

void Engine::Create(const std::string& windowName, const int windowWidth, const int windowHeight)
{
	m_Window.Create(windowName, windowWidth, windowHeight);
}

bool Engine::Update()
{
	sf::Event event;
	while (m_Window.GetEvent(event))
	{
		if (event.type == sf::Event::Closed) m_Window.Close();
	}

	return !m_Window.ShouldClose();
}

void Engine::BeginFrame()
{
	// Begin dela-time calclation
	m_Timer.BeginFrame();

	// Render game
	m_Window.Clear(sf::Color::Black);
}

void Engine::EndFrame()
{	
	// If in debug mode, display FPS
#ifdef DEBUG
	m_Debugger.Draw(m_Window, m_Timer.m_fDelta);
#endif

	// Swap buffers
	m_Window.Display();

	// End delta-time calculation
	m_Timer.EndFrame();
}

Engine::~Engine() {}
