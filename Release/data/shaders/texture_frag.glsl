#version 330

in vec2 texCoord;
smooth in vec4 pDiffuse;
smooth in vec4 pSpecular;
smooth in vec3 pNormal;
out vec4 outputColor;

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

uniform Material mtl;
uniform DirectionalLight sunLight;
uniform vec3 cameraDirection;
uniform float phong;

uniform sampler2D gSampler;

void main()
{
	vec4 vTexColor = texture2D(gSampler, texCoord);
	
	if(phong < 1)
	{	
		outputColor = vTexColor * 0.2 + vTexColor * pDiffuse + pSpecular;
	}
	else 
	{
		vec3 ambient, diffuse, specular;

		ambient = vec3(vTexColor * 0.2);

		if(sunLight.vColor != vec4(0.0, 0.0, 0.0, 1.0))
		{
			float diffuseIntensity = dot(pNormal, sunLight.vDirection);
			if (diffuseIntensity > 0)
			{
				//diffuse

				diffuse = vec3(vTexColor * sunLight.vColor * diffuseIntensity);

				//specular
				vec3 reflection = normalize(reflect(sunLight.vDirection, pNormal));
			
				float specularIntensity = dot(reflection, cameraDirection);
			
				if(specularIntensity > 0)
				{
					specular = vec3(sunLight.vColor * pow(specularIntensity, mtl.ns)* mtl.ks);
					outputColor = vec4(ambient + diffuse + specular, 1.0);
				}
				else 
				{
					outputColor = vec4(ambient + diffuse, 1.0);
				}
			}
			else 
			{
				outputColor = vec4(ambient, 1.0);
			}
		}
		else 
		{
			outputColor = vec4(ambient, 1.0);
		}	
	}
}