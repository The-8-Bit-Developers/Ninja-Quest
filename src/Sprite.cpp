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
	
	m_Width = m_Texture.getSize().x;
	m_Height = m_Texture.getSize().y;
	m_bPhysics = false;
	m_fGravity = 0.0f;
}

Sprite::Sprite(sf::Texture& texture) : m_Scale(1.0f, 1.0f)
{
	m_Sprite.setTexture(texture);
	m_Texture = texture;

	m_ID = s_SpriteIDCount;
	s_SpriteIDCount++;
	s_Sprites[m_ID] = this;

	m_Width = m_Texture.getSize().x;
	m_Height = m_Texture.getSize().y;
	m_bPhysics = false;
	m_fGravity = 0.0f;
}

Sprite::Sprite() : m_Scale(1.0f, 1.0f), m_Width(0), m_Height(0), m_bPhysics(false), m_fGravity(0) { s_SpriteIDCount++; s_Sprites[m_ID] = this; }

#include <iostream>
void Sprite::ComputePhysicsForSprite(float deltaTime)
{
	// Apply gravity
	m_Velocity += Vec2(0.0f, -m_fGravity * deltaTime);

	// Apply velocity and check for collisions in substeps
	bool bCollided = false;
	Vec2 newPosition = m_Position;

	const int steps = 1;
	for (int i = 0; i < steps; ++i)
	{
		if (m_bPhysics) newPosition += m_Velocity / (float)(i+1);

		// Check for collisions by incrementing position by the substep,
		// and if a collision occurs, zero velocity
		for (auto&[id, sprite] : Sprite::s_Sprites)
		{
			if (id == m_ID || sprite->m_bPhysics == false) continue;
			else if (CollidesWith(*sprite, newPosition))
			{
				bCollided = true;
				
				// Estimate the normal, by working out which side
				// the sprite lies on.
				float spriteUpperY = sprite->m_Position.y;
				float spriteLowerY = sprite->m_Position.y + sprite->m_Height;
				float spriteUpperX = sprite->m_Position.x;
				float spriteLowerX = sprite->m_Position.x + sprite->m_Width;

				float upperY = newPosition.y;
				float lowerY = newPosition.y + m_Height;
				float upperX = newPosition.x;
				float lowerX = newPosition.x + m_Width;

				Vec2 normal;
				if (upperY >= spriteUpperY) normal = {  0.0f,  1.0f };
				if (lowerY <  spriteLowerY) normal = {  0.0f, -1.0f };
				else if (upperX >= spriteUpperX) normal = {  1.0f,  0.0f };
				else if (lowerX <  spriteLowerX) normal = { -1.0f,  0.0f };
				//else exit(1);

				if (id == 0 && m_ID == 11) std::cout << "Normal: " << std::string(normal) << std::endl;


				// Take steps to "uncollide" objects by using
				// the normal to calculate the direction the 
				// object should move towards
				auto reflect = [](Vec2 incident, Vec2 normal) { return incident * 2.0f * normal.dot(incident) * normal; };
				Vec2 direction = reflect(sprite->m_Position - newPosition, normal).normalise();
				direction = { 0.0f, 0.0f };
				Vec2 magnitude = (m_Velocity / (float)steps * (float)i) * 1.0f;// Assume masses of 1
				//Vec2 currentVelocity = m_Velocity+ Vec2{0.01f, 0.01f};
				m_Velocity = direction;
			}
			
		}

	}

	m_Position = newPosition;

	//if (!bCollided) m_Position += m_Velocity;
}

Sprite::~Sprite() { s_Sprites.erase(m_ID); }