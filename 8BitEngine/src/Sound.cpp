#include "Sound.h"

Logger* Sound::s_Logger = nullptr;
std::unordered_map<std::string, sf::SoundBuffer*> Sound::s_Buffers;

Sound::Sound(const std::string& fileName)
{
	if (s_Buffers.find(RES_PATH + fileName) == s_Buffers.end()) 
		s_Buffers.emplace(RES_PATH + fileName, new sf::SoundBuffer());

	if (!s_Buffers.at(RES_PATH + fileName)->loadFromFile(RES_PATH + fileName)) s_Logger->Err("Could not load audio file " + fileName + "!");
	m_Sound.setBuffer(*s_Buffers.at(RES_PATH + fileName));
}

void Sound::Play()				{ m_Sound.play(); }
void Sound::Pause()				{ m_Sound.pause(); }
void Sound::Stop()				{ m_Sound.stop(); }
void Sound::SetVolume(float v)  { m_Sound.setVolume(v); }
void Sound::SetLoop(bool b)		{ return m_Sound.setLoop(b); }

void Sound::Clean()
{
	for (auto& [string, buffer] : s_Buffers) delete buffer;
}

Sound::~Sound()
{
	
}