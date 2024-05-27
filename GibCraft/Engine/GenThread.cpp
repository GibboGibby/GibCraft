#include "GenThread.h"


GenThread::GenThread(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, std::shared_ptr<FPSCamera> camera, int world_seed, int render_dist, int build_dist)
{
	
	render_distance = render_dist;
	build_distance = build_dist;
	worldSeed = world_seed;
	threadRef = std::thread(&GenThread::ThreadFunc, this, worldChunks, std::ref(camera));
	threadRef.detach();
}

void GenThread::AddToSet(int x, int y)
{
	AddToSet(std::pair<int, int>(x, y));
}

void GenThread::AddToSet(std::pair<int, int> chunk)
{
	mutex.lock();
	chunksToGen.insert(chunk);
	mutex.unlock();
}

void GenThread::RemoveFromSet(std::pair<int, int> chunk)
{
	mutex.lock();
	chunksToGen.erase(chunk);
	mutex.unlock();
}

bool GenThread::ChunkExistsInMap(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz)
{
	std::map<std::pair<int, int>, std::shared_ptr<Chunk>>::iterator chunk_exists = worldChunks->find(std::pair<int, int>(cx, cz));

	if (chunk_exists == worldChunks->end())
	{
		return false;
	}

	return true;
}

std::shared_ptr<Chunk> GenThread::RetrieveChunkFromMap(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz)
{
	auto chunk = worldChunks->find(std::pair<int, int>(cx, cz));

	if (chunk == worldChunks->end())
	{
		//std::cout << "Failed to find chunk in world data - gen thread version" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Chunk> ret_val = worldChunks->at(std::pair<int, int>(cx, cz));
	return ret_val;
}

ChunkDataTypePtr GenThread::GetChunkDataForMeshing(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz)
{
	if (ChunkExistsInMap(worldChunks, cx, cz))
	{
		return &RetrieveChunkFromMap(worldChunks, cx, cz)->pChunkContents;
	}

	return nullptr;
}


std::shared_ptr<Chunk> GenThread::EmplaceChunkInMap(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz)
{
	std::map<std::pair<int, int>, std::shared_ptr<Chunk>>::iterator chunk_exists = worldChunks->find(std::pair<int, int>(cx, cz));

	if (chunk_exists == worldChunks->end())
	{
		//Timer timer(str.str());

		//worldChunks->emplace(std::pair<int, int>(cx, cz), new Chunk(glm::vec3(cx, 0, cz)));
		worldChunks->emplace(std::pair<int, int>(cx, cz), std::make_shared<Chunk>(glm::vec3(cx,0,cz)));
		//worldChunks->insert(std::pair<std::pair<int, int>, std::shared_ptr<Chunk>>(std::pair<int, int>(cx, cz), std::make_shared<Chunk>(glm::vec3(cx, 0, cz))));
		//m_ChunkCount++;
	}

	return worldChunks->at(std::pair<int, int>(cx, cz));
}

void GenThread::ThreadFunc(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, std::shared_ptr<FPSCamera> camera)
{
	
	while (true)
	{
		int player_chunk_x = 0;
		int player_chunk_z = 0;

		int build_distance = render_distance + 4;

		player_chunk_x = (int)floor(camera->GetPosition().x / CHUNK_SIZE_X);
		player_chunk_z = (int)floor(camera->GetPosition().z / CHUNK_SIZE_Z);


		for (int i = player_chunk_x - build_distance; i < player_chunk_x + build_distance; i++)
		{
			for (int j = player_chunk_z - build_distance; j < player_chunk_z + build_distance; j++)
			{
				World::worldMutex.lock();
				//std::cout << "chunk at - x: " << i << " z: " << j << std::endl;
				if (!ChunkExistsInMap(worldChunks, i, j))
				{
					//std::cout << "Chunk not found so I am generating a chunk" << std::endl;
					//World::worldMutex.lock();
					//std::shared_ptr<Chunk> chunk = EmplaceChunkInMap(worldChunks, i, j);
					//chunk->chunkMutex.lock();
					//WorldGen::GenerateChunk(chunk, worldSeed);
					//chunk->DisplayTopLayerOfChunk();
					//chunk->pChunkState = ChunkState::Generated;
					//chunk->chunkMutex.unlock();
					//World::worldMutex.unlock();
				}
				else
				{
					
					std::shared_ptr<Chunk> chunk = RetrieveChunkFromMap(worldChunks, i, j);
					if (chunk->pChunkState == ChunkState::Ungenerated)
					{
						
						//World::worldMutex.lock();
						//std::shared_ptr<Chunk> chunk = EmplaceChunkInMap(worldChunks, i, j);
						//chunk->chunkMutex.lock();
						for (int x = 0; x < CHUNK_SIZE_X; x++)
						{
							for (int y = 0; y < CHUNK_SIZE_Y; y++)
							{
								for (int z = 0; z < CHUNK_SIZE_Z; z++)
								{
									chunk->SetBlock(BlockType::AIR, glm::vec3(x, y, z));
								}
							}
						}
						WorldGen::GenerateChunk(chunk, worldSeed);
						chunk->pChunkState = ChunkState::Generated;
						//chunk->chunkMutex.unlock();
						//World::worldMutex.unlock();
					}
				}
				World::worldMutex.unlock();
			}
		}
		//std::cout << "Full Gen Thread Run Complete" << std::endl;
		std::this_thread::sleep_for(16ms);
	}
}
