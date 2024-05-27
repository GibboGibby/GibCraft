#include "ChunkMesh.h"
#include "Chunk.h"
#include <iostream>
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

ChunkMesh::ChunkMesh() : mVBO(GL_ARRAY_BUFFER)
{

	//std::cout << "Max size of the vertices vector - " << mVertices.max_size();
	static bool IndexBufferInitialized = false;

	// Static index buffer
	static IndexBuffer StaticIBO;

	if (IndexBufferInitialized == false)
	{
		IndexBufferInitialized = true;

		GLuint* IndexBuffer = nullptr;

		int index_size = CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z * 6;
		int index_offset = 0;

		IndexBuffer = new GLuint[index_size * 6];
		//std::cout << "index size - " << index_size << std::endl;
		for (size_t i = 0; i < index_size; i += 6)
		{
			IndexBuffer[i] = 0 + index_offset;
			IndexBuffer[i + 1] = 1 + index_offset;
			IndexBuffer[i + 2] = 2 + index_offset;
			IndexBuffer[i + 3] = 2 + index_offset;
			IndexBuffer[i + 4] = 3 + index_offset;
			IndexBuffer[i + 5] = 0 + index_offset;

			index_offset = index_offset + 4;
		}

		StaticIBO.BufferData(index_size * 6 * sizeof(GLuint), IndexBuffer, GL_STATIC_DRAW);

		delete[] IndexBuffer;
	}

	pVAO.Bind();
	mVBO.Bind();
	StaticIBO.Bind();
	mVBO.VertexAttribIPointer(0, 3, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	mVBO.VertexAttribIPointer(1, 2, GL_UNSIGNED_SHORT, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));
	pVAO.Unbind();

	m_ForwardFace[0] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	m_ForwardFace[1] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	m_ForwardFace[2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_ForwardFace[3] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

	m_BackFace[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_BackFace[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_BackFace[2] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	m_BackFace[3] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	m_TopFace[0] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	m_TopFace[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	m_TopFace[2] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_TopFace[3] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

	m_BottomFace[0] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_BottomFace[1] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_BottomFace[2] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
	m_BottomFace[3] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	m_LeftFace[0] = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	m_LeftFace[1] = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	m_LeftFace[2] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	m_LeftFace[3] = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);

	m_RightFace[0] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	m_RightFace[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	m_RightFace[2] = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	m_RightFace[3] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
}

bool ChunkMesh::ConstructMesh(std::shared_ptr<Chunk> chunk, const glm::vec3& chunk_pos, ChunkDataTypePtr ForwardChunkData, ChunkDataTypePtr BackwardChunkData, ChunkDataTypePtr RightChunkData, ChunkDataTypePtr LeftChunkData)
{
	ChunkDataTypePtr ChunkData = &chunk->pChunkContents;

	glm::vec3 world_position;
	glm::vec3 local_position;
	mVertices.clear();

	

	if (ForwardChunkData && BackwardChunkData && RightChunkData && LeftChunkData)
	{
		
		for (int x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (int y = 0; y < CHUNK_SIZE_Y; y++)
			{
				for (int z = 0; z < CHUNK_SIZE_Z; z++)
				{
					if (ChunkData->at(x).at(y).at(z).type != BlockType::AIR)
					{
						Block* block = &ChunkData->at(x).at(y).at(z);

						// To fix chunk edge mesh building issues, both faces are added if it is in the edge

						world_position.x = chunk_pos.x * CHUNK_SIZE_X + x;
						world_position.y = 0 * CHUNK_SIZE_Y + y;
						world_position.z = chunk_pos.z * CHUNK_SIZE_Z + z;
						local_position = glm::vec3(x, y, z);


						if (z <= 0)
						{
							if (block->IsTransparent())
							{
								if (BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).IsTransparent() &&
									BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false);
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}

								else if (ChunkData->at(x).at(y).at(1).IsTransparent() &&
									ChunkData->at(x).at(y).at(1).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false); 
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}
							}

							else
							{
								if (BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}

								else if (ChunkData->at(x).at(y).at(1).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}
							}
						}

						else if (z >= CHUNK_SIZE_Z - 1)
						{
							if (block->IsTransparent())
							{
								if (ForwardChunkData->at(x).at(y).at(0).IsTransparent() &&
									ForwardChunkData->at(x).at(y).at(0).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false);
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}

								else if (ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).IsTransparent() &&
									ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false);
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}
							}

							else
							{
								if (ForwardChunkData->at(x).at(y).at(0).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}

								else if (ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}
							}
						}

						else
						{
							if (block->IsTransparent())
							{
								if (ChunkData->at(x).at(y).at(z + 1).IsTransparent() &&
									ChunkData->at(x).at(y).at(z + 1).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false);
								}

								if (ChunkData->at(x).at(y).at(z - 1).IsTransparent() &&
									ChunkData->at(x).at(y).at(z - 1).type != block->type)
								{
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}
							}

							else
							{
								//If the forward block is an air block, add the forward face to the mesh
								if (ChunkData->at(x).at(y).at(z + 1).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
								}

								// If the back (-forward) block is an air block, add the back face to the mesh
								if (ChunkData->at(x).at(y).at(z - 1).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}
							}
						}

						if (x <= 0)
						{
							if (block->IsTransparent())
							{
								if (LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).IsTransparent() &&
									LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
								}

								else if (ChunkData->at(1).at(y).at(z).IsTransparent() &&
									ChunkData->at(1).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
								}

							}

							else
							{
								if (LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
								}

								else if (ChunkData->at(1).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
								}
							}
						}

						else if (x >= CHUNK_SIZE_X - 1)
						{
							if (block->IsTransparent())
							{
								if (RightChunkData->at(0).at(y).at(z).IsTransparent() &&
									RightChunkData->at(0).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
								}

								else if (ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).IsTransparent() &&
									ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
								}
							}

							else
							{
								if (RightChunkData->at(0).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
								}

								else if (ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
								}
							}

						}

						else
						{
							if (block->IsTransparent())
							{
								if (ChunkData->at(x + 1).at(y).at(z).IsTransparent() &&
									ChunkData->at(x + 1).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
								}

								if (ChunkData->at(x - 1).at(y).at(z).IsTransparent() &&
									ChunkData->at(x - 1).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
								}
							}

							else
							{
								// If the next block is an air block, add the right face to the mesh
								if (ChunkData->at(x + 1).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
								}

								// If the previous block is an air block, add the left face to the mesh
								if (ChunkData->at(x - 1).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
								}
							}
						}

						if (y <= 0)
						{
							if (ChunkData->at(x).at(y + 1).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::BOTTOM, local_position, block->type);
							}
						}

						else if (y >= CHUNK_SIZE_Y - 1)
						{
							AddFace(chunk, BlockFace::TOP, local_position, block->type);
						}

						else
						{
							if (block->IsTransparent())
							{
								if (ChunkData->at(x).at(y - 1).at(z).IsTransparent() &&
									ChunkData->at(x).at(y - 1).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::BOTTOM, local_position, block->type, false);
								}

								if (ChunkData->at(x).at(y + 1).at(z).IsTransparent() &&
									ChunkData->at(x).at(y + 1).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::TOP, local_position, block->type, false);
								}
							}

							else
							{
								// If the top block is an air block, add the top face to the mesh
								if (ChunkData->at(x).at(y - 1).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::BOTTOM, local_position, block->type);
								}

								// If the bottom block is an air block, add the top face to the mesh
								if (ChunkData->at(x).at(y + 1).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::TOP, local_position, block->type);
								}
							}
						}
					}
				}
			}
		}

		// Upload the data to the GPU whenever the mesh is reconstructed

		p_VerticesCount = 0;

		if (mVertices.size() > 0)
		{
			mVBO.BufferData(this->mVertices.size() * sizeof(Vertex), &this->mVertices.front(), GL_STATIC_DRAW);
			p_VerticesCount = mVertices.size();
			mVertices.clear();
		}
		return true;
	}

	//std::cout << "One of chunk dir is nullptr" << std::endl;
	return false;
}

bool ChunkMesh::ConstructMeshNoBind(std::shared_ptr<Chunk> chunk, const glm::vec3& chunk_pos, ChunkDataSharedPtr ForwardChunkData, ChunkDataSharedPtr BackwardChunkData, ChunkDataSharedPtr RightChunkData, ChunkDataSharedPtr LeftChunkData)
{
	ChunkDataSharedPtr ChunkData = chunk->pChunkContentsPtr;

	glm::vec3 world_position;
	glm::vec3 local_position;
	mVertices.clear();


	//if (!ForwardChunkData) std::cout << "foward bad" << std::endl;
	//if (!BackwardChunkData) std::cout << "backward bad" << std::endl;
	//if (!RightChunkData) std::cout << "right bad" << std::endl;
	//if (!LeftChunkData) std::cout << "left bad" << std::endl;
	if (ForwardChunkData && BackwardChunkData && RightChunkData && LeftChunkData)
	{
		std::cout << "chunk mesh has in fact been generated" << std::endl;
		for (int x = 0; x < CHUNK_SIZE_X; x++)
		{
			for (int y = 0; y < CHUNK_SIZE_Y; y++)
			{
				for (int z = 0; z < CHUNK_SIZE_Z; z++)
				{
					if (ChunkData->at(x).at(y).at(z).type != BlockType::AIR)
					{
						Block* block = &ChunkData->at(x).at(y).at(z);

						// To fix chunk edge mesh building issues, both faces are added if it is in the edge

						world_position.x = chunk_pos.x * CHUNK_SIZE_X + x;
						world_position.y = 0 * CHUNK_SIZE_Y + y;
						world_position.z = chunk_pos.z * CHUNK_SIZE_Z + z;
						local_position = glm::vec3(x, y, z);


						if (z <= 0)
						{
							if (block->IsTransparent())
							{
								if (BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).IsTransparent() &&
									BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false);
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}

								else if (ChunkData->at(x).at(y).at(1).IsTransparent() &&
									ChunkData->at(x).at(y).at(1).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false);
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}
							}

							else
							{
								if (BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}

								else if (ChunkData->at(x).at(y).at(1).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}
							}
						}

						else if (z >= CHUNK_SIZE_Z - 1)
						{
							if (block->IsTransparent())
							{
								if (ForwardChunkData->at(x).at(y).at(0).IsTransparent() &&
									ForwardChunkData->at(x).at(y).at(0).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false);
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}

								else if (ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).IsTransparent() &&
									ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false);
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}
							}

							else
							{
								if (ForwardChunkData->at(x).at(y).at(0).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}

								else if (ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}
							}
						}

						else
						{
							if (block->IsTransparent())
							{
								if (ChunkData->at(x).at(y).at(z + 1).IsTransparent() &&
									ChunkData->at(x).at(y).at(z + 1).type != block->type)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type, false);
								}

								if (ChunkData->at(x).at(y).at(z - 1).IsTransparent() &&
									ChunkData->at(x).at(y).at(z - 1).type != block->type)
								{
									AddFace(chunk, BlockFace::BACK, local_position, block->type, false);
								}
							}

							else
							{
								//If the forward block is an air block, add the forward face to the mesh
								if (ChunkData->at(x).at(y).at(z + 1).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::FRONT, local_position, block->type);
								}

								// If the back (-forward) block is an air block, add the back face to the mesh
								if (ChunkData->at(x).at(y).at(z - 1).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::BACK, local_position, block->type);
								}
							}
						}

						if (x <= 0)
						{
							if (block->IsTransparent())
							{
								if (LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).IsTransparent() &&
									LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
								}

								else if (ChunkData->at(1).at(y).at(z).IsTransparent() &&
									ChunkData->at(1).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
								}

							}

							else
							{
								if (LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
								}

								else if (ChunkData->at(1).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
								}
							}
						}

						else if (x >= CHUNK_SIZE_X - 1)
						{
							if (block->IsTransparent())
							{
								if (RightChunkData->at(0).at(y).at(z).IsTransparent() &&
									RightChunkData->at(0).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
								}

								else if (ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).IsTransparent() &&
									ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
								}
							}

							else
							{
								if (RightChunkData->at(0).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
								}

								else if (ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
								}
							}

						}

						else
						{
							if (block->IsTransparent())
							{
								if (ChunkData->at(x + 1).at(y).at(z).IsTransparent() &&
									ChunkData->at(x + 1).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type, false);
								}

								if (ChunkData->at(x - 1).at(y).at(z).IsTransparent() &&
									ChunkData->at(x - 1).at(y).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type, false);
								}
							}

							else
							{
								// If the next block is an air block, add the right face to the mesh
								if (ChunkData->at(x + 1).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::RIGHT, local_position, block->type);
								}

								// If the previous block is an air block, add the left face to the mesh
								if (ChunkData->at(x - 1).at(y).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::LEFT, local_position, block->type);
								}
							}
						}

						if (y <= 0)
						{
							if (ChunkData->at(x).at(y + 1).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::BOTTOM, local_position, block->type);
							}
						}

						else if (y >= CHUNK_SIZE_Y - 1)
						{
							AddFace(chunk, BlockFace::TOP, local_position, block->type);
						}

						else
						{
							if (block->IsTransparent())
							{
								if (ChunkData->at(x).at(y - 1).at(z).IsTransparent() &&
									ChunkData->at(x).at(y - 1).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::BOTTOM, local_position, block->type, false);
								}

								if (ChunkData->at(x).at(y + 1).at(z).IsTransparent() &&
									ChunkData->at(x).at(y + 1).at(z).type != block->type)
								{
									AddFace(chunk, BlockFace::TOP, local_position, block->type, false);
								}
							}

							else
							{
								// If the top block is an air block, add the top face to the mesh
								if (ChunkData->at(x).at(y - 1).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::BOTTOM, local_position, block->type);
								}

								// If the bottom block is an air block, add the top face to the mesh
								if (ChunkData->at(x).at(y + 1).at(z).IsOpaque() == false)
								{
									AddFace(chunk, BlockFace::TOP, local_position, block->type);
								}
							}
						}
					}
				}
			}
		}

		// Upload the data to the GPU whenever the mesh is reconstructed

		
		return true;
	}

	//std::cout << "One of chunk dir is nullptr" << std::endl;
	return false;
}

void ChunkMesh::BindConstructedMesh()
{
	p_VerticesCount = 0;

	if (mVertices.size() > 0)
	{
		mVBO.BufferData(this->mVertices.size() * sizeof(Vertex), &this->mVertices.front(), GL_STATIC_DRAW);
		p_VerticesCount = mVertices.size();
		mVertices.clear();
	}
}

bool ChunkMesh::CreateMesh(std::shared_ptr<Chunk> chunk, const glm::vec3& chunk_pos, std::shared_ptr<Chunk> chunks[4])
{
	ChunkDataTypePtr ChunkData = &chunk->pChunkContents;
	glm::vec3 worldPos;
	glm::vec3 localPos;
	mVertices.clear();
	int numVertexes = 0;
	std::shared_ptr<Chunk> forwardChunk = chunks[0];
	std::shared_ptr<Chunk> backwardChunk = chunks[1];
	std::shared_ptr<Chunk> leftChunk = chunks[2];
	std::shared_ptr<Chunk> rightChunk = chunks[3];

	ChunkDataTypePtr ForwardChunkData = &forwardChunk->pChunkContents;
	ChunkDataTypePtr BackwardChunkData = &backwardChunk->pChunkContents;
	ChunkDataTypePtr RightChunkData = &rightChunk->pChunkContents;
	ChunkDataTypePtr LeftChunkData = &leftChunk->pChunkContents;
	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				if (ChunkData->at(x).at(y).at(z).type != BlockType::AIR)
				{
					Block* block = &ChunkData->at(x).at(y).at(z);


					worldPos.x = chunk_pos.x * CHUNK_SIZE_X + x;
					worldPos.y = 0 * CHUNK_SIZE_Y + y;
					worldPos.z = chunk_pos.z * CHUNK_SIZE_Z + z;
					localPos = glm::vec3(x, y, z);

					if (z <= 0)
					{
						if (block->IsTransparent())
						{
							if (BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).IsTransparent() && BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).type != block->type)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}
							else if (ChunkData->at(x).at(y).at(1).IsTransparent() && ChunkData->at(x).at(y).at(1).type != block->type)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}
						}
						else
						{
							if (BackwardChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 1).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}

							else if (ChunkData->at(x).at(y).at(1).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}
						}
					}
					else if (z >= CHUNK_SIZE_Z - 1)
					{
						if (block->IsTransparent())
						{
							if (ForwardChunkData->at(x).at(y).at(0).IsTransparent() &&
								ForwardChunkData->at(x).at(y).at(0).type != block->type)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}

							else if (ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).IsTransparent() &&
								ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).type != block->type)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}
						}
						else
						{
							if (ForwardChunkData->at(x).at(y).at(0).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}

							else if (ChunkData->at(x).at(y).at(CHUNK_SIZE_Z - 2).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}
						}
					}
					else
					{
						if (block->IsTransparent())
						{
							if (ChunkData->at(x).at(y).at(z + 1).IsTransparent() &&
								ChunkData->at(x).at(y).at(z + 1).type != block->type)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
							}

							if (ChunkData->at(x).at(y).at(z - 1).IsTransparent() &&
								ChunkData->at(x).at(y).at(z - 1).type != block->type)
							{
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}
						}

						else
						{
							//If the forward block is an air block, add the forward face to the mesh
							if (ChunkData->at(x).at(y).at(z + 1).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
							}

							// If the back (-forward) block is an air block, add the back face to the mesh
							if (ChunkData->at(x).at(y).at(z - 1).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
							}
						}
					}

					if (x <= 0)
					{
						if (block->IsTransparent())
						{
							if (LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).IsTransparent() &&
								LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).type != block->type)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}

							else if (ChunkData->at(1).at(y).at(z).IsTransparent() &&
								ChunkData->at(1).at(y).at(z).type != block->type)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}
						}
						else
						{
							if (LeftChunkData->at(CHUNK_SIZE_X - 1).at(y).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}

							else if (ChunkData->at(1).at(y).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}
						}
					}
					else if (x >= CHUNK_SIZE_X - 1)
					{
						if (block->IsTransparent())
						{
							if (RightChunkData->at(0).at(y).at(z).IsTransparent() &&
								RightChunkData->at(0).at(y).at(z).type != block->type)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}

							else if (ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).IsTransparent() &&
								ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).type != block->type)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}
						}

						else
						{
							if (RightChunkData->at(0).at(y).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}

							else if (ChunkData->at(CHUNK_SIZE_X - 2).at(y).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}
						}
					}
					else
					{
						if (block->IsTransparent())
						{
							if (ChunkData->at(x + 1).at(y).at(z).IsTransparent() &&
								ChunkData->at(x + 1).at(y).at(z).type != block->type)
							{
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}

							if (ChunkData->at(x - 1).at(y).at(z).IsTransparent() &&
								ChunkData->at(x - 1).at(y).at(z).type != block->type)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
							}
						}

						else
						{
							// If the next block is an air block, add the right face to the mesh
							if (ChunkData->at(x + 1).at(y).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
							}

							// If the previous block is an air block, add the left face to the mesh
							if (ChunkData->at(x - 1).at(y).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
							}
						}
					}

					if (y <= 0)
					{
						if (ChunkData->at(x).at(y + 1).at(z).IsOpaque() == false)
						{
							AddFace(chunk, BlockFace::BOTTOM, localPos, block->type, true);
						}
					}

					else if (y >= CHUNK_SIZE_Y - 1)
					{
						AddFace(chunk, BlockFace::TOP, localPos, block->type, true);
					}

					else
					{
						if (block->IsTransparent())
						{
							if (ChunkData->at(x).at(y - 1).at(z).IsTransparent() &&
								ChunkData->at(x).at(y - 1).at(z).type != block->type)
							{
								AddFace(chunk, BlockFace::BOTTOM, localPos, block->type, true);
							}

							if (ChunkData->at(x).at(y + 1).at(z).IsTransparent() &&
								ChunkData->at(x).at(y + 1).at(z).type != block->type)
							{
								AddFace(chunk, BlockFace::TOP, localPos, block->type, true);
							}
						}

						else
						{
							// If the top block is an air block, add the top face to the mesh
							if (ChunkData->at(x).at(y - 1).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::BOTTOM, localPos, block->type, true);
							}

							// If the bottom block is an air block, add the top face to the mesh
							if (ChunkData->at(x).at(y + 1).at(z).IsOpaque() == false)
							{
								AddFace(chunk, BlockFace::TOP, localPos, block->type, true);
							}
						}
					}


					/*

					if (z >= 0 && z <= CHUNK_SIZE_Z)
					{
						AddFace(chunk, BlockFace::FRONT, localPos, block->type, true);
						AddFace(chunk, BlockFace::BACK, localPos, block->type, true);
						numVertexes += 8;
					}

					if (x >= 0 && x <= CHUNK_SIZE_X-1)
					{
						AddFace(chunk, BlockFace::LEFT, localPos, block->type, true);
						AddFace(chunk, BlockFace::RIGHT, localPos, block->type, true);
						numVertexes += 8;
					}

					if (y >= 0 && y <= CHUNK_SIZE_Y - 1)
					{
						AddFace(chunk, BlockFace::TOP, localPos, block->type, true);
						AddFace(chunk, BlockFace::BOTTOM, localPos, block->type, true);
						numVertexes += 8;
					}
					*/
					
				}
			}
		}
		

	}
	p_VerticesCount = 0;

	if (mVertices.size() > 0)
	{
		mVBO.BufferData(this->mVertices.size() * sizeof(Vertex), &this->mVertices.front(), GL_STATIC_DRAW);
		p_VerticesCount = mVertices.size();
		mVertices.clear();
	}
	return true;
}

void ChunkMesh::AddFace(std::shared_ptr<Chunk> chunk, BlockFace face, const glm::vec3& position, BlockType type, bool buffer)
{
	glm::vec4 translation = glm::vec4(position, 0.0f);
	Vertex v1, v2, v3, v4;
	bool reverse_texture_coordinates = false;

	switch (face)
	{
	case BlockFace::TOP:
		v1.position = translation + m_TopFace[0];
		v2.position = translation + m_TopFace[1];
		v3.position = translation + m_TopFace[2];
		v4.position = translation + m_TopFace[3];
		break;
	case BlockFace::BOTTOM:
		v1.position = translation + m_BottomFace[3];
		v2.position = translation + m_BottomFace[2];
		v3.position = translation + m_BottomFace[1];
		v4.position = translation + m_BottomFace[0];

		reverse_texture_coordinates = true;
		break;
	case BlockFace::LEFT:
		v1.position = translation + m_LeftFace[3];
		v2.position = translation + m_LeftFace[2];
		v3.position = translation + m_LeftFace[1];
		v4.position = translation + m_LeftFace[0];
		reverse_texture_coordinates = true;
		break;
	case BlockFace::RIGHT:
		v1.position = translation + m_RightFace[0];
		v2.position = translation + m_RightFace[1];
		v3.position = translation + m_RightFace[2];
		v4.position = translation + m_RightFace[3];
		break;
	case BlockFace::FRONT:
		v1.position = translation + m_ForwardFace[3];
		v2.position = translation + m_ForwardFace[2];
		v3.position = translation + m_ForwardFace[1];
		v4.position = translation + m_ForwardFace[0];

		reverse_texture_coordinates = true;
		break;
	case BlockFace::BACK:
		v1.position = translation + m_BackFace[0];
		v2.position = translation + m_BackFace[1];
		v3.position = translation + m_BackFace[2];
		v4.position = translation + m_BackFace[3];
		break;

	}

	const std::array<uint16_t, 8>& TextureCoordinates = BlockDatabase::GetBlockTexture(type, face);

	if (reverse_texture_coordinates)
	{
		v1.texture_coords = { TextureCoordinates[6], TextureCoordinates[7] };
		v2.texture_coords = { TextureCoordinates[4], TextureCoordinates[5] };
		v3.texture_coords = { TextureCoordinates[2], TextureCoordinates[3] };
		v4.texture_coords = { TextureCoordinates[0], TextureCoordinates[1] };
	}
	else
	{
		v1.texture_coords = { TextureCoordinates[0], TextureCoordinates[1] };
		v2.texture_coords = { TextureCoordinates[2], TextureCoordinates[3] };
		v3.texture_coords = { TextureCoordinates[4], TextureCoordinates[5] };
		v4.texture_coords = { TextureCoordinates[6], TextureCoordinates[7] };
	}

	//std::cout << "These are the texture coords for this faces v1: x - " << v1.texture_coords.x << "  | y - " << v1.texture_coords.y << std::endl;
	if (buffer)
	{
		mVertices.push_back(v1);
		mVertices.push_back(v2);
		mVertices.push_back(v3);
		mVertices.push_back(v4);
	}
	else
	{
		// For Transparent Vertices
	}
}
