#version 330 core

layout(lines) in;
layout(triangle_strip, max_vertices = 4) out;

//View projection matrix.
uniform mat4 vp;

//Model matrix.
uniform mat4 m;

//r-> radius in x direction. Used to increase the segment lenght e draw different join styles.
//r-> line thickness.
uniform float r;

out vec2 uv;
out float p;
void main()
{
    //Transform points by model matrix to get the world positions.
    vec4 p1 = m * gl_in[0].gl_Position;
    vec4 p2 = m * gl_in[1].gl_Position;

    //Factor.
    float f = 1.5;

    vec2 u = p2.xy - p1.xy;
    float l = length(u);

    u = normalize(u);
    vec2 v = vec2(-u.y, u.x);

    vec2 w = (r * f) * u;
    vec2 k = (r * f) * v;
    float t = (l + 2.0 * r * f) / l;
    p = (l + 2.0 * r) / l;

    //Construct the rectangle in world space to avoid deformation caused by model matrix. Transform
    //the resulting points by view projection matrix.
    uv = vec2(-t, +f);
    gl_Position = vp * (p1 + vec4(-w + k, 0, 0));
    EmitVertex();

    uv = vec2(-t, -f);
    gl_Position = vp * (p1 + vec4(-w - k, 0, 0));
    EmitVertex();

    uv = vec2(+t, +f);
    gl_Position = vp * (p2 + vec4(+w + k, 0, 0));
    EmitVertex();

    uv = vec2(+t, -f);
    gl_Position = vp * (p2 + vec4(+w - k, 0, 0));
    EmitVertex();

    EndPrimitive();
}
