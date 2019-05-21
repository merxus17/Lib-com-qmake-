#version 330 core

uniform vec4 brushColor;
uniform vec4 penColor;
uniform vec2 brushRatio;

in vec2 uv;
out vec4 fragColor;

void main()
{
   //Get absolute value.
   float r = length(uv);

   //Compute gradient norm.
   float stepWidth = length(vec2(dFdx(r), dFdy(r)));

   //Compute transition from brush color to pen color.
   float t = smoothstep(brushRatio.x - stepWidth, brushRatio.y + stepWidth, r);
   fragColor = mix(brushColor, penColor, t);

   //Compute transition from circle color to border color.
   t = smoothstep(1 - stepWidth, 1 + stepWidth, r);
   fragColor = mix(fragColor, vec4(fragColor.xyz, 0), t);
}
