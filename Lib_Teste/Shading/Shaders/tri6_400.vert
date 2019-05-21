#version 400 core
layout(location = 0) in vec3 pos;

out vec3 posV;

void main()
{
    posV = pos;
}
