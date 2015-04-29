#version 330

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;
layout (location = 2) in vec3 inNormal;

uniform mat4 inverse;
uniform mat4 transform;
uniform mat4 rotate;
uniform mat4 vp;

uniform int bInverse;
uniform float move_speed;
uniform float move_offset;
uniform float time;
uniform vec3 lightDir;
uniform vec4 lightColor;
uniform float water_height;

smooth out vec2 pTexCoord;
smooth out vec3 vPosition;
smooth out vec4 pDiffuseColor;

void main()
{
	mat4 mvp;

	if(bInverse == 1)
	{
		mvp = vp * inverse * transform;
	}
	else
	{
		mvp = vp * transform;
	}
	
	vPosition = vec3(inverse * transform * vec4(inPosition, 1.0));
	
	vec3 vertex = inPosition;
	// tree movement
	vertex.x += 0.01 * pow(vertex.y, 2.0) * cos(time * move_speed + move_offset);
	
	gl_Position = mvp*vec4(vertex, 1.0);
	pTexCoord = inTexCoord;
	
	vec4 translated_normal = rotate * vec4(inNormal, 1.0);
	float diffuseIntensity = dot(normalize(translated_normal.xyz), lightDir);
	
	if (diffuseIntensity > 0) 
	{
		pDiffuseColor = lightColor * diffuseIntensity;
		pDiffuseColor.a = 1.0;
	}
	else
	{
		pDiffuseColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
	
}