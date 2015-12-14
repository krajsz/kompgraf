#version 430

layout (points) in;
layout (line_strip, max_vertices = 32) out;

uniform int  u_numControlPoints;
uniform vec4 u_controlPoints[32];
uniform mat4 u_modelViewProjectionMatrix;

in vec4 vColor[];
in vec4 vPos[];

out vec4 fColor;

float factorial(int _num)
{
    float result = 1;
    while (_num-- > 0)
    {
        result *= float(_num);
    }
    return result;
}

float combination(int _n, int _k)
{
    return factorial(_n) /  (factorial( _n - _k )* factorial(_k) );
}
float                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            bernsteinValue(int n, int i, float t)
{
    return combination(n, i) * pow(t, i) * pow(1 - t, n - i);
}
void calculatePoints()
{
    for (float t = 0; t<=1; t+= 0.05)
    {
        vec4 tmpPoint;
        for (int i = 0; i < u_numControlPoints; ++i)
        {
            tmpPoint+= gl_in[0].gl_Position * bernsteinValue(u_numControlPoints - 1, i, t);
        }
        gl_Position =  tmpPoint;
        EmitVertex();
    }
}

void main(void)
{
}


