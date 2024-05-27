#include "MeshThread.h"
MeshThread::MeshThread(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, std::shared_ptr<FPSCamera> camera, int world_seed, int render_dist, int build_dist)
{
	render_distance = render_dist;
	build_distance = build_dist;
	worldSeed = world_seed;
	threadRef = std::thread(&MeshThread::ThreadFunc, this, worldChunks, std::ref(camera));
	threadRef.detach();
}

void MeshThread::AddToSet(int x, int y)
{
	AddToSet(std::pair<int, int>(x, y));
}

void MeshThread::AddToSet(std::pair<int, int> chunk)
{
	mutex.lock();
	chunksToGen.insert(chunk);
	mutex.unlock();
}

void MeshThread::RemoveFromSet(std::pair<int, int> chunk)
{
	mutex.lock();
	chunksToGen.erase(chunk);
	mutex.unlock();
}

bool MeshThread::ChunkExistsInMap(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz)
{
	std::map<std::pair<int, int>, std::shared_ptr<Chunk>>::iterator chunk_exists = worldChunks->find(std::pair<int, int>(cx, cz));

	if (chunk_exists == worldChunks->end())
	{
		return false;
	}
	//std::cout << "Chunk exists in map" << std::endl;
	return true;
}


std::shared_ptr<Chunk> MeshThread::RetrieveChunkFromMap(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz)
{
	auto chunk = worldChunks->find(std::pair<int, int>(cx, cz));

	if (chunk == worldChunks->end())
	{
		//std::cout << "Failed to find chunk in world data - mesh thread version" << std::endl;
		return nullptr;
	}

	std::shared_ptr<Chunk> ret_val = worldChunks->at(std::pair<int, int>(cx, cz));
	return ret_val;
}

ChunkDataSharedPtr MeshThread::GetChunkDataForMeshing(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz)
{
	if (ChunkExistsInMap(worldChunks, cx, cz))
	{
		std::shared_ptr<Chunk> chunk = RetrieveChunkFromMap(worldChunks, cx, cz);
		if (chunk->pChunkState == ChunkState::Generated)
			return RetrieveChunkFromMap(worldChunks, cx, cz)->pChunkContentsPtr;
		
		//std::lock_guard<std::mutex> lock(RetrieveChunkFromMap(worldChunks, cx, cz)->chunkMutex);
		//return std::make_shared<std::array<std::array<std::array<Block, CHUNK_SIZE_X>, CHUNK_SIZE_Y>, CHUNK_SIZE_Z>>(RetrieveChunkFromMap(worldChunks, cx, cz)->pChunkContentsPtr);
	}

	return nullptr;
}

std::shared_ptr<Chunk> MeshThread::EmplaceChunkInMap(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz)
{
	std::map<std::pair<int, int>, std::shared_ptr<Chunk>>::iterator chunk_exists = worldChunks->find(std::pair<int, int>(cx, cz));

	if (chunk_exists == worldChunks->end())
	{
		//Timer timer(str.str());

		worldChunks->emplace(std::pair<int, int>(cx, cz), std::move(std::make_shared<Chunk>(glm::vec3(cx, 0, cz))));
		//m_ChunkCount++;
	}

	return worldChunks->at(std::pair<int, int>(cx, cz));
}

void MeshThread::ThreadFunc(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, std::shared_ptr<FPSCamera> camera)
{
	//std::this_thread::sleep_for(5s);
	while (true)
	{
		int player_chunk_x = 0;
		int player_chunk_z = 0;

		int build_distance = render_distance + 4;

		player_chunk_x = (int)floor(camera->GetPosition().x / CHUNK_SIZE_X);
		player_chunk_z = (int)floor(camera->GetPosition().z / CHUNK_SIZE_Z);


		for (int i = player_chunk_x - build_distance; i < player_chunk_x + render_distance; i++)
		{
			for (int j = player_chunk_z - build_distance; j < player_chunk_z + render_distance; j++)
			{
				
				World::worldMutex.lock();
				if (ChunkExistsInMap(worldChunks, i, j))
				{
					
					std::shared_ptr<Chunk> chunk = RetrieveChunkFromMap(worldChunks, i, j);
					if (chunk == nullptr) { World::worldMutex.unlock(); continue; }
					if (chunk->pChunkState == ChunkState::Ungenerated) {
						World::worldMutex.unlock();
						continue;
					}
					//this->loadedChunks.insert(chunk);
					//mutex.unlock();
					if (chunk->pMeshState == ChunkMeshState::Unbuilt && chunk->pChunkState == ChunkState::Generated)
						chunk->ConstructNoBind(chunk, GetChunkDataForMeshing(worldChunks, i, j + 1), GetChunkDataForMeshing(worldChunks, i, j - 1), GetChunkDataForMeshing(worldChunks, i + 1, j), GetChunkDataForMeshing(worldChunks, i - 1, j));
					
				}
				World::worldMutex.unlock();
			}
		}
		//std::cout << "Full Mesh Thread Run Complete" << std::endl;
		std::this_thread::sleep_for(16ms);
	}
}
