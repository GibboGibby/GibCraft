#version 330 core
in vec2 v_TexCoord;

in vec3 v_WorldPosition;

out vec4 o_Color;

uniform sampler2D u_Texture;


void main()
{
	o_Color =  texelFetch(u_Texture, ivec2(v_TexCoord.xy), 0);
	//o_Color = mix(vec4(pow(u_FogColor.xyz, vec3(1.75)), u_FogColor.w), o_Color, v_Visibility);
	//o_Color = vec4(RomBinDaHouseToneMapping(o_Color.xyz), o_Color.w);
}