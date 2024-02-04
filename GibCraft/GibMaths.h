#pragma once
#include <math.h>

struct Vec3
{
public:
	float x, y, z;
public:
	Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
};

struct Vec2
{
public:
	float x, y;
public:
	Vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
};