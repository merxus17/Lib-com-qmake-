#version 330 core
uniform vec4 brushColor;
uniform vec4 penColor;
uniform sampler1D wireframe;

in vec2 uv;
out vec4 fragmentColor;

void main()
{
    //Obtain the alpha from texture using mipmap.
    float alpha1 = texture( wireframe, uv.x ).r;
    float alpha2 = texture( wireframe, uv.y ).r;

    fragmentColor = brushColor;

    //Apply DECAL
    fragmentColor = mix( fragmentColor, penColor, alpha1 );
    fragmentColor = mix( fragmentColor, penColor, alpha2 );

    fragmentColor = vec4(fragmentColor.xyz, 1.0f);
}
