#version 430

out vec4 FragColor;
uniform vec3 u_color;

void main(void)
{
   FragColor = vec4(u_color, 1.0);
}

