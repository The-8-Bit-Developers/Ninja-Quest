#include "Engine.h"
#include  <iostream>

#if DEBUG
Engine::Engine() : m_Debugger(m_Logger), m_Lua(m_Logger, OnLuaPrint, OnLuaError) {}
#else
Engine::Engine() : m_Lua(m_Logger, OnLuaPrint, OnLuaError) {}
#endif

void Engine::Create(const std::string& windowName, const int windowWidth, const int windowHeight)
{
	m_Window.Create(windowName, windowWidth, windowHeight);
	Sprite::logger = &m_Logger;

	float aspect = (float)windowHeight / (float)windowWidth;
	m_DebugCamera.size = Vec2((float)windowWidth, (float)windowHeight);
	m_DebugCamera.position = Vec2((float)(windowWidth / 2), (float)(windowHeight / (2 + aspect)));

	// Register Lua functions
	m_Lua.SetGlobalFunction("CreateSprite", lua_CreateSprite);
	m_Lua.SetGlobalFunction("GetKeyDown",	lua_GetKeyDown);
	m_Lua.SetGlobalFunction("AddX",			lua_AddX);
	m_Lua.SetGlobalFunction("AddY",			lua_AddY);
	m_Lua.SetGlobalFunction("GetX",			lua_GetX);
	m_Lua.SetGlobalFunction("GetY",			lua_GetY);
	m_Lua.SetGlobalFunction("SetX",			lua_SetX);
	m_Lua.SetGlobalFunction("SetY",			lua_SetY);
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

int Engine::OnLuaPrint(lua_State* L)
{
	std::string message;
	int nargs = lua_gettop(L);

	for (int i = 1; i <= nargs; i++) // Can't call log because of my fancy macro, ha ha
	{
		if (lua_tostring(L, i) != NULL) message += std::string(lua_tostring(L, i)) + " ";
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

void Engine::EndFrame()
{
	// Do camera
	m_Window.SetCamera(m_Camera);

	// Render sprites
	for (auto& [id, sprite] : Sprite::s_Sprites)
	{		
		sprite->m_Sprite.setPosition(sprite->m_Position.x, sprite->m_Position.y);
		sprite->m_Sprite.setScale(sprite->m_Scale.x, sprite->m_Scale.y);
		m_Window.Draw(sprite->m_Sprite);
	}

	// If in debug mode, display FPS
#ifdef DEBUG
	m_Window.SetCamera(m_DebugCamera);
	m_Debugger.Draw(m_Window, m_Timer.m_fDelta);
	m_Window.SetCamera(m_Camera);
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


Engine::~Engine() 
{
	for (Sprite* sprite : m_luaSprites) delete sprite;
}
