#include "Renderer.h"

Renderer::Renderer() : mDefaultChunkShader("shaders/vertex/BlockRenderer.vert", "shaders/fragment/BlockRenderer.frag"), mVBO(GL_ARRAY_BUFFER)
{
	mBlockAtlas.CreateTexture("resources\\textures\\atlas.png");
}

void Renderer::RenderChunk(Chunk* chunk)
{
	if (chunk->GetChunkMesh()->p_VerticesCount > 0)
	{
		mDefaultChunkShader.UploadInt("u_ChunkX", chunk->pPosition.x);
		mDefaultChunkShader.UploadInt("u_ChunkZ", chunk->pPosition.z);

		chunk->GetChunkMesh()->pVAO.Bind();
		(glDrawElements(GL_TRIANGLES, floor(chunk->GetChunkMesh()->p_VerticesCount / 4) * 6, GL_UNSIGNED_INT, 0));
		chunk->GetChunkMesh()->pVAO.Unbind();
	}
}

void Renderer::StartChunkRendering(FPSCamera* camera)
{
	
	mDefaultChunkShader.Activate();
	mBlockAtlas.Bind(0);
	mDefaultChunkShader.UploadInt("u_Texture", 0);
	mDefaultChunkShader.UploadMat4("u_ViewProjection", camera->GetViewProjection());
	mDefaultChunkShader.UploadMat4("u_ViewMatrix", camera->GetViewMatrix());
	mDefaultChunkShader.UploadInt("u_RenderDistance", 5);
	mDefaultChunkShader.UploadInt("u_CHUNK_SIZE_X", CHUNK_SIZE_X);
	mDefaultChunkShader.UploadInt("u_CHUNK_SIZE_Z", CHUNK_SIZE_Y);

}

void Renderer::EndChunkRendering()
{
	glUseProgram(0);
}