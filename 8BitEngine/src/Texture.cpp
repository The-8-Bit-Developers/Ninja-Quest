#include "Texture.h"

std::unordered_map<std::string, Texture*> Texture::s_Textures;
Logger* Texture::s_Logger = nullptr;

Texture::Texture(const std::string& fileName)
{
	m_Texture = new sf::Texture();
	if (!m_Texture->loadFromFile(fileName))
	{
		s_Logger->Err("Could not load texture " + fileName + " !");
		delete m_Texture;
		m_Texture = nullptr;
	}
}

Texture* Texture::GetTexture(const std::string& string)
{
	// Create texture if it does't exist, then return it
	if (s_Textures.find(string) == s_Textures.end()) s_Textures[string] = new Texture("../../res/" + string);
	return s_Textures.at(string);
}

void Texture::CleanUp()
{
	for (auto& [name, texture] : s_Textures)
	{
		if (texture->m_Texture != nullptr) delete texture->m_Texture;
		delete texture;
	}
}
