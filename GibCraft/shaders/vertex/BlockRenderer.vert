#version 330 core
layout (location = 0) in ivec3 a_Position;
layout (location = 1) in ivec2 a_TexCoords;

uniform int u_RenderDistance;
uniform mat4 u_ViewProjection;
uniform mat4 u_ViewMatrix;

// To transform the positions
uniform int u_CHUNK_SIZE_X;
uniform int u_CHUNK_SIZE_Z;
uniform int u_ChunkX;
uniform int u_ChunkZ;

out vec2 v_TexCoord;
out vec3 v_WorldPosition;


void main()
{
	vec3 real_pos = vec3(a_Position.x + (u_ChunkX * u_CHUNK_SIZE_X), a_Position.y, a_Position.z + (u_ChunkZ * u_CHUNK_SIZE_Z));
	v_WorldPosition = real_pos;

	gl_Position = u_ViewProjection * vec4(real_pos, 1.0);
	
	v_TexCoord = vec2(a_TexCoords.xy);
}