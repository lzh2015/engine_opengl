#version 330 
layout(location = 0) in vec3 Vertex;

uniform mat4 MVP;
uniform mat4 transform;

out vec3 uvPosition;
out vec3 worldPosition;

void main()
{
    uvPosition = vec3(Vertex.x, Vertex.y, Vertex.z);
    if(Vertex.y < 0) uvPosition = vec3(Vertex.x, -Vertex.y, Vertex.z);
    worldPosition = vec3(transform * vec4(Vertex, 1)); 
    gl_Position = MVP * vec4(Vertex.xyz, 1.0);
}