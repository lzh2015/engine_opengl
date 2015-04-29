#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec4 vEyeSpacePos;
smooth in vec3 vWorldPos;
out vec4 outputColor;

uniform sampler2D gSampler;
uniform sampler2D gNormalMap;
uniform vec4 vColor;

#include "dirLight.frag"

uniform DirectionalLight sunLight;
uniform vec3 vEyePosition;

uniform Material matActive;

uniform int bEnableBumpMap;
in vec3 vLightDirTanSpace;

void main()
{
   vec4 vTexColor = texture2D(gSampler, vTexCoord);
   vec4 vMixedColor = vTexColor*vColor;
   
   vec3 vNormalized = normalize(vNormal);
   
	if(bEnableBumpMap == 0)
	{
		vec4 vDiffuseColor = GetDirectionalLightColor(sunLight, vNormalized);
		vec4 vSpecularColor = GetSpecularColor(vWorldPos, vEyePosition, matActive, sunLight, vNormalized);
		outputColor = vMixedColor*(vDiffuseColor+vSpecularColor);
	}
	else
	{
		vec3 vNormalExtr = normalize(texture2D(gNormalMap, vTexCoord).rgb*2.0 - 1.0);
		float fDiffuseIntensity = max(0.0, dot(vNormalExtr, -vLightDirTanSpace));
		
		float fMult = clamp(sunLight.fAmbient+fDiffuseIntensity, 0.0, 1.0);
		vec4 vDiffuseColor = vec4(sunLight.vColor*fMult, 1.0);
		vec4 vSpecularColor = GetSpecularColor(vWorldPos, vEyePosition, matActive, sunLight, vNormalized);
		
		outputColor = vMixedColor*(vDiffuseColor + vSpecularColor);
  }
}