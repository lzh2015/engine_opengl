#version 330

layout (location = 0) in vec3 inPosition;

uniform mat4 mvp;
uniform mat4 translate;

uniform vec3 bbox_min;
uniform vec3 bbox_max;

smooth out vec2 uv;
smooth out vec3 pVertex;

void main(void)
{
	vec3 vPositionNormalized = (inPosition.xyz - bbox_min.xyz) / (bbox_max.xyz - bbox_min.xyz);
	uv = vPositionNormalized.xz;
	pVertex = (translate * vec4(inPosition, 1.0)).xyz;
	gl_Position = mvp*vec4(inPosition, 1.0);
}
