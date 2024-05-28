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
#include "Frustum.h"

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

		static std::mutex worldMutex;

		void UpdatePlayerPosition(glm::vec3 playerPos);

		void RenderWorld(std::shared_ptr<FPSCamera> camera);
		void RenderWorldThreaded(FPSCamera* camera);
		void RenderSingleChunk(int x, int y, FPSCamera* camera);

		std::pair<Block*, std::shared_ptr<Chunk>> GetBlockFromPosition(const glm::vec3& pos) noexcept;
		BlockType GetBlockTypeFromPosition(const glm::vec3& pos) noexcept;
		std::shared_ptr<Chunk> RetrieveChunkFromMap(int cx, int cz) noexcept;

		bool ChunkExistsInMap(int cx, int cz);

		std::shared_ptr<Chunk> EmplaceChunkInMap(int cx, int cz);

		void RemoveChunkInMap(int cx, int cz);

		ChunkDataSharedPtr GetChunkDataForMeshing(int cx, int cz);

		void Init();

		void UpdateAddToChunks();

		void InitWorld();

		void Update();

		void UpdateViewFrustum(std::shared_ptr<FPSCamera> cam);

		void UpdateFramePause();

		void Raycast(bool place, std::shared_ptr<FPSCamera> camera);
		void CreateWorldGenThread(FPSCamera* camera);
		std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> m_WorldChunks = std::make_shared<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>>();
	private:

		std::mutex mutex;
		void GenerateAndBuildMesh(FPSCamera* camera);

		void ThreadedWorldGenAndMeshing(FPSCamera* camera);
		

		std::unordered_set<std::shared_ptr<Chunk>> toGenerate;
		std::unordered_set<std::shared_ptr<Chunk>> toMesh;
		std::unordered_set<std::shared_ptr<Chunk>> toRender;

		long long m_CurrentFrame;
		const int m_WorldSeed;
		glm::vec3 playerPosition;

		bool shouldThread = true;

		Renderer renderer;
		bool TestRayPlayerCollision(const glm::vec3& ray_block, glm::vec3 pos);
		std::unordered_set<std::shared_ptr<Chunk>> loadedChunks;

		int render_distance = 6;
		bool thread_started = false;

		bool framePause = false;

		ViewFrustum m_ViewFrustum;
	}; 

#endif // !WORLD_CLASS_H