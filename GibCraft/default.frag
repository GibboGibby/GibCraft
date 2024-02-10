#version 460 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D uTexture;

float circle(in vec2 uv, in float radius)
{
	vec2 dist = uv - vec2(0.5);
	return 1.0 - smoothstep(radius-(radius * 0.1), radius+(radius*0.01), dot(dist, dist)*4.0);
}

void main()
{
	//vec2 st = texCoord.xy/
	vec3 color = vec3(circle(texCoords,0.9));
	//FragColor = vec4(0.0f,1.0f,0.0f,1.0f);

	vec4 texColor = texture(uTexture, texCoords);
	//FragColor = vec4(color,1.0);
	FragColor = texColor;
}