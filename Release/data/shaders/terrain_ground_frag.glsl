#version 330

struct DirectionalLight
{
   vec4 vColor;
   vec3 vDirection;
}; 

smooth in vec3 vPosition;
smooth in vec2 vTexCoord;
smooth in vec3 pNormal;
smooth in vec4 camViewCoord;
smooth in vec4 shadowMapCoord_0;
smooth in vec4 shadowMapCoord_1;
in vec3 vLightDirTanSpace;
in vec3 vEyeDirTanSpace;


uniform sampler2D gNormalMap;
uniform sampler2D gDiffuse0;
uniform sampler2D gDiffuse1;
uniform sampler2D gDiffuse2;
uniform sampler2D gDiffuseMap;
uniform sampler2D gDepthMap_0;
uniform sampler2D gDepthMap_1;

uniform int bEnableBumpMap;
uniform DirectionalLight sunLight;
uniform bool bWaterReflection;
uniform float water_height;
uniform vec4 color;

out vec4 outputColor;

float SampleBasciShadowMap()
{
    float shadow = 1.0;
    
    if((camViewCoord.z/camViewCoord.w < 0) && (camViewCoord.z/camViewCoord.w >= -20))
    {
   		if(shadowMapCoord_0.x >= 0.0 && shadowMapCoord_0.y >= 0.0 && shadowMapCoord_0.x <= 1.0 && shadowMapCoord_0.y <= 1.0)
    	{
       		vec4 vDepthMapColor = texture2D(gDepthMap_0, shadowMapCoord_0.xy);
        	if(shadowMapCoord_0.z > vDepthMapColor.x)
        	{
            	shadow = clamp((shadowMapCoord_0.z - vDepthMapColor.x)*10.0, 0.0, 1.0);
            	//shadow = 0.5;
        	}
   	 	}
    }
    else if((camViewCoord.z/camViewCoord.w < -20) && (camViewCoord.z/camViewCoord.w >= -100))
    {
    	if(shadowMapCoord_1.x >= 0.0 && shadowMapCoord_1.y >= 0.0 && shadowMapCoord_1.x <= 1.0 && shadowMapCoord_1.y <= 1.0)
    	{
       		vec4 vDepthMapColor = texture2D(gDepthMap_1, shadowMapCoord_1.xy);
        	if(shadowMapCoord_1.z > vDepthMapColor.x)
        	{
            	shadow = clamp((shadowMapCoord_1.z - vDepthMapColor.x)*10.0, 0.0, 1.0);
            	//shadow = 0.5;
        	}
   	 	}
   	 }
    
    return shadow;
} 

void main()
{
	if(bWaterReflection)
	{
		if(vPosition.y > water_height+0.15) discard;
	}
	
	float ambient = 0.1;
	
	vec2 uv_diffuse = vTexCoord * 70;
	vec2 uv_detail = vTexCoord * 50;
	
	vec4 tBase[3];
	tBase[0] = texture2D(gDiffuse0, uv_diffuse);
	tBase[1] = texture2D(gDiffuse1, uv_diffuse);	
	tBase[2] = texture2D(gDiffuse2, uv_diffuse);
	vec4 DiffuseMap = texture2D(gDiffuseMap, vTexCoord);
	
	vec4 cBase;
	//if(vPosition.y < water_height)
	//	cBase = tBase[0];
	//else {
	cBase = mix(mix(tBase[1], tBase[0], DiffuseMap.r), tBase[2], DiffuseMap.g);
//	cBase = texture2D(gNormalMap, uv_detail).rgba;
	//}
	
	float visibility = SampleBasciShadowMap();
	
	if(bEnableBumpMap == 0)
	{
		//outputColor = cBase * ambient;
		outputColor = color;
	}
	else 
	{
		//diffuse
		vec3 normalTBN = texture2D(gNormalMap, uv_detail).xyz*2.0 - 1.0;
		normalTBN = normalize(normalTBN);
		//normalTBN = vec3(0.0, 0.0, 1.0);
		
		vec3 vLightDirTBN = normalize(vLightDirTanSpace);
		float iDiffuse = max(dot(vLightDirTBN, normalTBN), 0.0);	
		//float iDiffuse = max(dot(sunLight.vDirection, pNormal), 0.0);	
		
		vec4 diffuse = sunLight.vColor * iDiffuse * visibility;
			
		vec4 specular = vec4(0,0,0,1);
		//specular
		//specular = sunLight.vColor * pow(clamp(dot(reflect(vLightDirTBN, normalTBN), vEyeDirTanSpace), 0.0, 1.0), 8)/2.0;
		
		
		outputColor = cBase * ambient + diffuse * cBase + specular;
		//if(specularIntensity < 0)
		//{
		//	specular = vec3(0.0, 0.0, 0.0);
			
		//}
		
		//outputColor = vec4(ambient + diffuse + specular, 1.0);
	}
	
}