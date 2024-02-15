#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO
{
public:
	GLuint ID;
	EBO();
	void BufferData(GLsizeiptr size, void* data, GLenum usage);

	void Bind();
	void Unbind();
	void Delete();
};

#endif