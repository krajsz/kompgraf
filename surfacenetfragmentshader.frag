#version 430

uniform vec3 u_color ;
out vec4 FragColor;
void main(void)
{
    vec4 color = vec4(u_color, 1.0);
    FragColor = color;
}

