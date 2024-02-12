#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include "GibMaths.h"
#include <glm/ext.hpp>

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
	//void UploadMat4(GLint location, glm::mat4& matrix);

	void UploadVec4(const char* varName, const glm::vec4& vec4) const;
	void UploadVec3(const char* varName, const glm::vec3& vec3) const;
	void UploadVec2(const char* varName, const glm::vec2& vec2) const;

	void UploadIVec4(const char* varName, const glm::ivec4& vec4) const;
	void UploadIVec3(const char* varName, const glm::ivec3& vec3) const;
	void UploadIVec2(const char* varName, const glm::ivec2& vec2) const;

	void UploadFloat(const char* varName, float value) const;
	void UploadInt(const char* varName, int value) const;
	void UploadIntArray(const char* varName, int length, const int* array) const;
	void UploadUInt(const char* varName, glm::uint32 value) const;
	void UploadBool(const char* varName, bool value) const;

	void UploadMat4(const char* varName, const glm::mat4& mat4) const;
	void UploadMat3(const char* varName, const glm::mat3& mat3) const;
};

#endif