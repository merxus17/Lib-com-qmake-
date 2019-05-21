#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec2 uv;

void main()
{
    uv = vec2(1.0, 1.0);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    uv = vec2(1.0, 0.0);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    uv = vec2(0.0, 1.0);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
