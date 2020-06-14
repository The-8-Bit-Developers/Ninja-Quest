#ifdef DEBUG

#include "Debugger.h"

Debugger::Debugger(Logger& _logger)
{
	if (!m_Font.loadFromFile("../res/Munro.ttf")) _logger.Err("Debug font could not be loaded!");
	logger = &_logger;
}

void Debugger::Draw(Window& window, float fElapsedTime)
{
	const_cast<sf::Texture&>(m_Font.getTexture(30)).setSmooth(false);

	sf::Text text("", m_Font);
	text.setCharacterSize(30);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::White);

	int lineCount = 0;
	const int lineHeight = text.getCharacterSize();

	float x = 10;
	float y = 10;

	// Draw fps
	text.setPosition(sf::Vector2f(x, y));
	text.setString("Frame took " + std::to_string(int(fElapsedTime)) + " ms - (" + std::to_string(int(1000.0f / fElapsedTime)) + " FPS)");
	window.Draw(text);
	y += 50;

	// Draw log messages
	for (const std::string& message : logger->m_Messages)
	{
		text.setPosition(sf::Vector2f(x, y + lineCount * lineHeight));
		text.setString(message);
		window.Draw(text);
		lineCount++;
	}

}

Debugger::~Debugger() {}

#endif