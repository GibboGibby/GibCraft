#pragma once

#include "Defs.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct i8vec3
{
	uint8_t x;
	uint8_t y;
	uint8_t z;

	i8vec3 operator=(const glm::vec3& vec)
	{
		assert(floor(vec.x) <= CHUNK_SIZE_X);
		assert(floor(vec.y) <= 255);
		assert(floor(vec.z) <= CHUNK_SIZE_Z);

		x = floor(vec.x);
		y = floor(vec.y);
		z = floor(vec.z);

		return *this;
	}
};

struct i16vec2
{
	uint16_t x;
	uint16_t y;
};

struct Vertex
{
	i8vec3 position;
	i16vec2 texture_coords;
	uint8_t lighting_level;
	uint8_t block_face_lighting;
	float ambientOcclusion;
};