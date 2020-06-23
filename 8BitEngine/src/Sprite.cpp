#include "Sprite.h"
#include "Config.h"

Logger* Sprite::logger = nullptr;
unsigned int Sprite::s_SpriteIDCount = 0;
std::unordered_map<unsigned int, Sprite*> Sprite::s_Sprites;

b2World Sprite::s_PhysicsWorld(b2Vec2(0.0f, -10.0f));

Sprite::Sprite(const std::string& fileName) : m_Scale(1.0f, 1.0f), m_Layer(0)
{
	m_Texture = Texture::GetTexture(fileName)->m_Texture;
	m_Sprite.setTexture(*m_Texture);
	
	m_ID = s_SpriteIDCount;
	s_SpriteIDCount++;
	s_Sprites[m_ID] = this;
	
	m_Width = m_Texture->getSize().x;
	m_Height = m_Texture->getSize().y;

	m_PhysicsBody = nullptr;
}

Sprite::Sprite(sf::Texture* texture) : m_Scale(1.0f, 1.0f), m_Layer(0)
{
	m_Sprite.setTexture(*texture);
	m_Texture = texture;

	m_ID = s_SpriteIDCount;
	s_SpriteIDCount++;
	s_Sprites[m_ID] = this;

	m_Width = m_Texture->getSize().x;
	m_Height = m_Texture->getSize().y;

	m_PhysicsBody = nullptr;
}

Sprite::Sprite() : m_Scale(1.0f, 1.0f), m_Width(0), m_Height(0), m_Layer(0) 
{ 
	m_ID = s_SpriteIDCount;
	s_SpriteIDCount++; 
	s_Sprites[m_ID] = this;
	m_PhysicsBody = nullptr;
}

void Sprite::AddStaticPhysics(float width, float height)
{
	if (m_PhysicsBody != nullptr) RemovePhysics();

	// Define a body with position, etc
	b2BodyDef bodyDefinition;
	bodyDefinition.position.Set(m_Position.x * PHYSICS_SCALE, m_Position.y * PHYSICS_SCALE);
	
	// Create the body in the world
	m_PhysicsBody = s_PhysicsWorld.CreateBody(&bodyDefinition);
	m_PhysicsBody->SetFixedRotation(true);
	m_PhysicsBody->SetTransform(b2Vec2(m_Position.x * PHYSICS_SCALE, m_Position.y * PHYSICS_SCALE), 0.0f);

	// Define shape, friction, density, etc
	b2PolygonShape shape;
	shape.SetAsBox(width * 0.5f * PHYSICS_SCALE, height * 0.5f * PHYSICS_SCALE);
	m_PhysicsBody->CreateFixture(&shape, 0.0f);

}

void Sprite::AddStaticPhysicsSphere(float radius)
{
	if (m_PhysicsBody != nullptr) RemovePhysics();

	// Define a body with position, etc
	b2BodyDef bodyDefinition;
	bodyDefinition.position.Set(m_Position.x * PHYSICS_SCALE, m_Position.y * PHYSICS_SCALE);
	
	// Create the body in the world
	m_PhysicsBody = s_PhysicsWorld.CreateBody(&bodyDefinition);
	m_PhysicsBody->SetFixedRotation(true);
	m_PhysicsBody->SetTransform(b2Vec2(m_Position.x * PHYSICS_SCALE, m_Position.y * PHYSICS_SCALE), 0.0f);

	// Define shape, friction, density, etc
	b2CircleShape body;
	body.m_radius = radius * PHYSICS_SCALE;

	m_PhysicsBody->CreateFixture(&body, 0.0f);
}

void Sprite::SetTexture(sf::Texture* texture)
{
	m_Texture = texture;
	m_Sprite.setTexture(*texture);
	m_Width = texture->getSize().x;
	m_Height = texture->getSize().y;
}

void Sprite::SetTexture(const std::string & fileName)
{
	SetTexture(Texture::GetTexture(fileName)->m_Texture);
}

void Sprite::AddDynamicPhysics(float density, float width, float height)
{
	if (m_PhysicsBody != nullptr) RemovePhysics();

	// Define a body with position, etc
	b2BodyDef bodyDefinition;
	bodyDefinition.type = b2_dynamicBody;
	bodyDefinition.position.Set(m_Position.x * PHYSICS_SCALE, m_Position.y * PHYSICS_SCALE);

	// Create the body in the world
	m_PhysicsBody = s_PhysicsWorld.CreateBody(&bodyDefinition);
	m_PhysicsBody->SetFixedRotation(true);
	m_PhysicsBody->SetTransform(b2Vec2(m_Position.x * PHYSICS_SCALE, m_Position.y * PHYSICS_SCALE), 0.0f);

	// Define shape, friction, density, etc
	b2PolygonShape bodyBox;
	bodyBox.SetAsBox(0.5f * (float)width * PHYSICS_SCALE, 0.5f * (float)height * PHYSICS_SCALE);

	// Create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bodyBox;
	fixtureDef.density = density;
	fixtureDef.friction = 100.0f;
	m_PhysicsBody->CreateFixture(&fixtureDef);

}

void Sprite::AddDynamicPhysicsSphere(float density, float radius)
{
	if (m_PhysicsBody != nullptr) RemovePhysics();

	// Define a body with position, etc
	b2BodyDef bodyDefinition;
	bodyDefinition.type = b2_dynamicBody;
	bodyDefinition.position.Set(m_Position.x * PHYSICS_SCALE, m_Position.y * PHYSICS_SCALE);

	// Create the body in the world
	m_PhysicsBody = s_PhysicsWorld.CreateBody(&bodyDefinition);
	m_PhysicsBody->SetFixedRotation(true);
	m_PhysicsBody->SetTransform(b2Vec2(m_Position.x * PHYSICS_SCALE, m_Position.y * PHYSICS_SCALE), 0.0f);

	// Define shape, friction, density, etc
	b2CircleShape bodyCircle;
	bodyCircle.m_radius = radius * PHYSICS_SCALE;

	// Create fixture
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &bodyCircle;
	fixtureDef.density = density;
	fixtureDef.friction = 100.0f;
	m_PhysicsBody->CreateFixture(&fixtureDef);

}

void Sprite::RemovePhysics()
{
	if (m_PhysicsBody != nullptr) s_PhysicsWorld.DestroyBody(m_PhysicsBody);
	m_PhysicsBody = nullptr;
}

Component* Sprite::GetComponent(int id)
{
	return m_Components[id];
}

int Sprite::AddComponent(Component* component)
{
	m_Components.push_back(component);
	component->m_ParentID = m_ID;
	return m_Components.size() - 1;
}

void Sprite::RemoveComponent(int component)
{
	m_Components.erase(m_Components.begin() + component);
}

Sprite::~Sprite() 
{ 
	s_Sprites.erase(m_ID); 
	s_SpriteIDCount--;
	if (m_PhysicsBody != nullptr) s_PhysicsWorld.DestroyBody(m_PhysicsBody);
	for (Component* c : m_Components) delete c;
}