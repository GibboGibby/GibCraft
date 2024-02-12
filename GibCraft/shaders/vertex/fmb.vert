#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

uniform float uFrequency;
uniform float uAmplitude;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}

float noise (in vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);

    return mix(a, b, u.x) +
            (c - a)* u.y * (1.0 - u.x) +
            (d - b) * u.x * u.y;
}

void main()
{
	vec3 newPos = vec3(aPos.x, aPos.y, aPos.z);
	const int octaves = 80;
	float lacunarity = 2.0;
	float gain = 0.5;

	float amplitude = 0.5;
	float frequency = 1.;
	for (int i = 0; i < octaves;i++)
	{
		newPos.y += amplitude * noise(frequency*aTexCoords);
		frequency *= lacunarity;
		amplitude *= gain;
	}
	gl_Position = uProjection * uView * (uTransform * vec4(newPos, 1.0));
	texCoords = aTexCoords;
}