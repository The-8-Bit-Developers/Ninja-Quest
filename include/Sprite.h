#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

#include "Vec2.h"
#include "Logger.h"

/*
	Generic sprite class that also contains all pointers to sprites
*/
class Sprite
{
public:
	Sprite(const std::string& fileName);
	Sprite(sf::Texture& texture);
	Sprite(const Sprite& sprite);
	Sprite& operator=(const Sprite &rhs);
	Sprite();
	~Sprite();

	Vec2 m_Position;
	Vec2 m_Scale;

	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	
	static Logger* logger;
	static unsigned int s_SpriteIDCount;
	unsigned int m_ID;

	static std::unordered_map<unsigned int, Sprite*> s_Sprites;
};

#endif