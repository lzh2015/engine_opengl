#version 330

uniform struct Matrices
{
	mat4 projMatrix;
	mat4 modelMatrix;
	mat4 viewMatrix;                                                                           
	mat4 normalMatrix;
} matrices;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inTangent;
layout (location = 4) in vec3 inBitangent;

smooth out vec3 vNormal;
smooth out vec2 vTexCoord;
smooth out vec3 vWorldPos;

smooth out vec4 vEyeSpacePos;

#include "dirLight.frag"

uniform DirectionalLight sunLight;

uniform int bEnableBumpMap;
out vec3 vLightDirTanSpace;

void main()
{
	mat4 mMV = matrices.viewMatrix*matrices.modelMatrix;
	mat4 mMVP = matrices.projMatrix*matrices.viewMatrix*matrices.modelMatrix;
	
	vTexCoord = inCoord;
	
	vEyeSpacePos = mMV*vec4(inPosition, 1.0);
	gl_Position = mMVP*vec4(inPosition, 1.0);

	vNormal = normalize(mat3(matrices.normalMatrix) * inNormal);
	vWorldPos = (matrices.modelMatrix * vec4(inPosition.xyz, 1.0)).xyz;
	
	if(bEnableBumpMap == 1)
	{
		vec3 vTangent = inTangent;
		vec3 vBitangent = inBitangent;
		
		mat3 mTBN = transpose(mat3(vTangent, vBitangent, vNormal));
		vLightDirTanSpace = normalize(mTBN * sunLight.vDirection);
	}
	else vLightDirTanSpace = vec3(0, 0, 0);
}