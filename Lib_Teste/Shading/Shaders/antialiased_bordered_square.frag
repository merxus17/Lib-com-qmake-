#version 330 core

uniform vec4 brushColor;
uniform vec4 penColor;
uniform vec2 brushRatio;

in vec2 uv;
out vec4 fragColor;

void main()
{
   //Get absolute value.
   vec2 absuv = abs(uv);

   //Compute grandient vector for radius.
   float r = length(uv);
   vec2 dfdxy = vec2( abs(dFdx(r)), abs(dFdy(r)));

   //Compute transition from brush color to pen color.
   vec2 t = smoothstep(brushRatio - dfdxy, brushRatio + dfdxy, absuv);
   fragColor = mix(brushColor, penColor, t.x);
   fragColor = mix(fragColor, penColor, t.y);

   //Compute transition from square color to border color.
   t = smoothstep(1 - dfdxy, 1 + dfdxy, absuv);
   fragColor = mix(fragColor, vec4(fragColor.xyz, 0), t.x);
   fragColor = mix(fragColor, vec4(fragColor.xyz, 0), t.y);
}
