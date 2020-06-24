#include "LuaComponent.h"

#include "Engine.h"

LuaComponent* LuaComponent::s_CurrentInstance;

LuaComponent::LuaComponent(const std::string& fileName) : m_Lua(Engine::Get().m_Logger, Engine::OnLuaPrint, Engine::OnLuaError)
{
	RegisterFunctions();

	// Load script into state
	m_Lua.ExecuteFile(fileName);
	s_CurrentInstance = this;
	m_Lua.CallGlobalFunction("OnCreate");
}

LuaComponent::LuaComponent(const std::string& fileName, const std::string& variable, int number) : m_Lua(Engine::Get().m_Logger, Engine::OnLuaPrint, Engine::OnLuaError)
{
	RegisterFunctions();

	// Set variable
	m_Lua.SetGlobalNumber(variable, (float) number);

	// Load script into state
	m_Lua.ExecuteFile(fileName);
	s_CurrentInstance = this;
	m_Lua.CallGlobalFunction("OnCreate");
}

void LuaComponent::RegisterFunctions()
{
	// Init Lua functions
	m_Lua.SetGlobalFunction("CreateSprite",	lua_CreateSprite);
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
	m_Lua.SetGlobalFunction("AddSpherePhysics",	lua_AddSpherePhysics);
	m_Lua.SetGlobalFunction("RemovePhysics",lua_RemovePhysics);
	m_Lua.SetGlobalFunction("GetPhysics",	lua_GetPhysics);
	m_Lua.SetGlobalFunction("AddForce",		lua_AddForce);
	m_Lua.SetGlobalFunction("SetDensity",	lua_SetDensity);
	m_Lua.SetGlobalFunction("GetDensity",	lua_GetDensity);
	m_Lua.SetGlobalFunction("SetVelocityX", lua_SetVelocityX);
	m_Lua.SetGlobalFunction("GetVelocityX", lua_GetVelocityX);
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
	m_Lua.SetGlobalFunction("Delete",		lua_Delete);
	m_Lua.SetGlobalFunction("AddScript",	lua_AddScript);
	m_Lua.SetGlobalFunction("GetMouseDown", lua_GetMouseDown);
	m_Lua.SetGlobalFunction("GetMouseX",	lua_GetMouseX);
	m_Lua.SetGlobalFunction("GetMouseY",	lua_GetMouseY);
	m_Lua.SetGlobalFunction("SetTrigger",	lua_SetTrigger);
	m_Lua.SetGlobalFunction("IsTriggered",	lua_IsTriggered);
}

void LuaComponent::OnUpdate()
{
	s_CurrentInstance = this;
	m_Lua.CallGlobalFunction("OnUpdate", []() { s_CurrentInstance->m_Lua.PushNumber(Engine::Get().m_Timer.m_fDelta); }, 1, 0);
}

int LuaComponent::lua_CreateSprite(lua_State* L)
{
	if (lua_gettop(L) != 1 && lua_gettop(L) != 0) { Log("Invalid number of arguments in function CreateSprite"); return 0; }

	if (lua_gettop(L) == 0)
	{
		Engine::Get().m_luaSprites.emplace_back(new Sprite());
		LuaComponent::s_CurrentInstance->m_Lua.PushNumber((int)Engine::Get().m_luaSprites.back()->m_ID);
		return 1;
	}

	// Retrieve arguments
	const std::string fileName = LuaComponent::s_CurrentInstance->m_Lua.GetString(1);
	Engine::Get().m_luaSprites.emplace_back(new Sprite(fileName));

	// Send back arguments
	LuaComponent::s_CurrentInstance->m_Lua.PushNumber((int)Engine::Get().m_luaSprites.back()->m_ID);

	return 1;
}

int LuaComponent::lua_GetKeyDown(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetKeyDown"); return 0; }

	sf::Keyboard::Key key = (sf::Keyboard::Key)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	LuaComponent::s_CurrentInstance->m_Lua.PushBool(Engine::Get().GetKeyDown(key));

	return 1;
}

int LuaComponent::lua_AddX(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function AddX"); return 0; }

	// If camera
	if (LuaComponent::s_CurrentInstance->m_Lua.GetString(1) == "camera")
	{
		float change = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
		Engine::Get().m_Camera.position.x += change;
		return 0;
	}

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	float change = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Position.x += change;

	return 0;
}

int LuaComponent::lua_AddY(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function AddY"); return 0; }

	// If camera
	if (LuaComponent::s_CurrentInstance->m_Lua.GetString(1) == "camera")
	{
		float change = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
		Engine::Get().m_Camera.position.y += change;
		return 0;
	}


	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	float change = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Position.y += change;

	return 0;
}

int LuaComponent::lua_GetX(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetX"); return 0; }

	// If camera
	if (LuaComponent::s_CurrentInstance->m_Lua.GetString(1) == "camera")
	{
		LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Engine::Get().m_Camera.position.x);
		return 1;
	}

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_Position.x);
	else LuaComponent::s_CurrentInstance->m_Lua.PushNumber(-1);

	return 1;
}

int LuaComponent::lua_GetY(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetY"); return 0; }

	// If camera
	if (LuaComponent::s_CurrentInstance->m_Lua.GetString(1) == "camera")
	{
		LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Engine::Get().m_Camera.position.y);
		return 1;
	}

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_Position.y);
	else LuaComponent::s_CurrentInstance->m_Lua.PushNumber(-1);

	return 1;
}

int LuaComponent::lua_SetX(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetX"); return 0; }

	// If camera
	if (LuaComponent::s_CurrentInstance->m_Lua.GetString(1) == "camera")
	{
		float pos = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
		Engine::Get().m_Camera.position.x = pos;
		return 1;
	}

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	float newPos = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Position.x = newPos;

	return 1;
}

int LuaComponent::lua_SetY(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetY"); return 0; }

	// If camera
	if (LuaComponent::s_CurrentInstance->m_Lua.GetString(1) == "camera")
	{
		float pos = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
		Engine::Get().m_Camera.position.y = pos;
		return 1;
	}

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	float newPos = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Position.y = newPos;

	return 1;
}


int LuaComponent::lua_SetGravity(lua_State* L)
{
	if (lua_gettop(L) != 1 && lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetGravity"); return 0; }
	if (lua_gettop(L) == 1) Engine::Get().SetGravity(LuaComponent::s_CurrentInstance->m_Lua.GetFloat(1));
	else
	{
		float g = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
		unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
		if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr) Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->SetGravityScale(g);
	}
	return 0;
}

int LuaComponent::lua_GetGravity(lua_State* L)
{
	if (lua_gettop(L) != 0) { Log("Invalid number of arguments in function GetGravity"); return 0; }
	LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Engine::Get().m_Gravity);
	return 1;
}

int LuaComponent::lua_AddPhysics(lua_State* L)
{
	if (lua_gettop(L) < 2) { Log("Invalid number of arguments in function SetPhysics"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	bool dynamic = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetBool(2);

	if (lua_gettop(L) != 5 && lua_gettop(L) != 3 && dynamic) { Log("Invalid number of arguments in function SetPhysics"); return 0; }

	if (Sprite::s_Sprites.find(spriteID) == Sprite::s_Sprites.end()) return 0;

	if (dynamic)
	{
		float density = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(3);
		if (lua_gettop(L) == 5) Sprite::s_Sprites.at(spriteID)->AddDynamicPhysics(density, LuaComponent::s_CurrentInstance->m_Lua.GetFloat(4), LuaComponent::s_CurrentInstance->m_Lua.GetFloat(5));
		else Sprite::s_Sprites.at(spriteID)->AddDynamicPhysics(density);
	}
	else
	{
		if (lua_gettop(L) == 5) Sprite::s_Sprites.at(spriteID)->AddStaticPhysics(LuaComponent::s_CurrentInstance->m_Lua.GetFloat(4), LuaComponent::s_CurrentInstance->m_Lua.GetFloat(5));
		else Sprite::s_Sprites.at(spriteID)->AddStaticPhysics();
	}

	return 0;
}

int LuaComponent::lua_AddSpherePhysics(lua_State* L)
{
	if (lua_gettop(L) < 2) { Log("Invalid number of arguments in function SetPhysics"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	bool dynamic = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetBool(2);

	if (lua_gettop(L) != 4 && lua_gettop(L) != 3 && dynamic) { Log("Invalid number of arguments in function SetPhysics"); return 0; }

	if (Sprite::s_Sprites.find(spriteID) == Sprite::s_Sprites.end()) return 0;

	if (dynamic)
	{
		float density = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(3);
		if (lua_gettop(L) == 4) Sprite::s_Sprites.at(spriteID)->AddDynamicPhysicsSphere(density, LuaComponent::s_CurrentInstance->m_Lua.GetFloat(4));
		else Sprite::s_Sprites.at(spriteID)->AddDynamicPhysics(density);
	}
	else
	{
		if (lua_gettop(L) == 4) Sprite::s_Sprites.at(spriteID)->AddStaticPhysicsSphere(LuaComponent::s_CurrentInstance->m_Lua.GetFloat(4));
		else Sprite::s_Sprites.at(spriteID)->AddStaticPhysics();
	}

	return 0;
}

int LuaComponent::lua_RemovePhysics(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function SetPhysics"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->RemovePhysics();

	return 0;
}

int LuaComponent::lua_GetPhysics(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetPhysics"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) LuaComponent::s_CurrentInstance->m_Lua.PushBool(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr);
	else LuaComponent::s_CurrentInstance->m_Lua.PushNumber(-1);

	return 1;
}

int LuaComponent::lua_AddForce(lua_State* L)
{
	if (lua_gettop(L) != 3) { Log("Invalid number of arguments in function AddForce"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	float forceX = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
	float forceY = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(3);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		float originX = Sprite::s_Sprites.at(spriteID)->m_Position.x;
		float originY = Sprite::s_Sprites.at(spriteID)->m_Position.y;
		Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->ApplyForce(b2Vec2(forceX, forceY), b2Vec2(originX, originY), true);
	}

	return 0;
}

int LuaComponent::lua_SetDensity(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetDensity"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		float density = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(1);
		Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetFixtureList()->SetDensity(density);
	}

	return 0;
}

int LuaComponent::lua_GetDensity(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function SetDensity"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetFixtureList()->GetDensity());
	}
	else LuaComponent::s_CurrentInstance->m_Lua.PushNumber(-1.0f);

	return 1;
}

int LuaComponent::lua_GetVelocityX(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetVelocityX"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetLinearVelocity().x);
	}
	else LuaComponent::s_CurrentInstance->m_Lua.PushNumber(-1.0f);

	return 1;
}

int LuaComponent::lua_GetVelocityY(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetVelocityY"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetLinearVelocity().y);
	}
	else LuaComponent::s_CurrentInstance->m_Lua.PushNumber(-1.0f);

	return 1;
}

int LuaComponent::lua_SetVelocityX(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetVelocityX"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	float number = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->SetLinearVelocity(b2Vec2(number, Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetLinearVelocity().y));
	}

	return 0;
}

int LuaComponent::lua_SetVelocityY(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetVelocityY"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	float number = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->SetLinearVelocity(b2Vec2(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetLinearVelocity().x, number));
	}

	return 0;
}

int LuaComponent::lua_GetLayer(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetLayer"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_Layer);
	else LuaComponent::s_CurrentInstance->m_Lua.PushNumber(-1);

	return 1;
}

int LuaComponent::lua_SetLayer(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetLayer"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	int layer = LuaComponent::s_CurrentInstance->m_Lua.GetInt(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_Layer = layer;

	return 0;
}

int LuaComponent::lua_LoadTexture(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function LoadTexture"); return 0; }
	Texture::GetTexture(LuaComponent::s_CurrentInstance->m_Lua.GetString(1));

	return 0;
}

int LuaComponent::lua_SetTexture(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetTexture"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	std::string fileName = LuaComponent::s_CurrentInstance->m_Lua.GetString(2);
	if (lua_gettop(L) == 2 && Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end())
				Sprite::s_Sprites.at(spriteID)->SetTexture(Texture::GetTexture(fileName)->m_Texture);

	return 0;
}

int LuaComponent::lua_GetTexture(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetTexture"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	std::string returnValue = std::to_string(spriteID);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end())
	{
		// Go through each texture until the right one is found
		sf::Texture* texture = Sprite::s_Sprites.at(spriteID)->m_Texture;
		for (auto&[key, value] : Texture::s_Textures)
			if (value->m_Texture == texture)
			{
				returnValue = key;
			}
	}

	LuaComponent::s_CurrentInstance->m_Lua.PushString(returnValue);
	return 1;
}

int LuaComponent::lua_SetFriction(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function SetFriction"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	float friction = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetFixtureList()->SetFriction(friction);

	return 0;
}

int LuaComponent::lua_GetFriction(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetFriction"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end()) LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetFixtureList()->GetFriction());
	else LuaComponent::s_CurrentInstance->m_Lua.PushNumber(-1);

	return 1;
}

int LuaComponent::lua_RayCast(lua_State* L)
{
	if (lua_gettop(L) != 4) { Log("Invalid number of arguments in function RayCast"); return 0; }

	float x = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(1);
	float y = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(2);
	float dirX = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(3);
	float dirY = LuaComponent::s_CurrentInstance->m_Lua.GetFloat(4);

	LuaComponent::s_CurrentInstance->m_Lua.PushBool(DoRaycast({ x, y }, { dirX, dirY }));

	return 1;
}

int LuaComponent::lua_Delete(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function Delete"); return 0; }

	int spriteID = LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);

	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end())
	{
		// Find sprite in lua sprites first, then delete in other areas
		for (unsigned int i = 0; i < Engine::Get().m_luaSprites.size(); ++i)
			if (Engine::Get().m_luaSprites[i]->m_ID == (unsigned int)spriteID)
				Engine::Get().m_luaSprites.erase(Engine::Get().m_luaSprites.begin() + i);
		
		Sprite::s_Sprites[spriteID]->bDelete = true;
		//Sprite::s_Sprites.erase(spriteID);
		//delete Sprite::s_Sprites.at(spriteID);
	}

	return 0;
}

int LuaComponent::lua_AddScript(lua_State* L)
{
	if (lua_gettop(L) != 2) { Log("Invalid number of arguments in function AddScript"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	std::string script = LuaComponent::s_CurrentInstance->m_Lua.GetString(2);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end())
	{
		LuaComponent* c = new LuaComponent(script, "sprite", spriteID);
		Sprite::s_Sprites.at(spriteID)->AddComponent(c);
	}

	return 0;
}

int LuaComponent::lua_GetMouseDown(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function GetMouseDown"); return 0; }

	int button = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	LuaComponent::s_CurrentInstance->m_Lua.PushBool(Engine::Get().GetMouseDown((sf::Mouse::Button)button));
	return 1;
}

int LuaComponent::lua_GetMouseX(lua_State* L)
{
	if (lua_gettop(L) != 0) { Log("Invalid number of arguments in function GetMouseX"); return 0; }

	LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Engine::Get().m_Window.GetMousePosition().x);
	return 1;
}

int LuaComponent::lua_GetMouseY(lua_State* L)
{
	if (lua_gettop(L) != 0) { Log("Invalid number of arguments in function GetMouseY"); return 0; }

	LuaComponent::s_CurrentInstance->m_Lua.PushNumber(Engine::Get().m_Window.GetMousePosition().y);
	return 1;
}

int LuaComponent::lua_SetTrigger(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function SetTrigger"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end())
	{
		Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetFixtureList()->SetSensor(true);
	}

	return 0;
}

int LuaComponent::lua_IsTriggered(lua_State* L)
{
	if (lua_gettop(L) != 1) { Log("Invalid number of arguments in function IsTriggered"); return 0; }

	unsigned int spriteID = (unsigned int)LuaComponent::s_CurrentInstance->m_Lua.GetInt(1);
	bool bCollided = false;
	if (Sprite::s_Sprites.find(spriteID) != Sprite::s_Sprites.end() && Sprite::s_Sprites.at(spriteID)->m_PhysicsBody != nullptr)
	{
		b2ContactEdge* c = Sprite::s_Sprites.at(spriteID)->m_PhysicsBody->GetContactList();
		if (c != nullptr && c->contact->IsTouching())
		{
			c->contact->GetFixtureA()->GetUserData()->
			bCollided = true;
		}
	}
	LuaComponent::s_CurrentInstance->m_Lua.PushBool(bCollided);

	return 1;
}