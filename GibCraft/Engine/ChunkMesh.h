#pragma once
#include "BlockDatabase.h"
#include "Block.h"
#include<array>
#include <glm/ext.hpp>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderClass.h"
#include "Vertex.h"
#include <vector>


class Chunk;
typedef std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>* ChunkDataTypePtr;
typedef std::shared_ptr<std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>> ChunkDataSharedPtr;
ChunkDataTypePtr _GetChunkDataForMeshing(int cx, int cz);

class ChunkMesh
{
private:
	//std::array<Block, 3> mChunkData = {BlockType::DIRT, BlockType::OAK_PLANKS, BlockType::GRASS};
	std::vector<Vertex> mVertices;

public:
	ChunkMesh();

	bool CreateMesh(std::shared_ptr<Chunk> chunk, const glm::vec3& chunk_pos, std::shared_ptr<Chunk> chunks[4]);

	bool ConstructMesh(std::shared_ptr<Chunk> chunk, const glm::vec3& chunk_pos, ChunkDataTypePtr forwardChunk, ChunkDataTypePtr backChunk, ChunkDataTypePtr leftChunk, ChunkDataTypePtr rightChunk);
	bool ConstructMeshNoBind(std::shared_ptr<Chunk> chunk, const glm::vec3& chunk_pos, ChunkDataSharedPtr forwardChunk, ChunkDataSharedPtr backChunk, ChunkDataSharedPtr leftChunk, ChunkDataSharedPtr rightChunk);

	void BindConstructedMesh();

	//void AddCube();

	void AddFace(std::shared_ptr<Chunk> chunk, BlockFace face, const glm::vec3& position, BlockType type, bool buffer = true);
	VertexArray pVAO;
	std::uint32_t p_VerticesCount;

private:
	glm::vec4 m_TopFace[4];
	glm::vec4 m_BottomFace[4];
	glm::vec4 m_ForwardFace[4];
	glm::vec4 m_BackFace[4];
	glm::vec4 m_LeftFace[4];
	glm::vec4 m_RightFace[4];

	VertexBuffer mVBO;

};