#version 330 core
layout(location = 0) in vec4 pos;
layout(location = 1) in vec3 n;

out vec3 vNormal;
out vec3 vLightDir;

uniform vec4 lpos;
uniform mat4 mvp;
uniform mat4 mv;
uniform mat3 nm;

void main()
{
    vec3 peye = vec3(mv * pos);
    if (lpos.w == 0)
    {
        vLightDir = normalize(lpos.xyz);
    }
    else
    {
        vLightDir = normalize(lpos.xyz - peye.xyz);
    }

    vNormal = normalize(nm * n);
    gl_Position = mvp * pos;
}
