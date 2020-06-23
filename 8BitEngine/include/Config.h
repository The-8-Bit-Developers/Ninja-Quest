#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#ifdef DEBUG
constexpr const char * RES_PATH = "../../res/";
#else
constexpr const char * RES_PATH = "./res/";
#endif

constexpr float PHYSICS_SCALE = 1.0f;
constexpr float PHYSICS_SCALE_INV = 1.0f / PHYSICS_SCALE;

#endif