#version 330

smooth in vec2 uv;

const int NUM_SAMPLES = 100 ;

uniform float x;
uniform float y;
uniform mat4 biasMatrix;
uniform mat4 camProjectMat;

uniform float density;
uniform float weight;
uniform float decay;
uniform float exposure;
uniform vec4 screenLightPos;

uniform sampler2D firstPass;
uniform sampler2D secondPass;

out vec4 outputColor;

void main()
{
	float px = 2*x*uv.x - x;
	float py = 2*y*uv.y - y;
	vec4 firstPass_position = vec4(px, py, -0.1, 1.0);
	vec4 firstPass_uv = biasMatrix * camProjectMat * firstPass_position;
	firstPass_uv /= firstPass_uv.w;
	
	vec2 texCoord = firstPass_uv.xy;
	vec2 deltaTextCoord = vec2(texCoord - screenLightPos.xy );
    deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * density;
    float illuminationDecay = 1.0;
    vec4 light = texture2D(firstPass, texCoord);
    
    //outputColor = light;
    
    for(int i=0; i < NUM_SAMPLES; i++)
	{
		texCoord -= deltaTextCoord;
		vec4 sample = texture2D(firstPass, texCoord);
		
		sample *= illuminationDecay * weight;
		
		light += sample;
		
		illuminationDecay *= decay;
	}
	
	vec4 color = texture2D(secondPass, uv);
	outputColor = vec4((color.rgb+light.rgb*exposure)*0.8, 1);
	//outputColor = vec4(light.rgb*exposure, 1);
}