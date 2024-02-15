#include "CubeRenderer.h"

CubeRenderer::CubeRenderer()
{
	std::array<glm::vec3, 8> cubeVertices;
	cubeVertices[0] = glm::vec3(-0.5f, 0.5f, 0.5f);
	cubeVertices[1] = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertices[2] = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertices[3] = glm::vec3(0.5f, -0.5f, 0.5f);

	cubeVertices[4] = glm::vec3(-0.5f, 0.5f, -0.5f);
	cubeVertices[5] = glm::vec3(0.5f, 0.5f, -0.5f);
	cubeVertices[6] = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertices[7] = glm::vec3(0.5f, -0.5f, -0.5f);

	// Tex-coords always loop with the triangle going:
	//		top-right, top-left, bottom-left
	//		top-right, bottom-right, bottom-left

	std::array<glm::vec2, 6> texCoords;
	texCoords[0] = glm::vec2(0.5f, 1.0f);
	
	texCoords[1] = glm::vec2(0.0f, 1.0f);
	texCoords[2] = glm::vec2(0.0f, 0.0f);

	texCoords[4] = glm::vec2(0.5f, 1.0f);
	
	texCoords[3] = glm::vec2(0.5f, 0.0f);
	
	texCoords[5] = glm::vec2(0.0f, 0.0f);

	for (int vertexIndex = 0; vertexIndex < cubeElements.size(); vertexIndex++)
	{
		vertices[vertexIndex].position = cubeVertices[cubeElements[vertexIndex]];
		vertices[vertexIndex].texCoords = texCoords[vertexIndex % 6];
		//vertices[vertexIndex].texCoords += 0.5;
		if (vertexIndex < 6)
		{
			//vertices[vertexIndex].texCoords /= 2;
		}
		else
		{
			//vertices[vertexIndex].texCoords /= 2;
			vertices[vertexIndex].texCoords += 0.5;
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)offsetof(CubeVertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)offsetof(CubeVertex, texCoords));
	glEnableVertexAttribArray(1);

}

CubeRenderer::~CubeRenderer()
{
}

void CubeRenderer::RenderCube(const glm::vec3& pos, float rotation, const glm::mat4& projection, Shader& shader, const glm::mat4& view)
{
	//int textureSlot;
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, 0);
	shader.UploadInt("uTexture", 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());

}
