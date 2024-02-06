#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include "GibMaths.h"

std::string GetFileContents(const char* filename);

class Shader
{
public:
	GLuint ID;
	Shader(const char* vertexFile, const char* fragmentFile);

	void Activate();
	void Delete();
	void CompileErrors(unsigned int shader, const char* type);

	// Upload Uniforms
	void UploadFloat4(GLint location, float x, float y, float z, float w);
	void UploadMat4(GLint location, glm::mat4 matrix);
};

#endif