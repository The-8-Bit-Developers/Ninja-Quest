#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Vec2.h"
#include "Logger.h"

/*
	Generic sprite class 
*/
class Sprite
{
public:
	Sprite(const std::string& fileName, Logger& logger);
	Sprite(sf::Texture& texture);
	~Sprite();

	Vec2 m_Position;
	Vec2 m_Scale;

	sf::Texture m_Texture;
	sf::Sprite m_Sprite;
	
};

#endif