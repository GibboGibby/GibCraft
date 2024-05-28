#version 330 core
in vec2 v_TexCoord;

in vec3 v_WorldPosition;
in vec4 v_TintColor;
in float v_SunlightIntensity;
out vec4 o_Color;
in float v_AmbientOcclusion;

uniform sampler2D u_Texture;



void main()
{

	vec3 SkyTint = vec3(151.0f, 183.0f, 245.0f) / 255.0f;
	const float threshhold = 0.1;
	o_Color = texelFetch(u_Texture, ivec2(v_TexCoord.xy), 0);


	//o_Color.xyz = pow(o_Color.xyz, vec3(2.2));
	//o_Color *= v_TintColor;

	//o_Color = o_Color * vec4(v_SunlightIntensity, v_SunlightIntensity, v_SunlightIntensity, 1.0f) ; 
	//o_Color.xyz = mix(o_Color.xyz * 1.1f, o_Color.xyz * 1.1 * SkyTint.xyz, 0.65f);
	//o_Color = texelFetch(u_Texture, ivec2(v_TexCoord.xy), 0);
}