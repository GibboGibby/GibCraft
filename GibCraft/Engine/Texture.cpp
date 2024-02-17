#include "Texture.h"

Texture::Texture(std::string path, bool flip, GLenum type, GLenum min_filter, GLenum mag_filter, GLenum texwrap_s, GLenum texwrap_t, std::array<GLfloat, 8> TextureCoords, bool clean_up)
{
	m_texture = new GLuint;
	this->CreateTexture(path, flip, type, min_filter, mag_filter, texwrap_s, texwrap_t, TextureCoords, clean_up);
}

void Texture::CreateTexture(std::string path, bool flip, GLenum type, GLenum min_filter, GLenum mag_filter, GLenum texwrap_s, GLenum texwrap_t, std::array<GLfloat, 8> TextureCoords, bool clean_up)
{
	if (flip)
		stbi_set_flip_vertically_on_load(true);
	else
		stbi_set_flip_vertically_on_load(true);

	GLenum internalFormat = GL_RGBA;

	m_delete_texture = true;
	m_texture = new GLuint;
	m_TextureCoords = TextureCoords;
	m_clean_up = clean_up;
	m_type = type;
	m_path = path;

	glGenTextures(1, m_texture);
	glBindTexture(type, *m_texture);
	glTexParameteri(type, GL_TEXTURE_WRAP_S, texwrap_s);
	glTexParameteri(type, GL_TEXTURE_WRAP_T, texwrap_t);

	glTexParameteri(type, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(type, GL_TEXTURE_MAG_FILTER, mag_filter);

	unsigned char* image = stbi_load(path.c_str(), &m_width, &m_height, &m_bpp, 4);

	m_bpp = 4;
	if (m_bpp == 1)
		internalFormat = GL_RED;
	else if (m_bpp == 3)
		internalFormat = GL_RGB;
	else if (m_bpp == 4)
		internalFormat = GL_RGBA;

	glTexImage2D(type, 0, internalFormat, m_width, m_height, 0, internalFormat, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(type);
	// 
	// (type);

	if (image && clean_up)
	{
		stbi_image_free(image);
	}
}

void Texture::IntCreateTexture(GLuint id, std::array<GLfloat, 8> tex_coords, int w, int h, bool delete_texture) // To use for Texture Atlases
{
	m_texture = new GLuint;
	*m_texture = id;
	m_TextureCoords = tex_coords;
	m_width = w;
	m_height = h;
	m_delete_texture = delete_texture;
}

ExtractedImageData ExtractTextureData(const std::string& path)
{
	ExtractedImageData  retval;
	retval.image_data = stbi_load(path.c_str(), &retval.width, &retval.height, &retval.channels, 4);
	return retval;
}