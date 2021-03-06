#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

#include <string>
#include <unordered_map>
#include <iostream>

#include "Vec2.h"
#include "Logger.h"
#include "Texture.h"
#include "Component.h"

/*
	Generic sprite class that also manages world state
*/
class Sprite
{
public:
	Sprite(const std::string& fileName);
	Sprite(sf::Texture* texture);
	Sprite();
	~Sprite();

	void Disable();
	bool m_bDisabled = false;

	Vec2 m_Position;
	Vec2 m_Scale;

	sf::Texture* m_Texture = nullptr;
	sf::Sprite m_Sprite;
	
	void SetTexture(sf::Texture* texture);
	void SetTexture(const std::string& fileName);

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

	int m_Width;
	int m_Height;

#ifdef DEBUG
	bool m_bDrawCollider = false;
#endif

	int m_Layer;

	Component* GetComponent(int id);
	int AddComponent(Component* component);
	void RemoveComponent(int component);
	std::vector<Component*> m_Components;

	bool bDelete = false;

public:
	static Logger* logger;
	static unsigned int s_SpriteIDCount;
	static std::unordered_map<unsigned int, Sprite*> s_Sprites;

public:
	// Physics
	static b2World s_PhysicsWorld;

	void AddStaticPhysics(float width, float height);
	inline void AddStaticPhysics() { AddStaticPhysics((float)m_Width, (float)m_Height); }
	void AddStaticPhysicsSphere(float radius);
	inline void AddStaticPhysicsSphere() { AddStaticPhysicsSphere((float)m_Width/2.0f); }
	void AddDynamicPhysics(float density, float width, float height);
	inline void AddDynamicPhysics(float density) { AddDynamicPhysics(density, (float)m_Width, (float)m_Height); }
	void AddDynamicPhysicsSphere(float density, float radius);
	inline void AddDynamicPhysicsSphere(float density) { AddDynamicPhysicsSphere(density, (float)m_Width/2.0f); }
	void RemovePhysics();
	b2Body* m_PhysicsBody = nullptr;
	Vec2 lastPhysicsPosition;

};

#endif