#pragma once
#include "Chunk.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "Chunk.h"
#include "FPSCamera.h"
#include "Texture.h"

class Renderer
{
public:
	Renderer();

	void StartChunkRendering(std::shared_ptr<FPSCamera> camera, int render_distacne);
	void RenderTransparentChunk(std::shared_ptr<Chunk> chunk);
	void RenderChunk(std::shared_ptr<Chunk> chunk);
	void EndChunkRendering();

	Texture* GetAtlasTexture() { return &mBlockAtlas; }

private:
	VertexBuffer mVBO;
	VertexArray mVAO;
	Shader mDefaultChunkShader;
	Texture mBlockAtlas;
};