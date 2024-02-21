#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <array>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ChunkMesh.h"


enum class ChunkMeshState : std::uint8_t
{
	Built = 0,
	Unbuilt,
	Building,
	NeedsBuilding,
	NeedsBinding,
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
	
	void Construct(ChunkDataTypePtr forward, ChunkDataTypePtr back, ChunkDataTypePtr left, ChunkDataTypePtr right);
	void ConstructNoBind(ChunkDataTypePtr forward, ChunkDataTypePtr back, ChunkDataTypePtr left, ChunkDataTypePtr right);
	void BindChunkMesh();
	void Construct(Chunk* chunks[4]);
	ChunkMesh* GetChunkMesh();

	Block* GetBlock(int x, int y, int z);

	const glm::vec3 pPosition;
	ChunkMeshState pMeshState;
	ChunkState pChunkState = ChunkState::Ungenerated;
	std::array<std::array<uint8_t, CHUNK_SIZE_X>, CHUNK_SIZE_Z> p_HeightMap;
	std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> pChunkContents;

private:
	ChunkMesh mChunkMesh;
};