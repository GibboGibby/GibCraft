#include "Cube.h"

float newVertices[] = {

	// back face
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	// front face
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	// left
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	// right
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 // bottom
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	// top
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

Cube::Cube()
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

	std::array<glm::vec2, 6> texCoords;
	texCoords[0] = glm::vec2(1.0f, 1.0f);
	texCoords[1] = glm::vec2(0.0f, 1.0f);
	texCoords[2] = glm::vec2(0.0f, 0.0f);

	texCoords[4] = glm::vec2(1.0f, 1.0f);
	texCoords[3] = glm::vec2(1.0f, 0.0f);
	texCoords[5] = glm::vec2(0.0f, 0.0f);

	for (int vertexIndex = 0; vertexIndex < cubeElements.size(); vertexIndex++)
	{
		vertices[vertexIndex].position = cubeVertices[cubeElements[vertexIndex]];
		vertices[vertexIndex].texCoords = texCoords[vertexIndex % 6];
	}

	for (int i = 0; i < 6; i++)
	{
		vertices[i].textureSlot = 1;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(2);

	// Set up and enable attribute 2 (OpenGL 3.3 method)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	glEnableVertexAttribArray(3);

	// Set up and enable attribute 2 (OpenGL 3.3 method)
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, textureSlot));
	glEnableVertexAttribArray(5);


}

void Cube::DrawCube(const glm::vec3& pos, Shader& shader)
{
	glm::mat4 transform = glm::translate(pos);
	//shader.UploadMat4("uTransform", transform);

	glBindVertexArray(vao);
	shader.UploadInt("uTexture", 1);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	shader.UploadInt("uTexture", 0);
	glDrawArrays(GL_TRIANGLES, 6, (GLsizei)vertices.size()-6);
	
}
