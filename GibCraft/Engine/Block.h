#pragma once

enum BlockType : char
{
	AIR,
	OAK_PLANKS,
	DIRT,
	GRASS,
	STONE,
	COBBLESTONE,
	UNKNOWN
};

enum BlockFace : char
{
	TOP,
	LEFT,
	RIGHT,
	FRONT,
	BACK,
	BOTTOM
};

struct Block
{

	Block() = default;
	BlockType type = BlockType::UNKNOWN;

	bool IsModel() const
	{
		/*
		if (type == Model_Grass ||
			type == Model_Deadbush ||
			type == Model_Kelp ||
			type == Flower_allium ||
			type == Flower_orchid ||
			type == Flower_tulip_red ||
			type == Flower_tulip_pink ||
			type == Flower_rose ||
			type == Flower_dandelion)
		{
			return true;
		}
		*/

		return false;
	}

	/*
	Blocks like cactus and flowers need their below block to exist. This function returns true if it depends on it's lower block
	*/
	bool DependsOnBelowBlock()
	{
		if (IsModel())
		{
			return true;
		}

		return false;
	}

	bool CastsShadow()
	{
		/*
		if (type != BlockType::Air && type != BlockType::GlassWhite && !IsModel())
		{
			return true;
		}

		*/
		return false;
	}

	bool IsOpaque() const
	{
		/*
		if (type != BlockType::Air && type != BlockType::GlassWhite
			&& type != BlockType::Water && type != BlockType::Cactus && type != BlockType::OakLeaves
			&& type != BlockType::SpruceLeaves && !IsModel())
		{
			return true;
		}
		*/
		if (type != BlockType::AIR)
			return true;

		return false;
	}

	bool IsTransparent() const
	{
		/*
		if (type == BlockType::Water || type == BlockType::Lava
			|| type == BlockType::GlassWhite || type == BlockType::OakLeaves ||
			type == BlockType::SpruceLeaves || type == BlockType::Air || IsModel())
		{
			return true;
		}
		*/
		if (type == BlockType::AIR)
			return true;
		return false;
	}

	bool IsLightPropogatable() const
	{
		/*
		if (type == BlockType::Water || type == BlockType::GlassWhite || type == BlockType::Air || IsModel())
		{
			return true;
		}
		*/

		if (type == BlockType::AIR)
			return true;
		return false;
	}

	bool Collidable() const
	{
		/*
		if (type != BlockType::Air && type != BlockType::Water && type != BlockType::Lava && !IsModel())
		{
			return true;
		}
		*/
		if (type != BlockType::AIR)
			return true;

		return false;
	}

	bool IsLiquid() const
	{
		/*
		if (type == BlockType::Water || type == BlockType::Lava)
		{
			return true;
		}
		*/

		return false;
	}
};