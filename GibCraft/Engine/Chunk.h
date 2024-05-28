#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <array>
#include <algorithm>
#include "Frustum.h"
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ChunkMesh.h"

#include <mutex>

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
	std::mutex chunkMutex;
	Chunk(const glm::vec3 chunkPos);
	~Chunk();

	void SetBlock(BlockType type, const glm::vec3& position);
	
	void Construct(std::shared_ptr<Chunk> chunk,  ChunkDataTypePtr forward, ChunkDataTypePtr back, ChunkDataTypePtr left, ChunkDataTypePtr right);
	void ConstructNoBind(std::shared_ptr<Chunk> chunk, ChunkDataSharedPtr forward, ChunkDataSharedPtr back, ChunkDataSharedPtr left, ChunkDataSharedPtr right);
	void BindChunkMesh();
	void Construct(std::shared_ptr<Chunk> chunk, std::shared_ptr<Chunk> chunks[4]);
	std::shared_ptr<ChunkMesh> GetChunkMesh();

	int DisplayTopLayerOfChunk();

	Block* GetBlock(int x, int y, int z);

	const glm::vec3 pPosition;
	ChunkMeshState pMeshState;
	ChunkState pChunkState = ChunkState::Ungenerated;
	std::array<std::array<uint8_t, CHUNK_SIZE_X>, CHUNK_SIZE_Z> p_HeightMap;
	std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z> pChunkContents;
	std::shared_ptr<std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>> pChunkContentsPtr;
	std::shared_ptr<std::array<std::array<std::array<uint8_t, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>> pChunkLightInformation;

	FrustumAABB p_ChunkFrustummAABB;

private:
	std::shared_ptr<ChunkMesh> mChunkMesh;
};