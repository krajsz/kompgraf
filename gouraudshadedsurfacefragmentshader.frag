#version 430

out vec4 FragColor;
uniform vec3 u_lightDirection;
in vec3 vNormal;

void main(void)
{
    vec3 n = normalize( vNormal);
    float cosine =  dot( u_lightDirection ,n);

    vec4 darker = vec4(0.1, 0.1, 0.1, 0.0);
    vec4 brighter = vec4(0.9, 0.9, 0.9, 0.0);

    FragColor = darker;

    if (cosine > 0.0)
    {
        FragColor = darker + cosine * brighter;
    }
}


