#include "BlockDatabase.h"
namespace BlockDatabase
{
	enum class BlockTypeTexture
	{
		OakPlanks,
		Dirt,
		GrassTop,
		GrassFront,
		GrassSide,
		Stone,
		Cobblestone,

		UnknownBlockType
	};

	std::unordered_map<BlockTypeTexture, std::array<uint16_t, 8>> BlockDatabase;
	std::unordered_map<BlockType, std::string> BlockNameDatabase;

	BlockTypeTexture GetRealBlockTexture(BlockType type, BlockFace face)
	{
		uint8_t block = (BlockType)type;
		BlockTypeTexture retVal = (BlockTypeTexture)block;

		switch (type)
		{
		case AIR:
			break;
		case OAK_PLANKS:
			retVal = BlockTypeTexture::OakPlanks;
			break;
		case DIRT:
			retVal = BlockTypeTexture::Dirt;
			break;
		case COBBLESTONE:
			retVal = BlockTypeTexture::Cobblestone;
			break;
		case STONE:
			retVal = BlockTypeTexture::Stone;
			break;
		case GRASS:
			if (face == BlockFace::FRONT || face == BlockFace::BACK)
				retVal = BlockTypeTexture::GrassFront;
			else if (face == BlockFace::TOP)
				retVal = BlockTypeTexture::GrassTop;
			else if (face == BlockFace::LEFT || face == BlockFace::RIGHT)
				retVal = BlockTypeTexture::GrassSide;
			else if (face == BlockFace::BOTTOM)
				retVal = BlockTypeTexture::Dirt;
			break;
		}

		return retVal;
	}

	const std::array<uint16_t, 8>& GetBlockTexture(BlockType type, BlockFace face)
	{
		static bool first_run = true;
		if (first_run)
		{
			TextureAtlas BlockTextureAtlas("resources\\textures\\atlas.png", 16, 16);
			first_run = false;
			constexpr int list_sz = 8;
			BlockTypeTexture texture_list[list_sz] =
			{
				BlockTypeTexture::Dirt,
				BlockTypeTexture::OakPlanks,
				BlockTypeTexture::GrassTop,
				BlockTypeTexture::GrassSide,
				BlockTypeTexture::GrassFront,
				BlockTypeTexture::Stone,
				BlockTypeTexture::Cobblestone,
				BlockTypeTexture::UnknownBlockType
			};

			for (int i = 0; i < list_sz; i++)
			{
				if (texture_list[i] == BlockTypeTexture::GrassSide)
				{
					BlockDatabase[texture_list[i]] = BlockTextureAtlas.SampleTexel(glm::vec2(i, 0), glm::vec2(i + 1, 1), true);
				}
				else
				{
					BlockDatabase[texture_list[i]] = BlockTextureAtlas.SampleTexel(glm::vec2(i, 0), glm::vec2(i + 1, 1));
				}
			}
		}

		BlockTypeTexture tex = GetRealBlockTexture(type, face);
		std::unordered_map<BlockTypeTexture, std::array<uint16_t, 8>>::iterator iter = BlockDatabase.find(tex);

		if (iter == BlockDatabase.end())
		{
			
			return BlockDatabase[BlockTypeTexture::UnknownBlockType];
		}
		return BlockDatabase[tex];
	}

	const std::string& GetBlockName(BlockType type)
	{
		// TODO: insert return statement here
		static bool first_run = true;
		if (first_run)
		{
			first_run = false;
			BlockNameDatabase[BlockType::GRASS] = "Grass";
			BlockNameDatabase[BlockType::DIRT] = "Dirt";
			BlockNameDatabase[BlockType::AIR] = "Air";
			BlockNameDatabase[BlockType::OAK_PLANKS] = "Oak Planks";
			BlockNameDatabase[BlockType::UNKNOWN] = "UNKNOWN BLOCK";
		}

		std::unordered_map<BlockType, std::string>::iterator iter = BlockNameDatabase.find(type);
		if (iter == BlockNameDatabase.end())
		{
			return BlockNameDatabase[BlockType::UNKNOWN];
		}
		return BlockNameDatabase[type];
	}

	const std::string& BlockDatabase::GetBlockSoundPath(BlockType type)
	{
		// TODO: insert return statement here
		return "";
	}

}