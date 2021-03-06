#include "Engine.h"
#include  <iostream>

#include "Raycast.h"

#if DEBUG
Engine::Engine() : m_Debugger(m_Logger) {}
#else
Engine::Engine() {}
#endif

void Engine::Create(const std::string& windowName, const int windowWidth, const int windowHeight)
{
	m_Window.Create(windowName, windowWidth, windowHeight);
	Sprite::logger = &m_Logger;
	Texture::s_Logger = &m_Logger;
	Sound::s_Logger = &m_Logger;

	m_DebugCamera = m_Window.GetDefaultCamera();
}

Sprite* Engine::GetSprite(unsigned int ID)
{
	if (Sprite::s_Sprites.find(ID) != Sprite::s_Sprites.end()) return Sprite::s_Sprites.at(ID);
	else return nullptr;
}

bool Engine::Update()
{
	sf::Event event;
	while (m_Window.GetEvent(event))
	{
		if (event.type == sf::Event::Closed)			m_Window.Close();
		if (event.type == sf::Event::Resized)
		{
			// Set min size of 800x600
			if (event.size.width < 800) m_Window.SetSize(800, m_Window.GetHeight());
			if (event.size.height < 600) m_Window.SetSize(m_Window.GetWidth(), 600);
		}
	}

	m_Window.SetCamera(m_Camera);
	m_DebugCamera = m_Window.GetDefaultCamera();

	return !m_Window.ShouldClose();
}

void Engine::SetGravity(float g)
{
	Sprite::s_PhysicsWorld.SetGravity(b2Vec2(0, -g));
	m_Gravity = g;
}

int Engine::OnLuaPrint(lua_State* L)
{
	std::string message;
	int nargs = lua_gettop(L);

	for (int i = 1; i <= nargs; i++) // Can't call log because of my fancy macro, ha ha
	{
		if (lua_tostring(L, i) != NULL) message += std::string(lua_tostring(L, i)) + " ";
		else if (lua_isboolean(L, i)) message += lua_toboolean(L, i) ? "true " : "false "; 
		else message += "NULL";
	}

	Engine::Get().m_Logger.m_Messages.push_back(message);

	return 0;
}

int Engine::OnLuaError(lua_State* L)
{
	std::string message;
	int nargs = lua_gettop(L);

	for (int i = 1; i <= nargs; i++) // Can't call log because of my fancy macro, ha ha
		message += std::string(lua_tostring(L, i)) + " ";

	Engine::Get().m_Logger.m_Messages.push_back(message);

	return 0;
}

void Engine::BeginFrame()
{

	// Begin dela-time calclation
	m_Timer.BeginFrame();

	// Update components
	for (auto&[id, sprite] : Sprite::s_Sprites)
		for (Component* c : sprite->m_Components)
		{
			if (sprite != nullptr && !sprite->bDelete && !sprite->m_bDisabled) c->OnUpdate();
		}

	// Clean up from Lua
	for (auto it = Sprite::s_Sprites.begin(); it != Sprite::s_Sprites.end();) 
	{
		if (it->second == nullptr) continue;
		if (it->second->bDelete)
		{
			Sprite* s = it->second;
			//std::cout << std::to_string(s->m_Position.x) << std::endl;
			it = Sprite::s_Sprites.erase(it);
			delete s;
		}
		else it++;
	}


	// Render game
	m_Window.Clear(sf::Color((uint8_t)m_BackgroundR, (uint8_t)m_BackgroundG, (uint8_t)m_BackgroundB));
}

void Engine::EndFrame()
{
	// Do camera
	m_Window.SetCamera(m_Camera);

	// Do physics
#ifdef DEBUG
	m_Timer.BeginTimer();
#endif
	constexpr float physicsSpeed = 1.0f / 60.0f;
	for (auto&[id, sprite] : Sprite::s_Sprites)
	{
		if (sprite->bDelete || sprite->m_bDisabled) continue;
		if (sprite->m_PhysicsBody != nullptr) sprite->m_PhysicsBody->SetAwake(true); // Set everything to awake to fix getting stuck!
		if (sprite->m_PhysicsBody != nullptr && (sprite->lastPhysicsPosition.x != sprite->m_Position.x || sprite->lastPhysicsPosition.y != sprite->m_Position.y))
		{
			sprite->m_PhysicsBody->SetTransform(b2Vec2(sprite->m_Position.x * PHYSICS_SCALE, sprite->m_Position.y * PHYSICS_SCALE), sprite->m_PhysicsBody->GetAngle());
		}
		sprite->lastPhysicsPosition = sprite->m_Position;
	}
	Sprite::s_PhysicsWorld.Step(physicsSpeed, 6, 2);
	for (auto&[id, sprite] : Sprite::s_Sprites)
	{
		if (sprite->m_PhysicsBody != nullptr && !sprite->m_bDisabled) sprite->m_Position = { sprite->m_PhysicsBody->GetPosition().x * PHYSICS_SCALE_INV, sprite->m_PhysicsBody->GetPosition().y * PHYSICS_SCALE_INV };
	}

#ifdef DEBUG
	float fPhysicsTime = m_Timer.EndTimer();
#endif

	auto RenderSprite = [&](Sprite* sprite)
	{
		if (sprite->m_Layer < 0 || sprite->m_bDisabled) return;
		sprite->m_Sprite.setPosition(sprite->m_Position.x, -sprite->m_Position.y);
		sprite->m_Sprite.setScale(sprite->m_Scale.x, sprite->m_Scale.y);
		m_Window.Draw(sprite->m_Sprite);
	};

	// Get the highest layer of sprites
	int highestLayer = 0;
	for (auto&[id, sprite] : Sprite::s_Sprites)
		if (sprite->m_Layer > highestLayer) highestLayer = sprite->m_Layer;

	// Render each sprite layer
	for (int i = 0; i <= highestLayer; ++i)
	{
		for (auto&[id, sprite] : Sprite::s_Sprites)
			if (sprite->m_Layer == i) RenderSprite(sprite);
	}

	// If in debug mode, display FPS and draw bounding boxes
#ifdef DEBUG
	m_Window.SetCamera(m_DebugCamera);
	m_Debugger.Draw(m_Window, m_Timer.m_fDelta, fPhysicsTime, m_BackgroundR == 255, m_Camera.position);
	m_Window.SetCamera(m_Camera);
	
	for (auto&[id, sprite] : Sprite::s_Sprites)
	{
		if (sprite->m_bDrawCollider)
		{
			if (sprite->m_PhysicsBody != nullptr && sprite->m_PhysicsBody->GetFixtureList()->GetType() == b2Shape::e_circle) {}
			else Engine::Get().m_Window.DrawBoundingBox(sprite->m_Position * Vec2(1.0f, -1.0), Vec2((float)sprite->m_Width, (float)sprite->m_Height));;
		}
	}
#endif

	// Swap buffers
	m_Window.Display();

	// End delta-time calculation
	m_Timer.EndFrame();
}

Engine::~Engine() 
{
	Texture::CleanUp();
	for (Sprite* sprite : m_luaSprites) delete sprite;
}
