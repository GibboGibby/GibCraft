#pragma once
#include "stb_image.h"
#include "glad/glad.h"
#include <string>
#include <array>
struct ExtractedImageData
{
	unsigned char* image_data;
	int width;
	int height;
	int channels;
};

class Texture
{
public:
	Texture(std::string path, bool flip = true, GLenum type = GL_TEXTURE_2D, GLenum min_filter = GL_NEAREST,
		GLenum mag_filter = GL_NEAREST, GLenum texWrapS = GL_REPEAT, GLenum texWrapT = GL_REPEAT,
		std::array<GLfloat, 8> TextureCoords = { 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, bool clean_up = true);

	Texture()
	{
		m_texture = new GLuint;
		m_TextureCoords = { 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
		m_clean_up = true;
		m_width = 0;
		m_height = 0;
		m_texture = 0;
		m_type = GL_TEXTURE_2D;
		m_intFormat = GL_RGBA;
		m_bpp = 0;
		m_delete_texture = true;
	}

	~Texture()
	{
		if (this->m_delete_texture == 1 && this->m_texture != nullptr)
		{
			glDeleteTextures(1, m_texture);
			m_texture = nullptr;
		}
		else
		{
			// Don't delete
		}
	}

	void CreateTexture(std::string path, bool flip = true, GLenum type = GL_TEXTURE_2D,
		GLenum min_filter = GL_NEAREST, GLenum mag_filter = GL_NEAREST,
		GLenum texwrap_s = GL_REPEAT, GLenum texwrap_t = GL_REPEAT, std::array<GLfloat, 8> TextureCoords = { 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }, bool clean_up = true);

	void IntCreateTexture(GLuint id, std::array<GLfloat, 8> tex_coords, int w, int h, bool delete_texture);

	inline int GetWidth() const
	{
		return m_width;
	}

	inline int GetHeight() const
	{
		return m_height;
	}

	inline void Bind(int slot = 0) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(this->m_type, *(this->m_texture));
	}

	inline void Unbind() const
	{
		glBindTexture(m_type, 0);
	}

	inline GLuint GetTextureID() const
	{
		return *m_texture;
	};

	inline std::string GetTexturePath() const
	{
		return m_path;
	}

	std::array<GLfloat, 8> GetTextureCoords() const
	{
		return m_TextureCoords;
	}

	int m_delete_texture;
private:
	bool m_clean_up;

	int m_width;
	int m_height;
	int m_bpp;
	GLenum m_intFormat;
	GLuint* m_texture;
	GLenum m_type;
	std::string m_path;
	std::array<float, 8> m_TextureCoords;
};

ExtractedImageData ExtractImageData(const std::string& path);