#include "Sprite.h"

Logger* Sprite::logger = nullptr;
unsigned int Sprite::s_SpriteIDCount = 0;
std::unordered_map<unsigned int, Sprite*> Sprite::s_Sprites;

b2World Sprite::s_PhysicsWorld(b2Vec2(0.0f, -10.0f));

Sprite::Sprite(const std::string& fileName) : m_Scale(1.0f, 1.0f)
{
	if (!m_Texture.loadFromFile("../res/" + fileName)) logger->Err("Could not load sprite with texture " + fileName);
	else m_Sprite.setTexture(m_Texture);
	
	m_ID = s_SpriteIDCount;
	s_SpriteIDCount++;
	s_Sprites[m_ID] = this;
	
	m_Width = m_Texture.getSize().x;
	m_Height = m_Texture.getSize().y;
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
}

Sprite::Sprite() : m_Scale(1.0f, 1.0f), m_Width(0), m_Height(0) { s_SpriteIDCount++; s_Sprites[m_ID] = this; }

void Sprite::AddStaticPhysics()
{
	if (m_PhysicsBody != nullptr) RemovePhysics();

	// Define a body with position, etc
	b2BodyDef bodyDefinition;
	bodyDefinition.position.Set(m_Position.x, m_Position.y);
	
	// Create the body in the world
	m_PhysicsBody = s_PhysicsWorld.CreateBody(&bodyDefinition);
	m_PhysicsBody->SetFixedRotation(true);

	// Define shape, friction, density, etc
	b2PolygonShape bodyBox;
	bodyBox.SetAsBox(0.5f * (float)m_Width, 0.5f * (float)m_Height);

	m_PhysicsBody->CreateFixture(&bodyBox, 0.0f);
}

void Sprite::AddDynamicPhysics(float density)
{
	if (m_PhysicsBody != nullptr) RemovePhysics();

	// Define a body with position, etc
	b2BodyDef bodyDefinition;
	bodyDefinition.type = b2_dynamicBody;
	bodyDefinition.position.Set(m_Position.x, m_Position.y);

	// Create the body in the world
	m_PhysicsBody = s_PhysicsWorld.CreateBody(&bodyDefinition);
	m_PhysicsBody->SetFixedRotation(true);

	// Define shape, friction, density, etc
	b2PolygonShape bodyBox;
	bodyBox.SetAsBox(0.5f * (float)m_Width, 0.5f * (float)m_Height);

	// Create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bodyBox;
	fixtureDef.density = density;
	fixtureDef.friction = 1.0f;
	m_PhysicsBody->CreateFixture(&fixtureDef);
}

void Sprite::RemovePhysics()
{
	if (m_PhysicsBody != nullptr) s_PhysicsWorld.DestroyBody(m_PhysicsBody);
	m_PhysicsBody = nullptr;
}

Sprite::~Sprite() { s_Sprites.erase(m_ID); if (m_PhysicsBody != nullptr) s_PhysicsWorld.DestroyBody(m_PhysicsBody); }