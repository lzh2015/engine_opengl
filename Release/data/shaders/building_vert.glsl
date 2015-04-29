#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform int bInverse;
uniform mat4 inverse;
uniform mat4 transform;
uniform mat4 rotate;
uniform mat4 vp;
uniform mat4 camView;
uniform vec3 lightDir;
uniform vec4 lightColor;
uniform mat4 depthBiasMVP_0;
uniform mat4 depthBiasMVP_1;


out float castShadow;
smooth out vec2 pTexCoord;
smooth out vec4 pDiffuseColor;
smooth out vec4 camViewCoord;
smooth out vec4 shadowMapCoord_0;
smooth out vec4 shadowMapCoord_1;

void main()
{
	mat4 mvp;

	if(bInverse == 1)
	{
		mvp = vp * inverse * transform;
	}
	else
	{
		mvp = vp * transform;
	}
	
	gl_Position = mvp*vec4(inPosition, 1.0);
	
	pTexCoord = inTexCoord;
	pTexCoord.y = 0.5 - (pTexCoord.y  - 0.5);
	
	camViewCoord = camView * transform * vec4(inPosition, 1.0);
	shadowMapCoord_0 = depthBiasMVP_0 * transform * vec4(inPosition, 1.0);
	shadowMapCoord_1 = depthBiasMVP_1 * transform * vec4(inPosition, 1.0);
	
	vec4 translated_normal = rotate * vec4(inNormal, 1.0);
	
	float diffuseIntensity = dot(normalize(translated_normal.xyz), lightDir);
	
	if (diffuseIntensity > 0) 
	{
		pDiffuseColor = lightColor * diffuseIntensity;
		pDiffuseColor.a = 1.0;
		castShadow = 1.0;
	}
	else
	{
		castShadow = 0.0;
		pDiffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}