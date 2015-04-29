#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 mvp;
uniform float time;
uniform vec3 lightDir;
uniform float lod_metric;
uniform mat4 camView;
uniform mat4 depthBiasMVP_0;
uniform mat4 depthBiasMVP_1;


smooth out vec2 pTexCoord;
smooth out vec4 pDiffuseColor;
smooth out vec4 camViewCoord;
smooth out vec4 shadowMapCoord_0;
smooth out vec4 shadowMapCoord_1;

void main()
{
	vec3 vertex = inPosition;
	vec3 normal = inNormal;
	
	camViewCoord = camView * vec4(inPosition, 1.0);
	shadowMapCoord_0 = depthBiasMVP_0 * vec4(inPosition, 1.0);
	shadowMapCoord_1 = depthBiasMVP_1 * vec4(inPosition, 1.0);
	
	if(normal.y < 0.0) {
		normal = -normal;
		vertex.x += 0.5*cos(time) * cos(vertex.x) * sin(vertex.x);
		vertex.z += 0.5*sin(time) * cos(vertex.x) * sin(vertex.x);
	}
	
	float intensity = dot(lightDir, normal);
	
	pDiffuseColor = vec4(intensity, intensity, intensity, 1.0);
	pDiffuseColor.a = 1.0 - clamp(length(inPosition)/lod_metric, 0.0, 1.0);
	
	gl_Position = mvp*vec4(vertex, 1.0);
	pTexCoord = inTexCoord;
	
}