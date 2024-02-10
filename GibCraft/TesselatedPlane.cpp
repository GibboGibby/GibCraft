#include "TesselatedPlane.h"

TesselatedPlane::TesselatedPlane(int startSize, int pointsPerMetre)
{
	vertexPoints[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	vertexPoints[1] = glm::vec3(1.0f, 0.0f, 0.0f);
	vertexPoints[3] = glm::vec3(1.0f, 0.0f, 1.0f);
	vertexPoints[2] = glm::vec3(0.0f, 0.0f, 1.0f);



	
	float pointIncrease = 1.0f / pointsPerMetre;
	/*
	glm::vec3 prevPos = glm::vec3(0.0f, 0.0f, 0.0f);
	for (float i = 0.0f; i < startSize; i += pointIncrease)
	{
		
		Vertex v1;
		v1.position = prevPos + (vertexPoints[0] * i);
		
		v1.texCoords = v1.position;

		Vertex v2;
		v2.position = prevPos + (vertexPoints[1] * i);
		
		v2.texCoords = v2.position;

		Vertex v3;
		v3.position = prevPos + (vertexPoints[3] * i);
		v3.texCoords = v3.position;

		Vertex v4;
		v4.position = prevPos + (vertexPoints[2] * i);
		v4.texCoords = v4.position;

		vertices.push_back(v1);
		vertices.push_back(v2);
		vertices.push_back(v3);

		vertices.push_back(v1);
		vertices.push_back(v4);
		vertices.push_back(v3);
	}
	*/

	int square = 0;
	for (float x = 0.0f; x < startSize; x += pointIncrease)
	{
		for (float y = 0.0f; y < startSize; y += pointIncrease)
		{
			Vertex v1;
			v1.position = glm::vec3(x, 0.0f, y) + (vertexPoints[0] * pointIncrease);
			//v1.texCoords = glm::vec3(v1.position.x, v1.position.z, v1.position.y);
			v1.texCoords = glm::vec2(v1.position.x, v1.position.z);

			Vertex v2;
			v2.position = glm::vec3(x, 0.0f, y) + (vertexPoints[1] * pointIncrease);
			v2.texCoords = glm::vec2(v2.position.x, v2.position.z);

			Vertex v3;
			v3.position = glm::vec3(x, 0.0f, y) + (vertexPoints[3] * pointIncrease);
			v3.texCoords = glm::vec2(v3.position.x, v3.position.z);

			Vertex v4;
			v4.position = glm::vec3(x, 0.0f, y) + (vertexPoints[2] * pointIncrease);
			v4.texCoords = glm::vec2(v4.position.x, v4.position.z);

			/*
			std::cout << "tri 1 - " << std::endl;
			std::cout << "    v1 - " << v1.position << std::endl;
			std::cout << "    v2 - " << v2.position << std::endl;
			std::cout << "    v3 - " << v3.position << std::endl;
			std::cout << "tri 2 - " << std::endl;
			std::cout << "    v1 - " << v1.position << std::endl;
			std::cout << "    v4 - " << v4.position << std::endl;
			std::cout << "    v3 - " << v3.position << std::endl;
			*/

			vertices.push_back(v1);
			vertices.push_back(v3);
			vertices.push_back(v2);

			
			vertices.push_back(v1);
			vertices.push_back(v4);
			vertices.push_back(v3);
			
			/*
			vertexIndices.push_back(square * 4 + 0);
			vertexIndices.push_back(square * 4 + 1);
			vertexIndices.push_back(square * 4 + 2);

			vertexIndices.push_back(square * 4 + 1);
			vertexIndices.push_back(square * 4 + 4);
			vertexIndices.push_back(square * 4 + 3);
			square++;
			*/
		}
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



}

void TesselatedPlane::DrawPlane(const glm::vec3& position, Shader& shader)
{
	glm::mat4 transform = glm::translate(position);

	
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
}