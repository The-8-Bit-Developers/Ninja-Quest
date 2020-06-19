#pragma once
#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#ifdef DEBUG
constexpr char* RES_PATH = "../../res/";
#else
constexpr char* RES_PATH = "./res/";
#endif

#endif