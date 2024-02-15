#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>
#include "GibMaths.h"
#include "Cube.h"

class VBO
{
public:
	GLuint ID;
	VBO(GLfloat* vertices, GLsizeiptr size);
	VBO(BaseCubeVertex* vertices, GLsizeiptr size);
	VBO();

	void BufferData(GLsizeiptr size, void* data, GLenum usage);
	void Bind();
	void Unbind();
	void Delete();
};

#endif