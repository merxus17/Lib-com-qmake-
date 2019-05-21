#version 330 core

uniform vec4 brushColor;
uniform vec4 penColor;
uniform vec2 brushRatio;

in vec3 uvw;
out vec4 fragColor;

void main()
{
    //Adjust brush ratio once the coordinate textures space is the half of that used for circle and square.
    vec2 newBrushRatio = vec2(1.0f - (1.0f - brushRatio.x) * 0.5f,
                              1.0f - (1.0f - brushRatio.y) * 0.5f);

    float dx = length(vec2(dFdx(uvw.x), dFdy(uvw.x)));
    float t = smoothstep(newBrushRatio.x - dx, newBrushRatio.x + dx, uvw.x);
    fragColor = mix(brushColor, penColor, t);

    float dy = length(vec2(dFdx(uvw.y), dFdy(uvw.y)));
    t = smoothstep(newBrushRatio.x - dy, newBrushRatio.x + dy, uvw.y);
    fragColor = mix(fragColor, penColor, t);

    float dz = length(vec2(dFdx(uvw.z), dFdy(uvw.z)));
    t = smoothstep(newBrushRatio.y - dz, newBrushRatio.y + dz, uvw.z);
    fragColor = mix(fragColor, penColor, t);


    t = smoothstep(1 - dx, 1 + dx, uvw.x);
    fragColor = mix(fragColor, vec4(fragColor.xyz, 0), t);

    t = smoothstep(1 - dy, 1 + dy, uvw.y);
    fragColor = mix(fragColor, vec4(fragColor.xyz, 0), t);

    t = smoothstep(1 - dz, 1 + dz, uvw.z);
    fragColor = mix(fragColor, vec4(fragColor.xyz, 0), t);
}
