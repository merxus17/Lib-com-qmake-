#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 vNormal[];
in vec3 vLightDir[];

out vec3 gNormal;
out vec3 gLightDir;
out vec3 gUVW;

void main()
{
    gNormal = vNormal[0];
    gLightDir = vLightDir[0];
    gUVW = vec3(1.0, 1.0, 0.0);
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    gNormal = vNormal[1];
    gLightDir = vLightDir[1];
    gUVW = vec3(0.0, 1.0, 1.0);
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();

    gNormal = vNormal[2];
    gLightDir = vLightDir[2];
    gUVW = vec3(1.0, 0.0, 1.0);
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    EndPrimitive();
}
