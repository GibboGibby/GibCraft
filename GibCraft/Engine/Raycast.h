#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>
#include <algorithm>

#include "Block.h"

namespace Physics
{
	void FastVoxelTraversal(glm::vec3 start, glm::vec3 end, std::vector<glm::vec3>& visited_voxels);
}