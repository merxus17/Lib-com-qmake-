#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

//View projection matrix.
uniform mat4 vp;

//Model matrix.
uniform mat4 m;

uniform vec2 r;

out vec2 uv;

void main()
{
   vec4 p = gl_in[0].gl_Position;

   float f = 1.1;
   uv = vec2(-f, +f);
   gl_Position = vp * m * (p + vec4(-f * r.x, +f * r.y, 0, 0));
   EmitVertex();

   uv = vec2(-f, -f);
   gl_Position = vp * m * (p + vec4(-f * r.x, -f * r.y, 0, 0));
   EmitVertex();

   uv = vec2(+f, +f);
   gl_Position = vp * m * (p + vec4(+f * r.x, +f * r.y, 0, 0));
   EmitVertex();

   uv = vec2(+f, -f);
   gl_Position = vp * m * (p + vec4(+f * r.x, -f * r.y, 0, 0));
   EmitVertex();

   EndPrimitive();
}
