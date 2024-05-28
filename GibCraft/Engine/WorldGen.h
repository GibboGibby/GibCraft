#pragma once

#include <iostream>
#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#include "World.h"
#include "FastNoise.h"
#include "Chunk.h"

#include "Random.h"

namespace WorldGen
{
	Block* GetWorldBlock(const glm::vec3& blockPos);

	void GenerateChunk(std::shared_ptr<Chunk> chunk, const int WorldSeed);

}