#pragma once
#ifndef RAYCAST_CALLBACK_H
#define RAYCAST_CALLBACK_H

#include <box2d/box2d.h>

/* Helper class for ray casting with Box2D */
class RaycastCallback : public b2RayCastCallback
{
public:
	RaycastCallback();

	// Inherited via b2RayCastCallback
	virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction);

	bool bHit;
	b2Fixture* out_fixture = nullptr;
	b2Vec2 out_point;
	b2Vec2 out_normal;
	float out_fraction;
};

#endif