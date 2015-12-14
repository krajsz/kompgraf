#version 430

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 Normal;
uniform mat4 u_modelViewProjectionMatrix;
uniform mat4 u_modelMatrix;
uniform vec3 u_lightDirection;

flat out vec4 vColor;
void main(void)
{
    gl_Position = u_modelViewProjectionMatrix * vec4(Vertex, 1.0);

    vec3 n = vec4(u_modelMatrix* vec4(Normal, 1.0)).xyz;
    n = normalize(n);

    float cosine = dot( u_lightDirection ,n);

    vec4 darker = vec4(0.2, 0.2, 0.2, 0.0);
    vec4 brighter = vec4(0.8, 0.8, 0.8, 0.0);

    vColor = darker;

    if (cosine > 0.0)
    {
        vColor = darker + cosine * brighter;
    }
}

