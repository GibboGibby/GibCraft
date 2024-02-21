#ifndef WORLD_CLASS_H
#define WORLD_CLASS_H


#include <glm/ext.hpp>
#include <string>
#include <map>
#include "Renderer.h"
#include <unordered_set>
#include <thread>
#include "WorldGen.h"
#include <mutex>

/*
bool TestAABB3DCollision(const glm::vec3& pos_1, const glm::vec3 dim_1, const glm::vec3& pos_2, const glm::vec3& dim_2)
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

*/
	class World
	{
	public:
		World(int seed, const glm::vec2& window_size, const std::string& world_name);
		World();

		void UpdatePlayerPosition(glm::vec3 playerPos);

		void RenderWorld(FPSCamera* camera);
		void RenderWorldThreaded(FPSCamera* camera);
		void RenderSingleChunk(int x, int y, FPSCamera* camera);

		std::pair<Block*, Chunk*> GetBlockFromPosition(const glm::vec3& pos) noexcept;
		BlockType GetBlockTypeFromPosition(const glm::vec3& pos) noexcept;
		Chunk* RetrieveChunkFromMap(int cx, int cz) noexcept;

		bool ChunkExistsInMap(int cx, int cz);

		Chunk* EmplaceChunkInMap(int cx, int cz);

		void RemoveChunkInMap(int cx, int cz);

		ChunkDataTypePtr GetChunkDataForMeshing(int cx, int cz);

		void Init();


		void Update();

		void UpdateFramePause();

		void Raycast(bool place, FPSCamera* camera);
		void CreateWorldGenThread(FPSCamera* camera);
	private:

		std::mutex mutex;
		void GenerateAndBuildMesh(FPSCamera* camera);

		void ThreadedWorldGenAndMeshing(FPSCamera* camera);
		

		std::unordered_set<Chunk*> toGenerate;
		std::unordered_set<Chunk*> toMesh;
		std::unordered_set<Chunk*> toRender;

		std::map<std::pair<int, int>, Chunk> m_WorldChunks;
		long long m_CurrentFrame;
		const int m_WorldSeed;
		glm::vec3 playerPosition;

		bool shouldThread = true;

		Renderer renderer;
		bool TestRayPlayerCollision(const glm::vec3& ray_block, glm::vec3 pos);
		std::unordered_set<Chunk*> loadedChunks;

		int render_distance = 12;
		bool thread_started = false;

		bool framePause = false;
	}; 

#endif // !WORLD_CLASS_H