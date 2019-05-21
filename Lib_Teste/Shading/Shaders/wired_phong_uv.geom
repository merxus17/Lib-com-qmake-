#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vNormal[];
in vec3 vLightDir[];

out vec3 gNormal;
out vec3 gLightDir;
out vec2 gUV;

void main()
{
    gNormal = vNormal[0];
    gLightDir = vLightDir[0];
    gUV = vec2(1.0, 1.0);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gNormal = vNormal[1];
    gLightDir = vLightDir[1];
    gUV = vec2(1.0, 0.0);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gNormal = vNormal[2];
    gLightDir = vLightDir[2];
    gUV = vec2(0.0, 1.0);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
