#version 330

smooth in vec2 pTexCoord;
smooth in vec3 vPosition;
smooth in vec4 pDiffuseColor;

uniform float water_height;
uniform bool bWaterReflection;
uniform sampler2D gTexture_1;
uniform sampler2D gTexture_2;
uniform sampler2D gTexture_3;
uniform sampler2D gTexture_4;
uniform int samplerID;
uniform bool volumetric_light;

out vec4 outputColor;

void main()
{	
	if(bWaterReflection)
	{
		if(vPosition.y > water_height+0.15) discard;
	}
	
	vec4 cBase;
	
	if(samplerID == 1) cBase = texture2D(gTexture_1, pTexCoord);
	if(samplerID == 2) cBase = texture2D(gTexture_2, pTexCoord);
	if(samplerID == 3) cBase = texture2D(gTexture_3, pTexCoord);
	if(samplerID == 4) cBase = texture2D(gTexture_4, pTexCoord);
	
	if(cBase.a < 0.4) discard;
	
	if(volumetric_light == true)
	{
		outputColor = vec4(0,0,0,1);
		return;
	}
	
	outputColor =  cBase * 0.6 + cBase * pDiffuseColor;
	outputColor.a = 1.0;
}