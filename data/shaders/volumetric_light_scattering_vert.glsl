#version 330

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexcoord;

smooth out vec2 uv;

void main(void)
{
//	if(inPosition.x ==  1.0) uv.x = 1;
//	if(inPosition.x == -1.0) uv.x = 0;
//	if(inPosition.y ==  1.0) uv.y = 1;
//	if(inPosition.y == -1.0) uv.y = 0;
	
	uv = inTexcoord;
	
	gl_Position = vec4(inPosition, 0, 1);
}
