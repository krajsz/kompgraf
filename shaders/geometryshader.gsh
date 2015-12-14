#version 430

layout (points) in;
layout (line_strip, max_vertices = 128) out;
/*uniform int  u_numControlPoints;
uniform vec3 u_controlPoints[32];*/

/*float factorial(int _num)
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
float bernsteinValue(int n, int i, float t)
{
    return combination(n, i) * pow(t, i) * pow(1 - t, n - i);
}
void calculatePoints()
{
    for (float t = 0; t<=1; t+= 0.01)
    {
        vec3 tmpPoint;
        for (int i = 0; i < u_numControlPoints; ++i)
        {
             tmpPoint+= u_controlPoints[i] * bernsteinValue(u_numControlPoints - 1, i, t);
        }
        gl_Position = vec4(tmpPoint, 0);
        EmitVertex();
    }
}*/

void main(void)
{

    vec3 tmp = gl_in[0].gl_Position.xyz + vec3(0.05, 0, 0);
    gl_Position = vec4(tmp, 0);
    EmitVertex();

    tmp = gl_in[0].gl_Position.xyz + vec3(-0.05, 0, 0);
    gl_Position = vec4(tmp, 0);
    EmitVertex();

    EndPrimitive();
}


