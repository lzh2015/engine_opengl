#version 330

struct DirectionalLight
{
   vec4 vColor;
   vec3 vDirection;
}; 

uniform samplerCube SkyTexture;
uniform vec3 eyePosition;
uniform DirectionalLight sunLight;
uniform bool volumetric_light;

in vec3 uvPosition;
in vec3 worldPosition;
out vec4 outPutColor;

void main()
{	
	vec4 skyBase;
	if(volumetric_light == true) skyBase = vec4(0.3, 0.3, 0.3, 1);
    else skyBase = texture(SkyTexture, uvPosition);

   	float day_factor = clamp(max(sunLight.vDirection.y*3, 0.0), 0.0, 1.0);
   	
   	vec3 V = normalize(worldPosition - eyePosition);
   	float dotv = max(dot(V, sunLight.vDirection), 0.0);
   	
   	float pow_factor = day_factor * 1750.0 + 25.0;
	float sun_factor = clamp(pow(dotv, pow_factor), 0.0, 1.0);
   	
   	outPutColor = skyBase * day_factor + sunLight.vColor * sun_factor;
   //	outPutColor = skyBase * day_factor;
}