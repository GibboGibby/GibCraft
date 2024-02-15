#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderClass.h"
#include <map>

/*
struct Cube
{
	float length, height, breadth;
};
*/

enum OldBlockType
{
	DIRT,
	OAK_PLANK
};

struct CubeVertex
{
	glm::vec3 position;
	glm::vec2 texCoords;
};

class CubeRenderer
{

private:
	//std::array<glm::vec2, 2> atlasPositions = { glm::vec2(0,0), glm::vec2(1,0) };
	std::map<OldBlockType, glm::vec2> atlasPosition = { {OldBlockType::DIRT, glm::vec2(0,0)}, {OldBlockType::OAK_PLANK, glm::vec2(1,0)} };
	std::array<CubeVertex, 36> vertices;
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

public:
	CubeRenderer();
	~CubeRenderer();

	//void RenderCube(const glm::vec3& position, Texture* texture, float rotation, const glm::mat4& projection, const glm::mat4& view = glm::mat4(1.0f), Shader* shader = nullptr);

	void RenderCube(const glm::vec3& pos, float rotation, const glm::mat4& projection, Shader& shader, const glm::mat4& view = glm::mat4(1.0f));


};