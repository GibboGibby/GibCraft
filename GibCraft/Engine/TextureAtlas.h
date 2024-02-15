#pragma once
#include "Texture.h"
#include <glm/ext.hpp>
class TextureAtlas
{
public:
	TextureAtlas(Texture* atlas_texture, int tx, int ty)
	{
		mAtlas = atlas_texture;
		mTileX = tx;
		mTileY = ty;
		mAtlasWidth = atlas_texture->GetWidth();
		mAtlasHeight = atlas_texture->GetHeight();
	}

	TextureAtlas(const std::string& atlas_path, int tx, int ty)
	{
		mAtlas = new Texture(atlas_path);
		mTileX = tx;
		mTileY = ty;
		mAtlasWidth = mAtlas->GetWidth();
		mAtlasHeight = mAtlas->GetHeight();
	}

	TextureAtlas(int width, int height, int tx, int ty)
	{
		mAtlasWidth = width;
		mAtlasHeight = height;
		mTileX = tx;
		mTileY = ty;
	}

	std::array<GLfloat, 8> Sample(const glm::vec2& startCoords, const glm::vec2& endCoords, bool flip = false)
	{
		glm::vec2 start;
		glm::vec2 end;

		start = startCoords;
		end = endCoords;

		if (flip)
		{
			start = endCoords;
			end = startCoords;
		}

		float width, height;
		float x1, y1;
		float x2, y2;

		std::array<float, 8> TextureCoordinates;
		x1 = start.x * mTileX;
		y1 = start.y * mTileY;
		x2 = end.x * mTileX;
		y2 = end.y * mTileY;

		TextureCoordinates[0] = x2;
		TextureCoordinates[1] = y1;
		TextureCoordinates[2] = x1;
		TextureCoordinates[3] = y1;
		TextureCoordinates[4] = x1;
		TextureCoordinates[5] = y2;
		TextureCoordinates[6] = x2;
		TextureCoordinates[7] = y2;

		return TextureCoordinates;
	}

	std::array<uint16_t, 8> SampleTexel(const glm::vec2& start_coords, const glm::vec2& end_coords, bool flip = false)
	{
		glm::vec2 s_coords;
		glm::vec2 e_coords;

		s_coords = start_coords;
		e_coords = end_coords;

		if (flip)
		{
			s_coords = end_coords;
			e_coords = start_coords;
		}

		float width, height;
		float x2, y2;
		float x1, y1;

		std::array<uint16_t, 8> TextureCoordinates;

		x1 = s_coords.x * mTileX;
		y1 = s_coords.y * mTileY;
		x2 = e_coords.x * mTileX;
		y2 = e_coords.y * mTileY;

		TextureCoordinates[0] = x2;
		TextureCoordinates[1] = y1;
		TextureCoordinates[2] = x1;
		TextureCoordinates[3] = y1;
		TextureCoordinates[4] = x1;
		TextureCoordinates[5] = y2;
		TextureCoordinates[6] = x2;
		TextureCoordinates[7] = y2;

		return TextureCoordinates;
	}

	std::array<GLfloat, 8> SampleCustom(const glm::vec2& start_coords, const glm::vec2& end_coords)
	{
		float width, height;
		float x2, y2;
		float x1, y1;

		width = end_coords.x - start_coords.x;
		height = end_coords.y - start_coords.y;

		std::array<GLfloat, 8> TextureCoordinates;

		x1 = start_coords.x;
		y1 = start_coords.y;
		x2 = end_coords.x;
		y2 = end_coords.y;

		x1 = x1 / mAtlasWidth;
		y1 = y1 / mAtlasHeight;
		x2 = x2 / mAtlasWidth;
		y2 = y2 / mAtlasHeight;

		TextureCoordinates[0] = x2;
		TextureCoordinates[1] = y2;
		TextureCoordinates[2] = x2;
		TextureCoordinates[3] = y1;
		TextureCoordinates[4] = x1;
		TextureCoordinates[5] = y1;
		TextureCoordinates[6] = x1;
		TextureCoordinates[7] = y2;

		return TextureCoordinates;
	}

private:
	int mTileX, mTileY;
	int mAtlasWidth, mAtlasHeight;
	Texture* mAtlas;
};