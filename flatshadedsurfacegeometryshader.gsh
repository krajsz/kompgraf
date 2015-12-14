#version 430

layout(triangles) in;

layout(triangle_strip, max_vertices=3) out;
uniform vec3 u_ambientLight;
out vec4 fColor;
void main(void)
{
    vec3 l = normalize(u_ambientLight);

    vec3 a = gl_in[0].gl_Position.xyz;
    vec3 b = gl_in[1].gl_Position.xyz;
    vec3 c = gl_in[2].gl_Position.xyz;

    vec3 u = b-a;
    vec3 v = c-a;

    vec3 face_normal = normalize(cross(u, v));
    float cosine =  dot(l, face_normal);

    vec4 face_color_darker =   vec4(0.4, 0.4, 0.4, 0.0);
    vec4 face_color_brighter = vec4(0.8, 0.8, 0.8, 0.0);

    fColor = face_color_darker;

    if  (cosine > 0.0)
    {
        fColor = face_color_darker + cosine*face_color_brighter;
    }

    gl_Position = vec4(a, 1.0);
    EmitVertex();

    gl_Position = vec4(b, 1.0);
    EmitVertex();

    gl_Position = vec4(c, 1.0);
    EmitVertex();

    EndPrimitive();
}
