#version 330 core
in vec2 v_TexCoord;

in vec3 v_WorldPosition;
in float v_AmbientOcclusion;

out vec4 o_Color;

uniform sampler2D u_Texture;
uniform int u_Transparent;

void main()
{
	const float threshold = 0.1;
	o_Color =  texelFetch(u_Texture, ivec2(v_TexCoord.xy), 0);
	//float val = 0.75f * v_AmbientOcclusion;
	//val = 1 - val;
	//o_Color = o_Color * vec4(val, val, val, 1.0f);

	//o_Color *= vec4(1.0f,1.0f,1.0f,1.0f);

	if (o_Color.a < threshold)
	{
		discard;
	}

	
	if (v_AmbientOcclusion <= 0.1f)
	{
		o_Color *= vec4(0.9f,0.9f,0.9f,1.0f);
	}
	else
	{
		float val = 1.0f - (0.575f * v_AmbientOcclusion);
		if (v_AmbientOcclusion == 1.0f)
		{
			//val = 0.25f;
		}
		o_Color *= vec4(val,val,val,1.0f);
	}

	
	//o_Color = mix(vec4(pow(u_FogColor.xyz, vec3(1.75)), u_FogColor.w), o_Color, v_Visibility);
	//o_Color = vec4(RomBinDaHouseToneMapping(o_Color.xyz), o_Color.w);
}