#version 330

smooth in vec3 pNormal;
smooth in vec4 shadowMapCoord;

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
uniform sampler2D gSampler;


vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);
		
float SampleLinear()
{
	float bias = 0.005;
	float visibility = 1.0;
	
	for (int i=0;i<8;i++)
	{
		float z = texture2D( gSampler, shadowMapCoord.xy + poissonDisk[i]/700.0).x;
	 	if (z < (shadowMapCoord.z - bias ))
	 	{
 	 		visibility-=0.1;
		}
	}
	
	return visibility;
}

float linstep(float low, float high, float v)
{
	return clamp((v-low)/(high-low), 0.0, 1.0);
}

float SampleVarianceShadowMap()
{
	float varianceMin = 0.000005f;
	float lightBleedReductionAmount = 0.02f;
	vec2 moments = texture2D(gSampler, shadowMapCoord.xy).rg;
	
	float p = step(shadowMapCoord.z, moments.x);
	float variance = max(moments.y - moments.x * moments.x, varianceMin);
//	float variance = moments.y - moments.x * moments.x;
//	variance = max(variance, varianceMin);
	float d = shadowMapCoord.z - moments.x;
	float pMax = linstep(lightBleedReductionAmount, 1.0, variance / (variance + d*d));
	return min(max(p, pMax), 1.0);
	//return step((shadowMapCoord.z - 0.0002), texture2D(gSampler, shadowMapCoord.xy).r);
}

float SampleExponentShadowMap()
{
	float shadow = 1.0;
	float c = 200.0; // shadow coeffecient - change this to to affect shadow darkness/fade
	float texel = texture2D( gSampler, shadowMapCoord.xy ).r;
	shadow = clamp( exp( -c * (shadowMapCoord.z - texel)), 0.0, 1.0 );
	return shadow;
}

float SampleBasciShadowMap()
{
	float shadow = 1.0;
	if((shadowMapCoord.z) > texture2D(gSampler, shadowMapCoord.xy).r)
	{
		shadow = 0.5;
	}
	return shadow;
}

void main()
{
	vec3 ambient, diffuse, specular;

	ambient = vec3(mtl.color * 0.2);
	
//	float visibility = 1.0; 
	float visibility = SampleBasciShadowMap();
//	float visibility = SampleLinear();
//	float visibility = SampleVarianceShadowMap();
//	float visibility = SampleExponentShadowMap();

	if(sunLight.vColor != vec4(0.0, 0.0, 0.0, 1.0))
	{
		float diffuseIntensity = dot(normalize(pNormal), sunLight.vDirection);
		if (diffuseIntensity > 0)
		{
			//diffuse

			diffuse = vec3(mtl.color * sunLight.vColor * diffuseIntensity * visibility);

			//specular
			vec3 reflection = normalize(reflect(sunLight.vDirection, pNormal));
			
			float specularIntensity = dot(reflection, cameraDirection);
			
			if(specularIntensity > 0)
			{
				specular = vec3(sunLight.vColor * pow(specularIntensity, mtl.ns)* mtl.ks * visibility);
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