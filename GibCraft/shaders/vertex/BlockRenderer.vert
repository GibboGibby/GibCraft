#version 330 core
layout (location = 0) in ivec3 a_Position;
layout (location = 1) in ivec2 a_TexCoords;
layout (location = 2) in uint a_LightingLevel;
layout (location = 3) in uint a_BlockFaceLightLevel;
layout (location = 4) in uint a_AmbientOcclusion;

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
out vec4 v_TintColor;
out float v_SunlightIntensity;
out uint v_AmbientOcclusion;



void main()
{
	vec4 sunPos = vec4(0.0f,1500.0f,0.0f,1.0f);
	vec4 u_AmbientLight = vec4(0.4f,0.4f,0.4f,1.0f);
	v_AmbientOcclusion = a_AmbientOcclusion;
	vec3 real_pos = vec3(a_Position.x + (u_ChunkX * u_CHUNK_SIZE_X), a_Position.y, a_Position.z + (u_ChunkZ * u_CHUNK_SIZE_Z));
	v_WorldPosition = real_pos;

	gl_Position = u_ViewProjection * vec4(real_pos, 1.0);


	float lighting_level = float(a_LightingLevel) ;	
	lighting_level /= 24;
	lighting_level *= lighting_level;
	lighting_level *= 2.0f;

	float block_light = a_BlockFaceLightLevel < 16.0f ? float(a_BlockFaceLightLevel) : 10.0f;
	block_light /= 10.0f;

	v_SunlightIntensity = max(sunPos.y / 1000.0f, 0.6f);

	if (lighting_level < 0.2)
	{
		v_TintColor = u_AmbientLight;
		v_TintColor = v_TintColor * vec4(block_light, block_light, block_light, 1.0f);
		//v_TintColor = vec4(1.0f,1.0f,1.0f,1.0f);
	}

	else 
	{
		if (lighting_level < u_AmbientLight.x)
		{
			lighting_level = u_AmbientLight.x;
		}

		v_TintColor = vec4(lighting_level, lighting_level, lighting_level, 1.0f);

		if (v_SunlightIntensity < 1.0f)
		{
			v_SunlightIntensity = 1.0f;
		}
	}

	
	v_TexCoord = vec2(a_TexCoords.xy);
}