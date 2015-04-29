#version 330

in float castShadow;
smooth in vec2 pTexCoord;
smooth in vec4 pDiffuseColor;
smooth in vec4 camViewCoord;
smooth in vec4 shadowMapCoord_0;
smooth in vec4 shadowMapCoord_1;

uniform sampler2D gTexture;
uniform sampler2D gDepthMap_0;
uniform sampler2D gDepthMap_1;
uniform bool volumetric_light;
uniform float csm_frumstum_0;
uniform float csm_frumstum_1;

out vec4 outputColor;

float SampleBasciShadowMap()
{
    float shadow = 1.0;
    float bias = 0.0005;
    
    if((camViewCoord.z/camViewCoord.w < 0) && (camViewCoord.z/camViewCoord.w >= -csm_frumstum_0))
    {
   		if(shadowMapCoord_0.x >= 0.0 && shadowMapCoord_0.y >= 0.0 && shadowMapCoord_0.x <= 1.0 && shadowMapCoord_0.y <= 1.0)
    	{
       		vec4 vDepthMapColor = texture2D(gDepthMap_0, shadowMapCoord_0.xy);
        	if((shadowMapCoord_0.z - bias)> vDepthMapColor.x)
        	{
            	shadow = clamp((shadowMapCoord_0.z - vDepthMapColor.x)*10.0, 0.0, 1.0);	
        	}
   	 	}
    }
    else if((camViewCoord.z/camViewCoord.w < -csm_frumstum_0) && (camViewCoord.z/camViewCoord.w >= -csm_frumstum_1))
    {
    	if(shadowMapCoord_1.x >= 0.0 && shadowMapCoord_1.y >= 0.0 && shadowMapCoord_1.x <= 1.0 && shadowMapCoord_1.y <= 1.0)
    	{
       		vec4 vDepthMapColor = texture2D(gDepthMap_1, shadowMapCoord_1.xy);
        	if((shadowMapCoord_1.z - bias) > vDepthMapColor.x)
        	{
            	shadow = clamp((shadowMapCoord_1.z - vDepthMapColor.x)*10.0, 0.0, 1.0);
        	}
   	 	}
   	 }
    
    return shadow;
}

void main()
{
	vec4 cBase =  texture2D(gTexture, pTexCoord);
	if(cBase.a < 1.0) discard;
	
	if(volumetric_light == true)
	{
		outputColor = vec4(0,0,0,1);
		return;
	}
	
	if(castShadow > 0.5)
	{
		float visibility = SampleBasciShadowMap();
		outputColor =  cBase * 0.5 + cBase * pDiffuseColor * visibility;
	}
	else 
	{
		outputColor =  cBase * 0.5 + cBase * pDiffuseColor;
	}
	
	
	//outputColor = vec4(1,0,1,1);
}