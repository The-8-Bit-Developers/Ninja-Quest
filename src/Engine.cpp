#include "Engine.h"
#include  <iostream>

#include "Raycast.h"

#if DEBUG
Engine::Engine() : m_Debugger(m_Logger), m_Lua(m_Logger, OnLuaPrint, OnLuaError) {}
#else
Engine::Engine() : m_Lua(m_Logger, OnLuaPrint, OnLuaError) {}
#endif

void Engine::Create(const std::string& windowName, const int windowWidth, const int windowHeight)
{
	m_Window.Create(windowName, windowWidth, windowHeight);
	Sprite::logger = &m_Logger;
	Texture::s_Logger = &m_Logger;

	m_DebugCamera = m_Window.GetDefaultCamera();

	// Register Lua functions
	m_Lua.SetGlobalFunction("CreateSprite", lua_CreateSprite);
	m_Lua.SetGlobalFunction("GetKeyDown",	lua_GetKeyDown);
	m_Lua.SetGlobalFunction("AddX",			lua_AddX);
	m_Lua.SetGlobalFunction("AddY",			lua_AddY);
	m_Lua.SetGlobalFunction("GetX",			lua_GetX);
	m_Lua.SetGlobalFunction("GetY",			lua_GetY);
	m_Lua.SetGlobalFunction("SetX",			lua_SetX);
	m_Lua.SetGlobalFunction("SetY",			lua_SetY);
	m_Lua.SetGlobalFunction("SetGravity",	lua_SetGravity);
	m_Lua.SetGlobalFunction("GetGravity",	lua_GetGravity);
	m_Lua.SetGlobalFunction("AddPhysics",	lua_AddPhysics);
	m_Lua.SetGlobalFunction("RemovePhysics",lua_RemovePhysics);
	m_Lua.SetGlobalFunction("GetPhysics",	lua_GetPhysics);
	m_Lua.SetGlobalFunction("AddForce",		lua_AddForce);
	m_Lua.SetGlobalFunction("SetDensity",	lua_SetDensity);
	m_Lua.SetGlobalFunction("GetDensity",	lua_GetDensity);
	m_Lua.SetGlobalFunction("SetVelocityX",	lua_SetVelocityX);
	m_Lua.SetGlobalFunction("GetVelocityX",	lua_GetVelocityX);
	m_Lua.SetGlobalFunction("SetVelocityY", lua_SetVelocityY);
	m_Lua.SetGlobalFunction("GetVelocityY", lua_GetVelocityY);
	m_Lua.SetGlobalFunction("SetLayer",		lua_SetLayer);
	m_Lua.SetGlobalFunction("GetLayer",		lua_GetLayer);
	m_Lua.SetGlobalFunction("SetTexture",	lua_SetTexture);
	m_Lua.SetGlobalFunction("GetTexture",	lua_GetTexture);
	m_Lua.SetGlobalFunction("LoadTexture",	lua_LoadTexture);
	m_Lua.SetGlobalFunction("GetFriction",	lua_GetFriction);
	m_Lua.SetGlobalFunction("SetFriction",	lua_SetFriction);
	m_Lua.SetGlobalFunction("RayCast",		lua_RayCast);
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
		if (event.type == sf::Event::Closed) m_Window.Close();
	}

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
		else if (lua_toboolean(L, i) != NULL) message += lua_toboolean(L, i) ? "true " : "false "; 
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

	// Render game
	m_Window.Clear(sf::Color::Black);
}

#include <thread>
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
		if (sprite->m_PhysicsBody != nullptr && (sprite->lastPhysicsPosition.x != sprite->m_Position.x || sprite->lastPhysicsPosition.y != sprite->m_Position.y))
		{
			sprite->m_PhysicsBody->SetTransform(b2Vec2(sprite->m_Position.x, sprite->m_Position.y), sprite->m_PhysicsBody->GetAngle());
			sprite->m_PhysicsBody->SetAwake(true);
		}
		sprite->lastPhysicsPosition = sprite->m_Position;
	}
	Sprite::s_PhysicsWorld.Step(physicsSpeed, 6, 2);
	for (auto&[id, sprite] : Sprite::s_Sprites)
	{
		if (sprite->m_PhysicsBody != nullptr) sprite->m_Position = { sprite->m_PhysicsBody->GetPosition().x, sprite->m_PhysicsBody->GetPosition().y };
	}

#ifdef DEBUG
	float fPhysicsTime = m_Timer.EndTimer();
#endif]

	auto RenderSprite = [&](Sprite* sprite)
	{
		if (sprite->m_Layer < 0) return;
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
	m_Debugger.Draw(m_Window, m_Timer.m_fDelta, fPhysicsTime);
	m_Window.SetCamera(m_Camera);
	
	for (auto&[id, sprite] : Sprite::s_Sprites)
	{
		if (sprite->m_bDrawCollider)
			Engine::Get().m_Window.DrawBoundingBox(sprite->m_Position * Vec2(1.0f, -1.0), Vec2((float)sprite->m_Width, (float)sprite->m_Height));
	}
#endif

	// Swap buffers
	m_Window.Display();

	// End delta-time calculation
	m_Timer.EndFrame();
}

int Engine::lua_CreateSprite(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function CreateSprite"); return 0; }

	// Retrieve arguments
	const std::string fileName = Engine::Get().m_Lua.GetString(1);
	Engine::Get().m_luaSprites.emplace_back(new Sprite(fileName));

	// Send back arguments
	Engine::Get().m_Lua.PushNumber((int)Engine::Get().m_luaSprites.back()->m_ID);

	return 1;
}

int Engine::lua_GetKeyDown(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetKeyDown"); return 0; }
	
	sf::Keyboard::Key key = (sf::Keyboard::Key)Engine::Get().m_Lua.GetInt(1);
	Engine::Get().m_Lua.PushBool(sf::Keyboard::isKeyPressed(key));

	return 1;
}

int Engine::lua_AddX(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function AddX"); return 0; }

	// If camera
	if (Engine::Get().m_Lua.GetString(1) == "camera")
	{
		float change = Engine::Get().m_Lua.GetFloat(2);
		Engine::Get().m_Camera.position.x += change;
		return 0;
	}

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	float change = Engine::Get().m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Position.x += change;

	return 0;
}

int Engine::lua_AddY(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function AddY"); return 0; }

	// If camera
	if (Engine::Get().m_Lua.GetString(1) == "camera")
	{
		float change = Engine::Get().m_Lua.GetFloat(2);
		Engine::Get().m_Camera.position.y += change;
		return 0;
	}


	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	float change = Engine::Get().m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Position.y += change;

	return 0;
}

int Engine::lua_GetX(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetX"); return 0; }

	// If camera
	if (Engine::Get().m_Lua.GetString(1) == "camera")
	{
		Engine::Get().m_Lua.PushNumber(Engine::Get().m_Camera.position.x);
		return 0;
	}

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Engine::Get().m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_Position.x);
	else Engine::Get().m_Lua.PushNumber(-1);

	return 1;
}

int Engine::lua_GetY(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetY"); return 0; }

	// If camera
	if (Engine::Get().m_Lua.GetString(1) == "camera")
	{
		Engine::Get().m_Lua.PushNumber(Engine::Get().m_Camera.position.y);
		return 0;
	}

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Engine::Get().m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_Position.y);
	else Engine::Get().m_Lua.PushNumber(-1);

	return 1;
}

int Engine::lua_SetX(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetX"); return 0; }

	// If camera
	if (Engine::Get().m_Lua.GetString(1) == "camera")
	{
		float pos = Engine::Get().m_Lua.GetFloat(2);
		Engine::Get().m_Camera.position.x = pos;
		return 0;
	}

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	float newPos = Engine::Get().m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Position.x = newPos;

	return 1;
}

int Engine::lua_SetY(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetY"); return 0; }

	// If camera
	if (Engine::Get().m_Lua.GetString(1) == "camera")
	{
		float pos = Engine::Get().m_Lua.GetFloat(2);
		Engine::Get().m_Camera.position.y = pos;
		return 0;
	}

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	float newPos = Engine::Get().m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Position.y = newPos;

	return 1;
}


int Engine::lua_SetGravity(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function SetGravity"); return 0; }
	Engine::Get().SetGravity(Engine::Get().m_Lua.GetFloat(1));
	return 0;
}

int Engine::lua_GetGravity(lua_State* L)
{
	if (lua_gettop(L) != 0) { Log("Invalid number of arguments in function GetGravity"); return 0; }
	Engine::Get().m_Lua.PushNumber(Engine::Get().m_Gravity);
	return 1;
}

int Engine::lua_AddPhysics(lua_State* L)
{
	if (lua_gettop(L) < 2) { Log("Invalid number of arguments in function SetPhysics"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	bool dynamic = (unsigned int)Engine::Get().m_Lua.GetBool(2);

	if (lua_gettop(L) != 5 && lua_gettop(L) != 3 && dynamic) { Log("Invalid number of arguments in function SetPhysics"); return 0; }

	if (Sprite::s_Sprites.find(spriteID) == Sprite::s_Sprites.end()) return 0;

	if (dynamic)
	{
		float density = Engine::Get().m_Lua.GetFloat(3);
		if (lua_gettop(L) == 5) Sprite::s_Sprites.at(spriteID)->AddDynamicPhysics(density, Engine::Get().m_Lua.GetFloat(4), Engine::Get().m_Lua.GetFloat(5));
		else Sprite::s_Sprites.at(spriteID)->AddDynamicPhysics(density);
	}
	else
	{
		if (lua_gettop(L) == 5) Sprite::s_Sprites.at(spriteID)->AddStaticPhysics(Engine::Get().m_Lua.GetFloat(4), Engine::Get().m_Lua.GetFloat(5));
		Sprite::s_Sprites.at(spriteID)->AddStaticPhysics();
	}

	return 0;
}

int Engine::lua_RemovePhysics(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function SetPhysics"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->RemovePhysics();

	return 0;
}

int Engine::lua_GetPhysics(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetPhysics"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Engine::Get().m_Lua.PushBool(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr);
	else Engine::Get().m_Lua.PushNumber(-1);

	return 1;
}

int Engine::lua_AddForce(lua_State* L)
{
	if (lua_gettop(L) != 3) { Log("Invalid number of arguments in function AddForce"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	float forceX = Engine::Get().m_Lua.GetFloat(2);
	float forceY = Engine::Get().m_Lua.GetFloat(3);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		float originX = Sprite::s_Sprites.at(spriteID)->m_Position.x;
		float originY = Sprite::s_Sprites.at(spriteID)->m_Position.y;
		Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->ApplyForce(b2Vec2(forceX, forceY), b2Vec2(originX, originY), true);
	}

	return 0;
}

int Engine::lua_SetDensity(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetDensity"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		float density = Engine::Get().m_Lua.GetFloat(1);
		Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetFixtureList()->SetDensity(density);
	}

	return 0;
}

int Engine::lua_GetDensity(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function SetDensity"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		Engine::Get().m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetFixtureList()->GetDensity());
	}
	else Engine::Get().m_Lua.PushNumber(-1.0f);

	return 1;
}

int Engine::lua_GetVelocityX(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetVelocityX"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		Engine::Get().m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetLinearVelocity().x);
	}
	else Engine::Get().m_Lua.PushNumber(-1.0f);

	return 1;
}

int Engine::lua_GetVelocityY(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetVelocityY"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		Engine::Get().m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetLinearVelocity().y);
	}
	else Engine::Get().m_Lua.PushNumber(-1.0f);

	return 1;
}

int Engine::lua_SetVelocityX(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetVelocityX"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	float number = Engine::Get().m_Lua.GetFloat(2);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->SetLinearVelocity(b2Vec2(number, Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetLinearVelocity().y));
	}

	return 0;
}

int Engine::lua_SetVelocityY(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetVelocityY"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	float number = Engine::Get().m_Lua.GetFloat(2);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->SetLinearVelocity(b2Vec2(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetLinearVelocity().x, number));
	}

	return 0;
}

int Engine::lua_GetLayer(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetLayer"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Engine::Get().m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_Layer);
	else Engine::Get().m_Lua.PushNumber(-1);

	return 1;
}

int Engine::lua_SetLayer(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetLayer"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	int layer = Engine::Get().m_Lua.GetInt(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Layer = layer;

	return 0;
}

int Engine::lua_LoadTexture(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function LoadTexture"); return 0; }
	Texture::GetTexture(Engine::Get().m_Lua.GetString(1));

	return 0;
}

int Engine::lua_SetTexture(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetTexture"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	std::string fileName = Engine::Get().m_Lua.GetString(2);
	if (lua_gettop(L) == 2 && Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end())
		if (Texture::s_Textures.find(fileName) != Texture::s_Textures.end())
			if (Texture::s_Textures.at(fileName)->m_Texture != nullptr)
				Sprite::s_Sprites.at(spriteID)->SetTexture(Texture::GetTexture(fileName)->m_Texture);

	return 0;
}

int Engine::lua_GetTexture(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetTexture"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	std::string returnValue = std::to_string(spriteID);
	
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end())
	{
		// Go through each texture until the right one is found
		sf::Texture* texture = Sprite::s_Sprites.at(spriteID)->m_Texture;
		for (auto& [key, value] : Texture::s_Textures)
			if (value->m_Texture == texture)
			{
				returnValue = key;
			}	
	}
	
	Engine::Get().m_Lua.PushString(returnValue);
	return 1;
}

int Engine::lua_SetFriction(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetFriction"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	float friction = Engine::Get().m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetFixtureList()->SetFriction(friction);

	return 0;
}

int Engine::lua_GetFriction(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetFriction"); return 0; }

	unsigned int spriteID = (unsigned int)Engine::Get().m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Engine::Get().m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetFixtureList()->GetFriction());
	else Engine::Get().m_Lua.PushNumber(-1);

	return 1;
}

int Engine::lua_RayCast(lua_State* L)
{
	if (lua_gettop(L) != 4) { Log("Invalid number of arguments in function RayCast"); return 0; }

	float x = Engine::Get().m_Lua.GetFloat(1);
	float y = Engine::Get().m_Lua.GetFloat(2);
	float dirX = Engine::Get().m_Lua.GetFloat(3);
	float dirY = Engine::Get().m_Lua.GetFloat(4);
	
	Engine::Get().m_Lua.PushBool(DoRaycast({ x, y }, { dirX, dirY }));

	return 1;
}

Engine::~Engine() 
{
	Texture::CleanUp();
	for (Sprite* sprite : m_luaSprites) delete sprite;
}
