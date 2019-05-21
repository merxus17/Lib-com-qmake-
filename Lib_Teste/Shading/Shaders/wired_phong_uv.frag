#version 330 core
uniform vec4 brushColor;
uniform vec4 penColor;
uniform sampler1D wireframe;
uniform vec3 diffuseLight;
uniform vec3 ambientLight;
uniform vec3 diffuseMaterial;
uniform vec3 ambientMaterial;

in vec3 gNormal;
in vec3 gLightDir;
in vec2 gUV;
out vec4 fragmentColor;

void main()
{
    vec3 n = normalize(gNormal);
    vec3 color = vec3(0, 0, 0);
    vec3 l = normalize(gLightDir);
    color += max(0.0f, dot(n, l)) * (diffuseMaterial * diffuseLight);

    color += ambientMaterial * ambientLight;

    //Obtain the alpha from texture using mipmap.
    float alpha1 = texture( wireframe, gUV.x ).r;
    float alpha2 = texture( wireframe, gUV.y ).r;

    fragmentColor = vec4(color, 1.0f);

    //Apply DECAL
    fragmentColor = mix( fragmentColor, penColor, alpha1 );
    fragmentColor = mix( fragmentColor, penColor, alpha2 );
}
