#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <array>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ChunkMesh.h"


enum class ChunkMeshState : std::uint8_t
{
	Built = 0,
	Unbuilt,
	Error
};

enum class ChunkState
{
	Ungenerated = 5,
	Generated,
	Changed,
	undefined
};
class Chunk
{
public:
	Chunk(const glm::vec3 chunkPos);
	~Chunk();

	void SetBlock(BlockType type, const glm::vec3& position);
	
	void Construct(Chunk* forward, Chunk* back, Chunk* left, Chunk* right);
	void Construct();
	ChunkMesh* GetChunkMesh();

	Block* GetBlock(int x, int y, int z);

	const glm::vec3 pPosition;
	ChunkMeshState pMeshState;
	ChunkState pChunkState = ChunkState::Ungenerated;
	std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> pChunkContents;

private:
	ChunkMesh mChunkMesh;
};