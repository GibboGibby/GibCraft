#include "World.h"


bool TestAABB3DCollision(const glm::vec3& pos_1, const glm::vec3& dim_1, const glm::vec3& pos_2, const glm::vec3& dim_2)
{
	if (pos_1.x < pos_2.x + dim_2.x &&
		pos_1.x + dim_1.x > pos_2.x &&
		pos_1.y < pos_2.y + dim_2.y &&
		pos_1.y + dim_1.y > pos_2.y &&
		pos_1.z < pos_2.z + dim_2.z &&
		pos_1.z + dim_1.z > pos_2.z)
	{
		return true;
	}

	return false;
}

World::World(int seed, const glm::vec2& window_size, const std::string& world_name) : m_WorldSeed(seed)
{
	//Init();
}

World::World() : m_WorldSeed(5213)
{
	//Init();
	
}

void World::CreateWorldGenThread(FPSCamera* camera)
{
	if (thread_started) return;
	std::thread thr = std::thread(&World::ThreadedWorldGenAndMeshing, this, std::ref(camera));
	thr.detach();
	thread_started = true;
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
				WorldGen::GenerateChunk(chunk, m_WorldSeed);
				//chunk->pMeshState = ChunkMeshState::NeedsBuilding;
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

void World::UpdateFramePause()
{
	framePause = false;
}

static glm::ivec3 WorldBlockToLocalBlockCoordinates(const glm::vec3& pos)
{
	int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
	int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
	int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
	int by = static_cast<int>(floor(pos.y));
	int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

	return glm::ivec3(bx, by, bz);
}

bool World::TestRayPlayerCollision(const glm::vec3& ray_block, glm::vec3 pos)
{

	if (TestAABB3DCollision(pos, glm::vec3(0.75f, 1.5f, 0.75f), ray_block, glm::vec3(1.0f, 1.0f, 1.0f)))
	{
		return true;
	}

	return false;
}

void World::Raycast(bool place, FPSCamera* camera)
{
	glm::vec3 position = camera->GetPosition();
	const glm::vec3& direction = camera->GetFront();
	int max = 50; // block reach

	glm::vec3 sign;

	for (int i = 0; i < 3; ++i)
		sign[i] = direction[i] > 0;

	for (int i = 0; i < max; ++i)
	{
		glm::vec3 tvec = (floor(position + sign) - position) / direction;
		float t = std::min(tvec.x, std::min(tvec.y, tvec.z));

		position += direction * (t + 0.001f);

		if (position.y >= 0 && position.y < CHUNK_SIZE_Y)
		{
			std::pair<Block*, Chunk*> ray_hitblock = GetBlockFromPosition(glm::vec3(
				floor(position.x),
				floor(position.y),
				floor(position.z)
			));

			Block* ray_block = ray_hitblock.first;


			if (ray_block != nullptr && ray_block->type != BlockType::AIR && ray_block->IsLiquid() == false)
			{
				glm::vec3 normal;

				for (int i = 0; i < 3; ++i)
				{
					normal[i] = (t == tvec[i]);

					if (sign[i])
					{
						normal[i] = -normal[i];
					}
				}

				if (place)
				{
					position = position + normal;
				}

				std::pair<Block*, Chunk*> edit_block;

				if (position.y >= 0 && position.y < CHUNK_SIZE_Y)
				{
					edit_block = GetBlockFromPosition(glm::vec3(position.x, position.y, position.z));
					glm::ivec3 local_block_pos = WorldBlockToLocalBlockCoordinates(position);
					glm::vec2 chunk_pos = glm::vec2(edit_block.second->pPosition.x, edit_block.second->pPosition.z);

					Chunk* front_chunk = RetrieveChunkFromMap(chunk_pos.x, chunk_pos.y + 1);
					Chunk* back_chunk = RetrieveChunkFromMap(chunk_pos.x, chunk_pos.y - 1);
					Chunk* right_chunk = RetrieveChunkFromMap(chunk_pos.x + 1, chunk_pos.y);
					Chunk* left_chunk = RetrieveChunkFromMap(chunk_pos.x - 1, chunk_pos.y);

					BlockType snd_type;

					if (place && !TestRayPlayerCollision(position, camera->GetPosition()))
					{
						edit_block.first->type = BlockType::DIRT;
						snd_type = edit_block.first->type;
					}
					else
					{
						snd_type = edit_block.first->type;

						//Check if Bedrock

						if (local_block_pos.y >= 0 && local_block_pos.y < CHUNK_SIZE_Y - 1)
						{
							if (edit_block.second->pChunkContents.at(local_block_pos.x).at(local_block_pos.y + 1).at(local_block_pos.z).DependsOnBelowBlock())
							{
								edit_block.second->pChunkContents.at(local_block_pos.x).at(local_block_pos.y + 1).at(local_block_pos.z).type = BlockType::AIR;
							}
						}

						edit_block.first->type = BlockType::AIR;
					}

					edit_block.second->pMeshState = ChunkMeshState::NeedsBuilding;

					if (local_block_pos.x <= 0)
					{
						Chunk* update_chunk = RetrieveChunkFromMap(edit_block.second->pPosition.x - 1, edit_block.second->pPosition.z);
						update_chunk->pMeshState = ChunkMeshState::NeedsBuilding;
					}

					if (local_block_pos.z <= 0)
					{
						Chunk* update_chunk = RetrieveChunkFromMap(edit_block.second->pPosition.x, edit_block.second->pPosition.z - 1);
						update_chunk->pMeshState = ChunkMeshState::NeedsBuilding;
					}

					if (local_block_pos.x >= CHUNK_SIZE_X - 1)
					{
						Chunk* update_chunk = RetrieveChunkFromMap(edit_block.second->pPosition.x + 1, edit_block.second->pPosition.z);
						update_chunk->pMeshState = ChunkMeshState::NeedsBuilding;
					}

					if (local_block_pos.z >= CHUNK_SIZE_Z - 1)
					{
						Chunk* update_chunk = RetrieveChunkFromMap(edit_block.second->pPosition.x, edit_block.second->pPosition.z + 1);
						update_chunk->pMeshState = ChunkMeshState::NeedsBuilding;
					}

					edit_block.second->pChunkState = ChunkState::Changed;


				}
				return;
			}
		}
	}
}

void World::GenerateAndBuildMesh(FPSCamera* camera)
{
	int player_chunk_x = 0;
	int player_chunk_z = 0;

	player_chunk_x = (int)floor(camera->GetPosition().x / CHUNK_SIZE_X);
	player_chunk_z = (int)floor(camera->GetPosition().z / CHUNK_SIZE_Z);

	for (int i = player_chunk_x - render_distance; i < player_chunk_x + render_distance; i++)
	{
		for (int j = player_chunk_z - render_distance; j < player_chunk_z + render_distance; j++)
		{
			Chunk* chunk = RetrieveChunkFromMap(i, j);
			//if (BoxInFrustum())

			if (chunk->pMeshState == ChunkMeshState::Unbuilt)
				chunk->Construct(GetChunkDataForMeshing(i, j + 1), GetChunkDataForMeshing(i, j - 1), GetChunkDataForMeshing(i - 1, j), GetChunkDataForMeshing(i + 1, j));
		}
	}
}

void World::ThreadedWorldGenAndMeshing(FPSCamera* camera)
{
	while (shouldThread)
	{
		glm::vec3 tempPlayerPos = this->playerPosition;
		std::cout << "This is the player x - " << tempPlayerPos.x << " and the player z - " << tempPlayerPos.z << std::endl;

		int player_chunk_x = 0;
		int player_chunk_z = 0;

		int build_distance = render_distance + 4;

		player_chunk_x = (int)floor(tempPlayerPos.x / CHUNK_SIZE_X);
		player_chunk_z = (int)floor(tempPlayerPos.z / CHUNK_SIZE_Z);


		for (int i = player_chunk_x - build_distance; i < player_chunk_x + build_distance; i++)
		{
			for (int j = player_chunk_z - build_distance; j < player_chunk_z + build_distance; j++)
			{
				if (!ChunkExistsInMap(i, j))
				{
					//mutex.lock();
					Chunk* chunk = this->EmplaceChunkInMap(i, j);
					WorldGen::GenerateChunk(chunk, m_WorldSeed);
					//mutex.unlock();
				}
				else
				{
					//mutex.lock();
					Chunk* chunk = this->EmplaceChunkInMap(i, j);
					if (chunk == nullptr) continue;
					//this->loadedChunks.insert(chunk);
					//mutex.unlock();
					if (i > player_chunk_x - render_distance && i < player_chunk_x + render_distance)
					{
						if (chunk->pMeshState == ChunkMeshState::Unbuilt)
							chunk->ConstructNoBind(GetChunkDataForMeshing(i, j + 1), GetChunkDataForMeshing(i, j - 1), GetChunkDataForMeshing(i + 1, j), GetChunkDataForMeshing(i - 1, j));

					}
				}
			}
		}

		std::vector<Chunk*> toRemove;
		if (loadedChunks.size() > 0)
		{
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
		}


		for (int i = 0; i < toRemove.size(); i++)
		{
			RemoveChunkInMap(static_cast<int>(toRemove[i]->pPosition.x), static_cast<int>(toRemove[i]->pPosition.z));
			loadedChunks.erase(toRemove[i]);
		}
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
			//mutex.lock();
			Chunk* chunk = RetrieveChunkFromMap(i, j);
			//if (BoxInFrustum())

			if (chunk->pMeshState == ChunkMeshState::Unbuilt)
			{
				chunk->Construct(GetChunkDataForMeshing(i, j + 1), GetChunkDataForMeshing(i, j - 1), GetChunkDataForMeshing(i + 1, j), GetChunkDataForMeshing(i - 1, j));
				//toMesh.insert(chunk);
				//chunk->pMeshState = ChunkMeshState::Building;
			}

			if (chunk->pMeshState == ChunkMeshState::Built)
				renderer.RenderChunk(chunk);
		}
	}
	renderer.EndChunkRendering();	
}

void World::RenderWorldThreaded(FPSCamera* camera)
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
			if (chunk == nullptr) continue;
			//if (BoxInFrustum())

			if (chunk->pMeshState == ChunkMeshState::NeedsBuilding)
			{
				chunk->Construct(GetChunkDataForMeshing(i, j + 1), GetChunkDataForMeshing(i, j - 1), GetChunkDataForMeshing(i + 1, j), GetChunkDataForMeshing(i - 1, j));

			}

			if (chunk->pMeshState == ChunkMeshState::NeedsBinding)
			{
				chunk->BindChunkMesh();
				//toMesh.insert(chunk);
				//chunk->pMeshState = ChunkMeshState::Building;
			}

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
	int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
	int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
	int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
	int by = static_cast<int>(floor(pos.y));
	int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

	Chunk* chunk = RetrieveChunkFromMap(block_chunk_x, block_chunk_z);

	return { &chunk->pChunkContents.at(bx).at(by).at(bz), chunk };
}

BlockType World::GetBlockTypeFromPosition(const glm::vec3& pos) noexcept
{
	int block_chunk_x = static_cast<int>(floor(pos.x / CHUNK_SIZE_X));
	int block_chunk_z = static_cast<int>(floor(pos.z / CHUNK_SIZE_Z));
	int bx = pos.x - (block_chunk_x * CHUNK_SIZE_X);
	int by = static_cast<int>(floor(pos.y));
	int bz = pos.z - (block_chunk_z * CHUNK_SIZE_Z);

	return static_cast<BlockType>(RetrieveChunkFromMap(block_chunk_x, block_chunk_z)->pChunkContents.at(bx).at(by).at(bz).type);
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
	//std::cout << "removing chunk at x - " << cx << " and z - " << cz << std::endl;
	m_WorldChunks.erase(std::pair<int, int>(cx, cz));
}
