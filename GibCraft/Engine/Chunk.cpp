#include "Chunk.h"

Chunk::Chunk(const glm::vec3 chunkPos) : pPosition(chunkPos), pMeshState(ChunkMeshState::Unbuilt), pChunkState(ChunkState::Ungenerated), p_ChunkFrustummAABB(glm::vec3(CHUNK_SIZE_X, CHUNK_SIZE_Y, CHUNK_SIZE_Z), glm::vec3(chunkPos.x* CHUNK_SIZE_X, chunkPos.y* CHUNK_SIZE_Y, chunkPos.z* CHUNK_SIZE_Z))

{
	//memset(&pChunkContents, BlockType::AIR, CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z);
	//std::fill_n(&pChunkContents[0][0][0], CHUNK_SIZE_X * CHUNK_SIZE_Y * CHUNK_SIZE_Z, BlockType::AIR);
	//auto start = std::chrono::system_clock::now();
	pChunkContentsPtr = std::make_shared<std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>>();
	//std::fill(pChunkContentsPtr->begin(), pChunkContentsPtr->end(), BlockType::AIR);
	mChunkMesh = std::make_shared<ChunkMesh>();
	/*
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
	*/
	memset(&p_HeightMap, 0, CHUNK_SIZE_X * CHUNK_SIZE_Z * sizeof(std::uint8_t));
	//pChunkState == ChunkState::Ungenerated;
	//auto end = std::chrono::system_clock::now();
	//std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << " - Time it took for a single chunk to be initialised" << std::endl;
}

Chunk::~Chunk()
{

}

void Chunk::SetBlock(BlockType type, const glm::vec3& position)
{
	Block b;
	b.type = type;

	//pChunkContents.at(position.x).at(position.y).at(position.z) = b;
	pChunkContentsPtr->at(position.x).at(position.y).at(position.z) = b;
}

void Chunk::Construct(std::shared_ptr<Chunk> chunk, ChunkDataTypePtr forward, ChunkDataTypePtr back, ChunkDataTypePtr left, ChunkDataTypePtr right)
{
	if (mChunkMesh->ConstructMesh(chunk, pPosition, forward, back, left, right))
	{
		pMeshState = ChunkMeshState::Built;
	}
	else
	{
		//
		//  << "Chunk is unbuilt" << std::endl;
		pMeshState = ChunkMeshState::Unbuilt;
	}
}

void Chunk::ConstructNoBind(std::shared_ptr<Chunk> chunk, ChunkDataSharedPtr forward, ChunkDataSharedPtr back, ChunkDataSharedPtr left, ChunkDataSharedPtr right)
{
	chunk->chunkMutex.lock();
	if (mChunkMesh->ConstructMeshNoBind(chunk, pPosition, forward, back, left, right))
	{
		chunk->pMeshState = ChunkMeshState::NeedsBinding;
	}
	else
	{
		//std::cout << "Chunk is unbuilt - Chunk" << std::endl;
		//chunk->pMeshState = ChunkMeshState::Unbuilt;
	}
	chunk->chunkMutex.unlock();
}

void Chunk::BindChunkMesh()
{
	mChunkMesh->BindConstructedMesh();
	pMeshState = ChunkMeshState::Built;
}


void Chunk::Construct(std::shared_ptr<Chunk> chunk, std::shared_ptr<Chunk> chunks[4])
{
	if (mChunkMesh->CreateMesh(chunk, pPosition, chunks))
	{
		pMeshState = ChunkMeshState::Built;
	}else
	{
		pMeshState = ChunkMeshState::Unbuilt;
	}
}



std::shared_ptr<ChunkMesh> Chunk::GetChunkMesh()
{
	return mChunkMesh;
}

int Chunk::DisplayTopLayerOfChunk()
{
	std::cout << "chunk at x - " + std::to_string(pPosition.x) + " y - " + std::to_string(pPosition.z);
	int checkingY = 0;
	for (int y = 254; y > 0; y--)
	{
		if (pChunkContentsPtr->at(0).at(y).at(0).type == BlockType::GRASS)
		{
			if (checkingY == 0)
				checkingY = y;
		}
	}
	std::cout << "with a y of - " << checkingY << std::endl;
	for (int i = 0; i < 16; i++)
	{
		std::string line = "";
		for (int j = 0; j < 16; j++)
		{
			switch (pChunkContentsPtr->at(i).at(checkingY).at(j).type)
			{
			case BlockType::AIR:
				line += "air ";
				break;
			case BlockType::DIRT:
				line += "dirt ";
				break;
			case BlockType::COBBLESTONE:
				line += "cobble ";
				break;
			case BlockType::GRASS:
				line += "grass ";
				break;
			case BlockType::OAK_PLANKS:
				line += "oak-plank ";
				break;
			case BlockType::STONE:
				line += "stone ";
				break;
			default:
				line += "none ";
				break;
			}
		}
		std::cout << line << std::endl;
	}
	std::cout << "--------------------------------------------\n";
	return checkingY;
}

Block* Chunk::GetBlock(int x, int y, int z)
{
	return &pChunkContents[x][y][z];
}