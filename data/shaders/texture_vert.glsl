#version 330

struct Material
{
	float ns;
	float ks;
};

struct DirectionalLight
{
   vec4 vColor;
   vec3 vDirection;
}; 

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform Material mtl;
uniform DirectionalLight sunLight;
uniform vec3 cameraDirection;
uniform float phong;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 2) in vec3 inNormal;

out vec2 texCoord;
smooth out vec4 pDiffuse;
smooth out vec4 pSpecular;
smooth out vec3 pNormal;
void main()
{
	gl_Position = projectionMatrix*modelViewMatrix*vec4(inPosition, 1.0);
	texCoord = inCoord;
	pNormal = inNormal;
	
	if(phong < 1)
	{
		vec3 diffuse, specular;
	
		if(sunLight.vColor != vec4(0.0, 0.0, 0.0, 1.0))
		{
			float diffuseIntensity = dot(inNormal, sunLight.vDirection);
			if (diffuseIntensity > 0)
			{
				//diffuse
				diffuse = vec3(sunLight.vColor * diffuseIntensity);
			
				//specular
				vec3 reflection = normalize(reflect(sunLight.vDirection, inNormal));
			
				float specularIntensity = dot(reflection, cameraDirection);
			
				if(specularIntensity > 0)
				{
					specular = vec3(sunLight.vColor * pow(specularIntensity, mtl.ns)* mtl.ks);		
					pDiffuse = vec4(diffuse, 1.0);
					pSpecular = vec4(specular, 1.0);
				}
				else 
				{
					pDiffuse = vec4(diffuse, 1.0);
					pSpecular = vec4(0.0, 0.0, 0.0, 1.0);
				}
			}
			else 
			{
				pDiffuse = vec4(0.0, 0.0, 0.0, 1.0);
				pSpecular = vec4(0.0, 0.0, 0.0, 1.0);
			}
		}
		else 
		{
			pDiffuse = vec4(0.0, 0.0, 0.0, 1.0);
			pSpecular = vec4(0.0, 0.0, 0.0, 1.0);
		}		
	}
}