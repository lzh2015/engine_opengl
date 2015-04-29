#version 330

struct DirectionalLight
{
   vec4 vColor;
   vec3 vDirection;
   vec3 vPosition;
}; 

smooth in vec2 uv;
smooth in vec3 pVertex;

uniform sampler2D gSampler;
uniform sampler2D texWaterNoiseNM;
uniform bool volumetric_light;

uniform int win_x;
uniform int win_y;
uniform float time;

uniform DirectionalLight sunLight;
uniform vec3 eyePosition;

out vec4 outputColor;


float Fresnel(vec3 incident, vec3 normal, float bias, float power)
{
	float scale = 1.0 - bias;
	return bias + pow(1.0 - dot(incident, normal), power) * scale;
}


void main()
{
	float noise_tile = 10.0f;
	float noise_factor = 0.1f;
	float water_shininess = 50.0f;
	
	vec2 uvNormal0 = uv*noise_tile;
	uvNormal0.x += time*0.01;
	uvNormal0.y += time*0.01;
	vec2 uvNormal1 = uv*noise_tile;
	uvNormal1.x -= time*0.01;
	uvNormal1.y += time*0.01;
	
	vec3 normal0 = texture2D(texWaterNoiseNM, uvNormal0).rgb * 2.0 - 1.0;
	vec3 normal1 = texture2D(texWaterNoiseNM, uvNormal1).rgb * 2.0 - 1.0;
	vec3 normal = normalize(normal0+normal1);
	
	vec2 uvReflection = vec2(gl_FragCoord.x/win_x, gl_FragCoord.y/win_y);
	
	vec2 uvFinal = uvReflection.xy + noise_factor*normal.xy;
	outputColor = texture2D(gSampler, uvFinal);
	
	vec3 N = vec3(normal.x, normal.z, normal.y);
	vec3 L = sunLight.vDirection;
	vec3 V = normalize(pVertex - eyePosition);
	float iSpecular = pow(clamp(dot(reflect(L, N), V), 0.0, 1.0), water_shininess);
	
	outputColor += sunLight.vColor * iSpecular;
	
	// fresnel alpha
	outputColor.a =  Fresnel(-V, N, 0.5, 2.0);
	
	if(volumetric_light == true)
	{
		outputColor = vec4(0,0,0,1);
		return;
	}
	
	
	//outputColor = texture2D(gSampler, uvReflection);
}