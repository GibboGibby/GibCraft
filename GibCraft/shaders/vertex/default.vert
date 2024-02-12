#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;


out vec2 texCoords;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

uniform float uFrequency;
uniform float uAmplitude;

void main()
{
	//gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	float amp = 0.1;
	float freq = 5.0;
	float y = uAmplitude * sin(aTexCoords.x * uFrequency);
	vec3 newPos = vec3(aPos.x, y, aPos.z);
	gl_Position = uProjection * uView * (uTransform * vec4(aPos, 1.0));
	texCoords = aTexCoords;
}