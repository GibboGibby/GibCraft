#include "Chunk.h"

Chunk::Chunk(const glm::vec3 chunkPos) : pPosition(chunkPos), pMeshState(ChunkMeshState::Unbuilt), pChunkState(ChunkState::Ungenerated)
{
	memset(&pChunkContents, BlockType::AIR, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
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
		std::cout << "Chunk is unbuilt" << std::endl;
		pMeshState = ChunkMeshState::Unbuilt;
	}
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