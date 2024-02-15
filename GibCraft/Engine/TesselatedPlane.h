#pragma once
#include <glm/glm.hpp>
#include <stdint.h>
#include <array>
#include <stdio.h>
#include <glad/glad.h>
#define GLM_EXT_INCLUDED
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/integer.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/quaternion.hpp>
#include "GibMaths.h"
#include "ShaderClass.h"
#include "Cube.h"
#include <vector>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
class TesselatedPlane
{
	std::array<glm::vec3, 4> vertexPoints;
	std::vector<GLuint> vertexIndices;
	uint32_t vao;
	uint32_t vbo;
	std::vector<BaseCubeVertex> vertices;
public:
	TesselatedPlane(int totalSize, int pointsPerMetre);

	void DrawPlane(const glm::vec3& position, Shader& shader);
};