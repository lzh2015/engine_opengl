#version 330

struct DirectionalLight
{
   vec4 vColor;
   vec3 vDirection;
}; 

smooth in vec2 vTexCoord;
smooth in vec3 pNormal;
smooth in vec3 vLightDirTanSpace;
in vec3 tangent;
in vec3 bitangent;

uniform sampler2D gNormalMap;
uniform sampler2D gDiffuse0;
uniform sampler2D gDiffuse1;
uniform sampler2D gDiffuse2;
uniform sampler2D gDiffuseMap;

uniform int bEnableBumpMap;
uniform DirectionalLight sunLight;
uniform vec3 cameraDirection;

uniform vec4 color;

out vec4 outputColor;

void main()
{
	float ambient = 0.5;
	
	vec2 uv_diffuse = vTexCoord * 70;
	vec2 uv_detail = vTexCoord * 20;
	
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
	//}
	
	if(bEnableBumpMap == 0)
	{
		outputColor = cBase * ambient;
	}
	else 
	{
		//diffuse
		vec3 normalTBN = texture2D(gNormalMap, uv_detail).rgb*2.0 - 1.0;
		normalTBN = normalize(normalTBN);
		normalTBN = vec3(0.0, 1.0, 0.0);
		
		float iDiffuse = max(dot(-vLightDirTanSpace, normalTBN), 0.0);	
		//float iDiffuse = max(dot(sunLight.vDirection, pNormal), 0.0);	
		
		vec4 diffuse = sunLight.vColor * iDiffuse;
		
		outputColor = cBase * ambient + diffuse * cBase;
		
		//specular
		//vec3 reflection = normalize(reflect(sunLight.vDirection, pNormal));
			
		//float specularIntensity = dot(reflection, cameraDirection);
		
		//if(specularIntensity < 0)
		//{
		//	specular = vec3(0.0, 0.0, 0.0);
			
		//}
		
		//outputColor = vec4(ambient + diffuse + specular, 1.0);
	}
}