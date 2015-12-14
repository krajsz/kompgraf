#version 430

layout(location = 0) in vec4 Vertex;
uniform mat4 u_modelViewProjectionMatrix;

void main(void)
{
    gl_Position = u_modelViewProjectionMatrix * Vertex;
}


