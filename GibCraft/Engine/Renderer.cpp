#include "Renderer.h"

Renderer::Renderer() : mDefaultChunkShader("shaders/vertex/OldBlockRenderer.vert", "shaders/fragment/OldBlockRenderer.frag"), mVBO(GL_ARRAY_BUFFER)
{
	mBlockAtlas.CreateTexture("resources\\textures\\atlas.png");
}

void Renderer::RenderChunk(std::shared_ptr<Chunk> chunk)
{
	if (chunk->GetChunkMesh()->p_VerticesCount > 0)
	{
		mDefaultChunkShader.UploadInt("u_ChunkX", chunk->pPosition.x);
		mDefaultChunkShader.UploadInt("u_ChunkZ", chunk->pPosition.z);
		mDefaultChunkShader.UploadInt("u_Transparent", 0);
		mDefaultChunkShader.UploadInt("u_VTransparent", 0);
		chunk->GetChunkMesh()->pVAO.Bind();
		(glDrawElements(GL_TRIANGLES, floor(chunk->GetChunkMesh()->p_VerticesCount / 4) * 6, GL_UNSIGNED_INT, 0));
		chunk->GetChunkMesh()->pVAO.Unbind();
	}
}

void Renderer::StartChunkRendering(std::shared_ptr<FPSCamera> camera, int render_distance)
{
	
	mDefaultChunkShader.Activate();
	mBlockAtlas.Bind(0);
	mDefaultChunkShader.UploadInt("u_Texture", 0);
	mDefaultChunkShader.UploadMat4("u_ViewProjection", camera->GetViewProjection());
	mDefaultChunkShader.UploadMat4("u_ViewMatrix", camera->GetViewMatrix());
	mDefaultChunkShader.UploadInt("u_RenderDistance", render_distance);
	mDefaultChunkShader.UploadInt("u_CHUNK_SIZE_X", CHUNK_SIZE_X);
	mDefaultChunkShader.UploadInt("u_CHUNK_SIZE_Z", CHUNK_SIZE_Z);

}

void Renderer::RenderTransparentChunk(std::shared_ptr<Chunk> chunk)
{
	if (chunk->GetChunkMesh()->p_TransparentVerticesCount > 0)
	{
		mDefaultChunkShader.UploadInt("u_ChunkX", chunk->pPosition.x);
		mDefaultChunkShader.UploadInt("u_ChunkZ", chunk->pPosition.z);
		mDefaultChunkShader.UploadInt("u_Transparent", 1);
		mDefaultChunkShader.UploadInt("u_VTransparent", 1);

		chunk->GetChunkMesh()->pTransparentVAO.Bind();
		(glDrawElements(GL_TRIANGLES, floor(chunk->GetChunkMesh()->p_TransparentVerticesCount / 4) * 6, GL_UNSIGNED_INT, 0));
		chunk->GetChunkMesh()->pTransparentVAO.Unbind();
	}
}

void Renderer::EndChunkRendering()
{
	glUseProgram(0);
}