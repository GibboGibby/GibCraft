#include "World.h"


World::World(int seed, const glm::vec2& window_size, const std::string& world_name) : m_WorldSeed(seed)
{
	//Init();
}

World::World() : m_WorldSeed(0)
{
	//Init();
}

void World::Init()
{
	for (int chunk_x = -15; chunk_x < 15; chunk_x++)
	{
		for (int chunk_y = -15; chunk_y < 15; chunk_y++)
		{
			//Y is actually Z
			Chunk* chunk = EmplaceChunkInMap(chunk_x, chunk_y);
			for (int x = 0; x < CHUNK_SIZE_X; x++)
			{
				for (int y = 0; y < 128; y++)
				{
					for (int z = 0; z < CHUNK_SIZE_Z; z++)
					{
						chunk->SetBlock(OAK_PLANKS, glm::vec3(x, y, z));
					}
				}
			}
		}
	}
}

void World::Update()
{
	int player_chunk_x = (int)floor(playerPosition.x / CHUNK_SIZE_X);
	int player_chunk_z = (int)floor(playerPosition.z / CHUNK_SIZE_Z);

	int build_distance = render_distance + 4;

	

	for (int i = player_chunk_x - build_distance; i < player_chunk_x + build_distance; i++)
	{
		for (int j = player_chunk_z - build_distance; j < player_chunk_z + build_distance; j++)
		{
			if (!ChunkExistsInMap(i, j))
			{
				Chunk* chunk = EmplaceChunkInMap(i, j);
				for (int x = 0; x < CHUNK_SIZE_X; x++)
				{
					for (int y = 0; y < CHUNK_SIZE_Y; y++)
					{
						for (int z = 0; z < CHUNK_SIZE_Z; z++)
						{
							if (y < 128)
								chunk->SetBlock(BlockType::OAK_PLANKS, glm::vec3(x, y, z));
							else
								chunk->SetBlock(BlockType::AIR, glm::vec3(x, y, z));
						}
					}
				}
			}
			else
			{
				Chunk* chunk = EmplaceChunkInMap(i, j);
				loadedChunks.insert(chunk);
			}
		}
	}

	std::vector<Chunk*> toRemove;
	
	for (auto chunk : loadedChunks)
	{
		
		if (!chunk) return;
		if (chunk->pPosition.x < player_chunk_x - build_distance || chunk->pPosition.z < player_chunk_z - build_distance || chunk->pPosition.x > player_chunk_x + build_distance || chunk->pPosition.z > player_chunk_z + build_distance)
		{
			//delete chunk;
			
			//loadedChunks.erase(chunk);
			toRemove.push_back(chunk);
		}
	}

	for (int i = 0; i < toRemove.size(); i++)
	{
		RemoveChunkInMap(static_cast<int>(toRemove[i]->pPosition.x), static_cast<int>(toRemove[i]->pPosition.z));
		loadedChunks.erase(toRemove[i]);
	}
	
}

void World::UpdatePlayerPosition(glm::vec3 playerPos)
{
	playerPosition = playerPos;
}

ChunkDataTypePtr World::GetChunkDataForMeshing(int cx, int cz)
{
	if (ChunkExistsInMap(cx, cz))
	{
		return &RetrieveChunkFromMap(cx, cz)->pChunkContents;
	}

	return nullptr;
}

void World::RenderWorld(FPSCamera* camera)
{
	int player_chunk_x = 0;
	int player_chunk_z = 0;

	//Render Skybox

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);

	player_chunk_x = (int)floor(camera->GetPosition().x / CHUNK_SIZE_X);
	player_chunk_z = (int)floor(camera->GetPosition().z / CHUNK_SIZE_Z);

	//std::cout << "player chunk x - " << player_chunk_x << "and z - " << player_chunk_z << std::endl;

	renderer.StartChunkRendering(camera, render_distance);

	//std::cout << "X range of - " << player_chunk_x - render_distance << " to " << player_chunk_x + render_distance << std::endl;

	for (int i = player_chunk_x - render_distance; i < player_chunk_x + render_distance; i++)
	{
		for (int j = player_chunk_z - render_distance; j < player_chunk_z + render_distance; j++)
		{
			Chunk* chunk = RetrieveChunkFromMap(i, j);
			//if (BoxInFrustum())

			if (chunk->pMeshState == ChunkMeshState::Unbuilt)
				chunk->Construct(GetChunkDataForMeshing(i, j + 1), GetChunkDataForMeshing(i, j - 1), GetChunkDataForMeshing(i - 1, j), GetChunkDataForMeshing(i + 1, j));

			if (chunk->pMeshState == ChunkMeshState::Built)
				renderer.RenderChunk(chunk);

			
		}
	}
	renderer.EndChunkRendering();	
}

void World::RenderSingleChunk(int x, int y, FPSCamera* camera)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glFrontFace(GL_CCW);
	renderer.StartChunkRendering(camera, render_distance);

	Chunk* chunk = RetrieveChunkFromMap(x,y);
	for (int x = 0; x < CHUNK_SIZE_X; x++)
	{
		for (int y = 0; y < CHUNK_SIZE_Y; y++)
		{
			for (int z = 0; z < CHUNK_SIZE_Z; z++)
			{
				chunk->SetBlock((y % 2 == 0) ? BlockType::DIRT : BlockType::OAK_PLANKS, glm::vec3(x, y, z));
			}
		}
	}

	if (chunk->pMeshState == ChunkMeshState::Unbuilt)
		chunk->Construct(GetChunkDataForMeshing(x, y+1), GetChunkDataForMeshing(x, y-1), GetChunkDataForMeshing(x+ 1, y), GetChunkDataForMeshing(x-1, y));
	if (chunk->pMeshState == ChunkMeshState::Built)
		renderer.RenderChunk(chunk);

	renderer.EndChunkRendering();
}

std::pair<Block*, Chunk*> World::GetBlockFromPosition(const glm::vec3& pos) noexcept
{
	return std::pair<Block*, Chunk*>();
}

BlockType World::GetBlockTypeFromPosition(const glm::vec3& pos) noexcept
{
	return BlockType();
}

Chunk* World::RetrieveChunkFromMap(int cx, int cz) noexcept
{
	auto chunk = m_WorldChunks.find(std::pair<int, int>(cx, cz));

	if (chunk == m_WorldChunks.end())
	{
		std::cout << "Failed to find chunk in world data" << std::endl;
		return nullptr;
	}

	Chunk* ret_val = &m_WorldChunks.at(std::pair<int, int>(cx, cz));
	return ret_val;
}

bool World::ChunkExistsInMap(int cx, int cz)
{
	std::map<std::pair<int, int>, Chunk>::iterator chunk_exists = m_WorldChunks.find(std::pair<int, int>(cx, cz));

	if (chunk_exists == m_WorldChunks.end())
	{
		return false;
	}

	return true;
}

Chunk* World::EmplaceChunkInMap(int cx, int cz)
{
	std::map<std::pair<int, int>, Chunk>::iterator chunk_exists = m_WorldChunks.find(std::pair<int, int>(cx, cz));

	if (chunk_exists == m_WorldChunks.end())
	{
		//Timer timer(str.str());

		m_WorldChunks.emplace(std::pair<int, int>(cx, cz), glm::vec3(cx, 0, cz));
		//m_ChunkCount++;
	}

	return &m_WorldChunks.at(std::pair<int, int>(cx, cz));
}

void World::RemoveChunkInMap(int cx, int cz)
{
	std::map<std::pair<int, int>, Chunk>::iterator chunk_exists = m_WorldChunks.find(std::pair<int, int>(cx, cz));
	if (chunk_exists == m_WorldChunks.end())
	{
		return;
	}
	std::cout << "removing chunk at x - " << cx << " and z - " << cz << std::endl;
	m_WorldChunks.erase(std::pair<int, int>(cx, cz));
}
