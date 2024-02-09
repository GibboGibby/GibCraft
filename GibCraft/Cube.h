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

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

class Cube
{
	std::array<Vertex, 36> vertices;
	uint32_t vao;
	uint32_t vbo;

	std::array<uint32_t, 36> cubeElements = {
		// Each set of 6 indices represents one quad
		1, 0, 2,    3, 1, 2,    // Front face
		5, 1, 3,    7, 5, 3,    // Right face
		7, 6, 4,    5, 7, 4,    // Back face
		0, 4, 6,    2, 0, 6,    // Left face
		5, 4, 0,    1, 5, 0,    // Top face
		3, 2, 6,    7, 3, 6     // Bottom face
	};

	std::array<glm::vec3, 6> normals = {
			glm::vec3(0, 0, 1),
			glm::vec3(1, 0, 0),
			glm::vec3(0, 0, -1),
			glm::vec3(-1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(0, -1, 0)
	};

	std::array<glm::vec3, 6> tangents = {
		glm::vec3(1, 0, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(-1, 0, 0),
		glm::vec3(0, 0, -1),
		glm::vec3(1, 0, 0),
		glm::vec3(-1, 0, 0)
	};

	std::array<glm::vec3, 6> biTangents = {
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, -1)
	};
public:
	Cube();

	void DrawCube(const glm::vec3& position, Shader& shader);
};