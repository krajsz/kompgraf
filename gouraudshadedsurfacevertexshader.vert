#version 430

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 Normal;
uniform mat4 u_modelViewProjectionMatrix;
uniform mat4 u_modelMatrix;

out vec3 vNormal;
void main(void)
{
    gl_Position = u_modelViewProjectionMatrix * vec4(Vertex, 1.0);
    vNormal = vec4(u_modelMatrix* vec4(Normal, 1.0)).xyz;
}

