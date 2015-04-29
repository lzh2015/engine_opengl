#version 330

struct Material
{
	vec4 color;
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

layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec3 inNormal;

smooth out vec4 pColor;

void main()
{
	gl_Position = projectionMatrix*modelViewMatrix*vec4(inPosition, 1.0);
	vec3 ambient, diffuse, specular;
	ambient = vec3(mtl.color * 0.2);

	if(sunLight.vColor != vec4(0.0, 0.0, 0.0, 1.0))
	{
		float diffuseIntensity = dot(inNormal, sunLight.vDirection);
		if (diffuseIntensity > 0)
		{
			//diffuse
			diffuse = vec3(mtl.color * sunLight.vColor * diffuseIntensity);
			
			//specular
			vec3 reflection = normalize(reflect(sunLight.vDirection, inNormal));
			
			float specularIntensity = dot(reflection, cameraDirection);
			
			if(specularIntensity > 0)
			{
				specular = vec3(sunLight.vColor * pow(specularIntensity, mtl.ns)* mtl.ks);
				pColor = vec4(ambient + diffuse + specular, 1.0);
			}
			else 
			{
				pColor = vec4(ambient + diffuse, 1.0);
			}
		}
		else 
		{
			pColor = vec4(ambient, 1.0);
		}
	}
	else 
	{
		pColor = vec4(ambient, 1.0);
	}	
}