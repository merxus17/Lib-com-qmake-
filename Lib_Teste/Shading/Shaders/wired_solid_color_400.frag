#version 400 core
uniform vec4 brushColor;
uniform vec4 penColor;
uniform sampler1D wireframe;

in vec3 uvw;
out vec4 fragmentColor;

void main()
{
    //Obtain the alpha from texture using mipmap.
    float alpha1 = texture( wireframe, uvw.x ).r;
    float alpha2 = texture( wireframe, uvw.y ).r;
    float alpha3 = texture( wireframe, uvw.z ).r;

    fragmentColor = brushColor;

    //Apply DECAL
    fragmentColor = mix( fragmentColor, penColor, alpha1 );
    fragmentColor = mix( fragmentColor, penColor, alpha2 );
    fragmentColor = mix( fragmentColor, penColor, alpha3 );

    fragmentColor = vec4(fragmentColor.xyz, 1.0f);
}
