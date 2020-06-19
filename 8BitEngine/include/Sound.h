#pragma once
#ifndef SOUND_H
#define SOUND_H

#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>

#include "Logger.h"
#include "Config.h"

class Sound
{
public:
	Sound(const std::string& fileName);
	~Sound();

	void Play();
	void Pause();
	void Stop();

	void SetVolume(float v);
	void SetLoop(bool b);

	static void Clean();

	static Logger* s_Logger;

private:
	sf::Sound m_Sound;

	static std::unordered_map<std::string, sf::SoundBuffer*> s_Buffers;
};

#endif