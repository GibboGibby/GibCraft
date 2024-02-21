#include "Chunk.h"

Chunk::Chunk(const glm::vec3 chunkPos) : pPosition(chunkPos), pMeshState(ChunkMeshState::Unbuilt), pChunkState(ChunkState::Ungenerated)

{
	//memset(&pChunkContents, BlockType::AIR, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
	//std::fill(std::begin(pChunkContents, std::end(pChunkContents)), BlockType::AIR);
	//std::fill_n(&pChunkContents[0][0][0], CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z, BlockType::AIR);
	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				SetBlock(BlockType::AIR, glm::vec3(x, y, z));
			}
		}
	}
	memset(&p_HeightMap, 0, CHUNK_SIZE_X * CHUNK_SIZE_Z * sizeof(std::uint8_t));
}

Chunk::~Chunk()
{

}

void Chunk::SetBlock(BlockType type, const glm::vec3& position)
{
	Block b;
	b.type = type;

	pChunkContents.at(position.x).at(position.y).at(position.z) = b;
}

void Chunk::Construct(ChunkDataTypePtr forward, ChunkDataTypePtr back, ChunkDataTypePtr left, ChunkDataTypePtr right)
{
	if (mChunkMesh.ConstructMesh(this, pPosition, forward, back, left, right))
	{
		pMeshState = ChunkMeshState::Built;
	}
	else
	{
		//std::cout << "Chunk is unbuilt" << std::endl;
		pMeshState = ChunkMeshState::Unbuilt;
	}
}

void Chunk::ConstructNoBind(ChunkDataTypePtr forward, ChunkDataTypePtr back, ChunkDataTypePtr left, ChunkDataTypePtr right)
{
	if (mChunkMesh.ConstructMeshNoBind(this, pPosition, forward, back, left, right))
	{
		pMeshState = ChunkMeshState::NeedsBinding;
	}
	else
	{
		std::cout << "Chunk is unbuilt" << std::endl;
		pMeshState = ChunkMeshState::Unbuilt;
	}
}

void Chunk::BindChunkMesh()
{
	mChunkMesh.BindConstructedMesh();
	pMeshState = ChunkMeshState::Built;
}


void Chunk::Construct(Chunk* chunks[4])
{
	if (mChunkMesh.CreateMesh(this, pPosition, chunks))
	{
		pMeshState = ChunkMeshState::Built;
	}else
	{
		pMeshState = ChunkMeshState::Unbuilt;
	}
}


ChunkMesh* Chunk::GetChunkMesh()
{
	return &mChunkMesh;
}

Block* Chunk::GetBlock(int x, int y, int z)
{
	return &pChunkContents[x][y][z];
}