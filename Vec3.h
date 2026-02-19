#pragma once
#include "Vec2.h"

struct vec3
{
	float x, y, z;

	vec3(float _value) : x(_value), y(_value), z(_value) {};
	vec3(vec2 const& v) : x(v.x), y(v.y), z(1.0f){};
	vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};

	vec3 operator+(vec3 const& other) { return vec3(x + other.x, y + other.y, z + other.z); }
	vec3 operator-(vec3 const& other) { return vec3(x - other.x, y - other.y, z - other.z); }
	vec3 operator*(vec3 const& other) { return vec3(x * other.x, y * other.y, z * other.z); }
	vec3 operator/(vec3 const& other) { return vec3(x / other.x, y / other.y, z / other.z); }
};
