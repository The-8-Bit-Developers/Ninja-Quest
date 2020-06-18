#pragma once
#ifndef RAYCAST_H
#define RAYCAST_H

#include <box2d/box2d.h>
#include "Vec2.h"
#include "RaycastCallback.h"
#include "Sprite.h"

inline bool DoRaycast(Vec2 position, Vec2 direction)
{
	b2Vec2 p1 = b2Vec2(position.x, position.y);
	b2Vec2 p2 = p1 + b2Vec2(direction.x, direction.y);

	RaycastCallback callback;
	Sprite::s_PhysicsWorld.RayCast(&callback, p1, p2);
	return callback.bHit;
}

#endif