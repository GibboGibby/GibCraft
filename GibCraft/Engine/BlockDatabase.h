#pragma once

#include "Block.h"
#include <array>
#include<unordered_map>
#include <string>

#include "TextureAtlas.h"

namespace BlockDatabase
{
	const std::array<uint16_t, 8>& GetBlockTexture(BlockType type, BlockFace face);
	const std::string& GetBlockName(BlockType type);
	const std::string& GetBlockSoundPath(BlockType type);
}



