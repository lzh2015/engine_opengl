#version 330

smooth in vec2 uv;

uniform float time;
uniform sampler2D texScreen;
uniform sampler2D texWaterNoiseNM;

out vec4 outputColor;

vec4 UnderWater()
{
	vec4 colorOut;
	float noise_tile = 0.05;
	float noise_factor = 0.02;

	vec2 uvNormal0 = uv*noise_tile;
	uvNormal0.s += time*0.01;
	uvNormal0.t += time*0.01;
	vec2 uvNormal1 = uv*noise_tile;
	uvNormal1.s -= time*0.01;
	uvNormal1.t += time*0.01;
		
	vec3 normal0 = texture2D(texWaterNoiseNM, uvNormal0).rgb * 2.0 - 1.0;
//	vec3 normal1 = texture2D(texWaterNoiseNM, uvNormal1).rgb * 2.0 - 1.0;
//	vec3 normal = normalize(normal0+normal1);
	
	
	colorOut = texture2D(texScreen, uv + noise_factor*normal0.st);
	
	colorOut = clamp(colorOut, vec4(0.0, 0.0, 0.0, 0.0),  vec4(1.0, 1.0, 1.0, 1.0));
	
	return colorOut;
}

void main()
{
	outputColor = UnderWater();
}