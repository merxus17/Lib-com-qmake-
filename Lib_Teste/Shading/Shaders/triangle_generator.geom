#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

//View projection matrix.
uniform mat4 vp;

//Model matrix.
uniform mat4 m;

uniform vec2 r;

out vec3 uvw;

void main()
{
   vec4 p = m * gl_in[0].gl_Position;

   float f = 1.1;
   //2 * sqrt(3) / 2
   float constant = 1.154700538;
   uvw = vec3(f, f, 1 - f);
   gl_Position = vp * (p + vec4(0, 4 * f * r.y / 3, 0, 0));
   EmitVertex();

   uvw = vec3(1 - f, f, f);
   gl_Position = vp * (p + vec4(-f * constant * r.x, -2 * f * r.y / 3, 0, 0));
   EmitVertex();

   uvw = vec3(f, 1 - f, f);
   gl_Position = vp * (p + vec4(f * constant * r.x, -2 * f * r.y / 3, 0, 0));
   EmitVertex();

   EndPrimitive();
}
