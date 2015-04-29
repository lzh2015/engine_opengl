#version 330

//uniform mat4 projectionMatrix;
//uniform mat4 modelViewMatrix;
uniform mat4 MVP;
uniform mat4 DepthBiasMVP;

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;

smooth out vec3 pNormal;
smooth out vec4 shadowMapCoord;

void main()
{
	//gl_Position = projectionMatrix*modelViewMatrix*vec4(inPosition, 1.0);
	gl_Position = MVP*vec4(inPosition, 1.0);
	shadowMapCoord = DepthBiasMVP * vec4(inPosition, 1.0);
	pNormal = inNormal;
}