#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 color;

uniform mat4 uTransform;
uniform mat4 uProjection;
uniform mat4 uView;

void main()
{
	//gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	gl_Position = vec4(aPos, 1.0);
	color = aColor;
}