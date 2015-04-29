#version 330 core

// Ouput data
//layout(location = 0) out float fragmentdepth;

out vec4 outputColor;

void main()
{
	float depth = gl_FragCoord.z;
	float dx = dFdx(depth);
	float dy = dFdy(depth);
	float moment2 = depth * depth + 0.25 * (dx * dx + dy * dy);
	outputColor = vec4(depth, moment2, 0.0, 0.0);
}