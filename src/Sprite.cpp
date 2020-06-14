#include "Sprite.h"

Sprite::Sprite(const std::string& fileName, Logger& logger) : m_Scale(1.0f, 1.0f)
{
	if (!m_Texture.loadFromFile("../res/" + fileName)) logger.Err("Could not load sprite with texture " + fileName);
	else m_Sprite.setTexture(m_Texture);
}

Sprite::Sprite(sf::Texture& texture) : m_Scale(1.0f, 1.0f)
{
	m_Sprite.setTexture(texture);
	m_Texture = texture;
}

Sprite::~Sprite()
{

}