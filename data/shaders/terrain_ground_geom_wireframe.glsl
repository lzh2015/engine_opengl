#version 330

in vec2 uv[];
in vec3 normal[];

layout(triangles) in;
layout(triangle_strip, max_vertices = 9)out;

uniform mat4 mvp;
uniform vec3 lightDir;

smooth out vec3 vLightDirTanSpace;
smooth out vec2 vTexCoord; 
smooth out vec3 pNormal;
out vec3 tangent;
out vec3 bitangent;


void main()
{	
	vec3 v0 = gl_in[0].gl_Position.xyz;
	vec3 v1 = gl_in[1].gl_Position.xyz;
	vec3 v2 = gl_in[2].gl_Position.xyz;
	
	vec3 deltaPos1 = v1 - v0;
	vec3 deltaPos2 = v2 - v0;
	
	vec2 deltaUV1 = uv[1]-uv[0];
    vec2 deltaUV2 = uv[2]-uv[0];
    
    float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
    
    vec3 tangent, bitangent;
    
    tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)* r;
    tangent = normalize(tangent);
    
    bitangent = (deltaPos2 * deltaUV1.x   - deltaPos1 * deltaUV2.x)* r;
    bitangent = normalize(bitangent);

   	mat3 mTBN;
    
    for(int i = 0; i < 1; i++)
    {
 		gl_Position = mvp*gl_in[0].gl_Position;
 			mTBN = transpose(mat3(tangent, bitangent, normal[0]));
		vLightDirTanSpace = normalize(mTBN * lightDir);

 		vTexCoord = uv[0];
 		pNormal = normal[0];
  		EmitVertex();

 		gl_Position = mvp*gl_in[1].gl_Position;
 		 	mTBN = transpose(mat3(tangent, bitangent, normal[1]));
		vLightDirTanSpace = normalize(mTBN * lightDir);			
 		vTexCoord = uv[1];
 		pNormal = normal[1];
  		EmitVertex();
  	
  		gl_Position = mvp*gl_in[2].gl_Position;
  			mTBN = transpose(mat3(tangent, bitangent, normal[2]));
		vLightDirTanSpace = normalize(mTBN * lightDir);			
  		vTexCoord = uv[2];
  		pNormal = normal[2];
  		EmitVertex();
  		
  		EndPrimitive();
  	}
  	
  
}