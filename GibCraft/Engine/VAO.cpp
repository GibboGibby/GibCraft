#include "VAO.h"

VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

void VAO::LinkAttrib(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

void VAO::LinkAttribIPointer(VBO VBO, GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
	VBO.Bind();
	glVertexAttribIPointer(index, size, type, stride, pointer);
	glEnableVertexAttribArray(index);
	VBO.Unbind();
}


void VAO::Bind()
{
	glBindVertexArray(ID);
}

void VAO::Unbind()
{
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}