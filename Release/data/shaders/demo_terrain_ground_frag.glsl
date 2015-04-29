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

uniform sampler2D normalMap;
uniform sampler2D mixmap_1;
uniform sampler2D mixmap_2;
uniform sampler2D snow;
uniform sampler2D dry_grass;
uniform sampler2D grass;
uniform sampler2D cliff;
uniform sampler2D cliff_steep;
uniform sampler2D sand;
uniform sampler2D texWaterCaustics;
uniform sampler2D gDepthMap_0;
uniform sampler2D gDepthMap_1;


uniform int bEnableBumpMap;
uniform bool bWaterReflection;
uniform DirectionalLight sunLight;
uniform vec4 color;
uniform float water_height;
uniform vec3 bbox_min;
uniform vec3 bbox_max;
uniform float time;
uniform bool volumetric_light;
uniform float csm_frumstum_0;
uniform float csm_frumstum_1;

out vec4 outputColor;

float CausticsAlpha()
{
	//return (water_height - vPosition.y) / (2*(water_height - bbox_min.y));
	return 0.5;
}

vec4 CausticsColor()
{
	vec2 uv0 = vTexCoord*100.0;
	uv0.s -= time*0.1;
	uv0.t += time*0.1;
	vec4 color0 = texture2D(texWaterCaustics, uv0);
	
	vec2 uv1 = vTexCoord*100.0;
	uv1.s += time*0.1;
	uv1.t += time*0.1;	
	vec4 color1 = texture2D(texWaterCaustics, uv1);
	
	return (color0 + color1) /2;	
}

float SampleBasciShadowMap()
{
    float shadow = 1.0;
    
    if((camViewCoord.z/camViewCoord.w < 0) && (camViewCoord.z/camViewCoord.w >= -csm_frumstum_0))
    {
   		if(shadowMapCoord_0.x >= 0.0 && shadowMapCoord_0.y >= 0.0 && shadowMapCoord_0.x <= 1.0 && shadowMapCoord_0.y <= 1.0)
    	{
       		vec4 vDepthMapColor = texture2D(gDepthMap_0, shadowMapCoord_0.xy);
        	if(shadowMapCoord_0.z > vDepthMapColor.x)
        	{
            	//shadow = clamp((shadowMapCoord_0.z - vDepthMapColor.x)*10.0, 0.0, 1.0);
            	shadow = 0.5;
        	}
   	 	}
    }
    else if((camViewCoord.z/camViewCoord.w < -csm_frumstum_0) && (camViewCoord.z/camViewCoord.w >= -csm_frumstum_1))
    {
    	if(shadowMapCoord_1.x >= 0.0 && shadowMapCoord_1.y >= 0.0 && shadowMapCoord_1.x <= 1.0 && shadowMapCoord_1.y <= 1.0)
    	{
       		vec4 vDepthMapColor = texture2D(gDepthMap_1, shadowMapCoord_1.xy);
        	if(shadowMapCoord_1.z > vDepthMapColor.x)
        	{
            	//shadow = clamp((shadowMapCoord_1.z - vDepthMapColor.x)*10.0, 0.0, 1.0);
            	shadow = 0.5;
        	}
   	 	}
   	 }
    
    return shadow;
}

void main()
{
	if(volumetric_light == true)
	{
		outputColor = vec4(0,0,0,1);
		return;
	}
	
	if(bWaterReflection)
	{
		if(vPosition.y > water_height+0.15) discard;
	}
	
	float ambient = 0.2;
	
	vec2 uv_diffuse = vTexCoord * 50;
	vec2 uv_detail = vTexCoord * 120;
	
	vec4 tBase[6];
	tBase[0] = texture2D(snow, uv_diffuse);
	tBase[1] = texture2D(dry_grass, uv_diffuse);	
	tBase[2] = texture2D(grass, uv_diffuse);
	tBase[3] = texture2D(cliff, uv_diffuse);
	tBase[4] = texture2D(cliff_steep, uv_diffuse);
	tBase[5] = texture2D(sand, uv_diffuse);
	
	vec4 diffuseMap_1 = texture2D(mixmap_1, vTexCoord);
	vec4 diffuseMap_2 = texture2D(mixmap_2, vTexCoord);
	
	vec4 cBase;
	
	cBase = mix(mix(mix(tBase[5], tBase[2], diffuseMap_1.b), tBase[1], diffuseMap_1.g), tBase[0], diffuseMap_1.r);
	
	cBase = mix(mix(mix(cBase, tBase[5], diffuseMap_2.b), tBase[4], diffuseMap_2.g), tBase[3], diffuseMap_2.r);
	
	float visibility = SampleBasciShadowMap();
	
	if(bEnableBumpMap == 0)
	{
		outputColor = cBase * ambient;
		//outputColor = color;
	}
	else 
	{
		//diffuse
		vec3 normalTBN = texture2D(normalMap, uv_detail).xyz*2.0 - 1.0;
		normalTBN = normalize(normalTBN);
		
		vec3 vLightDirTBN = normalize(vLightDirTanSpace);
		float iDiffuse = max(dot(vLightDirTBN, normalTBN), 0.0);	
		
		vec4 diffuse = sunLight.vColor * iDiffuse * visibility;
			
		vec4 specular = vec4(0,0,0,1);
		//specular
		
		if(vPosition.y < water_height)
		{
			specular = sunLight.vColor * pow(clamp(dot(reflect(vLightDirTBN, normalTBN), vEyeDirTanSpace), 0.0, 1.0), 8)/2.0;
		}
		
		outputColor = cBase * ambient + diffuse * cBase + specular;
		//if(specularIntensity < 0)
		//{
		//	specular = vec3(0.0, 0.0, 0.0);
			
		//}
		
		//outputColor = vec4(ambient + diffuse + specular, 1.0);
	}
	
	if(vPosition.y < water_height)
	{
		float alpha = CausticsAlpha();
		outputColor = (1-alpha) * outputColor + alpha * CausticsColor();
	}
}