#include "Sprite.h"

Logger* Sprite::logger = nullptr;
unsigned int Sprite::s_SpriteIDCount = 0;
std::unordered_map<unsigned int, Sprite*> Sprite::s_Sprites;

Sprite::Sprite(const std::string& fileName) : m_Scale(1.0f, 1.0f)
{
	if (!m_Texture.loadFromFile("../res/" + fileName)) logger->Err("Could not load sprite with texture " + fileName);
	else m_Sprite.setTexture(m_Texture);
	m_ID = s_SpriteIDCount;
	s_SpriteIDCount++;
	s_Sprites[m_ID] = this;
}

Sprite::Sprite(sf::Texture& texture) : m_Scale(1.0f, 1.0f)
{
	m_Sprite.setTexture(texture);
	m_Texture = texture;
	m_ID = s_SpriteIDCount;
	s_SpriteIDCount++;
	s_Sprites[m_ID] = this;
}

Sprite::Sprite() : m_Scale(1.0f, 1.0f) { s_SpriteIDCount++; s_Sprites[m_ID] = this; }

#include <iostream>
Sprite::~Sprite()
{
	s_Sprites.erase(m_ID);
	std::cout << "Deleted it " << std::to_string(m_ID) << std::endl;
}