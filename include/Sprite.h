#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>
#include <iostream>

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
	
	unsigned int m_ID;

	// --- Physics ---

	inline bool CollidesWith(Sprite& sprite, Vec2 newPosition)
	{
		/*
		return	(newPosition.x >= sprite.m_Position.x && newPosition.x <= sprite.m_Position.x + sprite.m_Width) &&
				(newPosition.y >= sprite.m_Position.y && newPosition.y <= sprite.m_Position.y + sprite.m_Height);
		*/

		Vec2 otherMax = Vec2(sprite.m_Position.x + sprite.m_Width, -sprite.m_Position.y + sprite.m_Height);
		Vec2 otherMin = Vec2(sprite.m_Position.x, -sprite.m_Position.y);

		Vec2 thisMax = Vec2(newPosition.x + m_Width, -newPosition.y + m_Height);
		Vec2 thisMin = Vec2(newPosition.x, -newPosition.y);

		return (otherMax.x > thisMin.x && otherMin.x < thisMax.x
			&& otherMax.y > thisMin.y && otherMin.y < thisMax.y);
	}

	void ComputePhysicsForSprite(float deltaTime);

	int m_Width;
	int m_Height;
	bool m_bPhysics;
	float m_fGravity;
	Vec2 m_Velocity;

#ifdef DEBUG
	bool m_bDrawCollider;
#endif

public:
	static Logger* logger;
	static unsigned int s_SpriteIDCount;
	static std::unordered_map<unsigned int, Sprite*> s_Sprites;
};

#endif