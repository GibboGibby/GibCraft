#include "ShaderClass.h"

std::string GetFileContents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	std::string vertexCode = GetFileContents(vertexFile);
	std::string fragmentCode = GetFileContents(fragmentFile);

	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);
	CompileErrors(vertexShader, "VERTEX");

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);
	CompileErrors(fragmentShader, "FRAGMENT");


	ID = glCreateProgram();

	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);

	glLinkProgram(ID);

	CompileErrors(ID, "PROGRAM");

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

void Shader::CompileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader Compilation Error for: " << type << "\n" << std::endl;
			std::cout << "Shader Error Info Log: \n" << infoLog << std::endl;;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "Shader linking error for: " << type << "\n" << std::endl;
		}
	}
}

void Shader::UploadFloat4(GLint location, float x, float y, float z, float w)
{
	glUniform4f(location, x, y, z, w);
}

void Shader::UploadVec4(const char* varName, const glm::vec4& vec4) const
{
	glUniform4f(glGetUniformLocation(ID, varName), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::UploadVec3(const char* varName, const glm::vec3& vec3) const
{
	glUniform3f(glGetUniformLocation(ID, varName), vec3.x, vec3.y, vec3.z);
}

void Shader::UploadVec2(const char* varName, const glm::vec2& vec2) const
{
	glUniform2f(glGetUniformLocation(ID, varName), vec2.x, vec2.y);
}

void Shader::UploadIVec4(const char* varName, const glm::ivec4& vec4) const
{
	glUniform4i(glGetUniformLocation(ID, varName), vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::UploadIVec3(const char* varName, const glm::ivec3& vec3) const
{
	glUniform3i(glGetUniformLocation(ID, varName), vec3.x, vec3.y, vec3.z);
}

void Shader::UploadIVec2(const char* varName, const glm::ivec2& vec2) const
{
	glUniform2i(glGetUniformLocation(ID, varName), vec2.x, vec2.y);
}

void Shader::UploadFloat(const char* varName, float value) const
{
	glUniform1f(glGetUniformLocation(ID, varName), value);
}

void Shader::UploadInt(const char* varName, int value) const
{
	glUniform1i(glGetUniformLocation(ID, varName), value);
}

void Shader::UploadIntArray(const char* varName, int length, const int* array) const
{
	//glUniform
}

void Shader::UploadUInt(const char* varName, glm::uint32 value) const
{
	glUniform1ui(glGetUniformLocation(ID, varName), value);
}

void Shader::UploadBool(const char* varName, bool value) const
{
	//glUniform1b
}

void Shader::UploadMat4(const char* varName, const glm::mat4& mat4) const
{
	glUniformMatrix4fv(glGetUniformLocation(ID, varName), 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::UploadMat3(const char* varName, const glm::mat3& mat3) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, varName), 1, GL_FALSE, glm::value_ptr(mat3));
}
