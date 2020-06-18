#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "Vec2.h"

class Camera
{
public:

	Camera() : size(400.0f, 400.0f) {}

	Vec2 size;
	Vec2 position;
};

#endif