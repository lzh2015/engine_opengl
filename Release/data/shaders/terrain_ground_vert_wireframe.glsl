#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out vec2 uv;
out vec3 normal;

void main()
{
	gl_Position = vec4(inPosition, 1.0);
	uv = inCoord;
	normal = inNormal;
}