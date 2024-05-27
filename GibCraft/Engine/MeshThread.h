#pragma once
#pragma once

#include <iostream>
#include <map>
#include <set>
#include "World.h"
#include <mutex>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;
class MeshThread
{
public:
	MeshThread(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, std::shared_ptr<FPSCamera> camera, int world_seed, int render_dist = 6, int build_dist = 10);

	void AddToSet(int x, int y);
	void AddToSet(std::pair<int, int> chunk);

	void RemoveFromSet(std::pair<int, int> chunk);

	bool ChunkExistsInMap(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz);

	void SetRenderDistance(int render_dist) { render_distance = render_dist; }
	void SetBuildDistance(int build_dist) { render_distance = build_dist; }


private:
	std::shared_ptr<Chunk> RetrieveChunkFromMap(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz);
	ChunkDataSharedPtr GetChunkDataForMeshing(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz);
	std::shared_ptr<Chunk> EmplaceChunkInMap(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, int cx, int cz);
	void ThreadFunc(std::shared_ptr<std::map<std::pair<int, int>, std::shared_ptr<Chunk>>> worldChunks, std::shared_ptr<FPSCamera> camera);
	std::mutex mutex;
	std::set<std::pair<int, int>> chunksToGen;

	int render_distance;
	int build_distance;
	int worldSeed;

	std::thread threadRef;
};