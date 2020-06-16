#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>

#include "Logger.h"

class Texture
{

public:
	sf::Texture* m_Texture;
	Texture(const std::string& fileName);
	Texture() {};

	static std::unordered_map<std::string, Texture*> s_Textures;
	static Logger* s_Logger;
	static Texture* GetTexture(const std::string& string);
	static void CleanUp();

};

#endif