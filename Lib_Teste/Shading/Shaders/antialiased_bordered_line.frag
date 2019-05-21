#version 330 core
uniform vec4 brushColor;
uniform vec4 penColor;
uniform float brushRatio;
uniform int capStyle;

in float p;
in vec2 uv;

out vec4 colorFragment;

void main()
{
    //Compute color for corner fragments.
    float u = abs(uv.x);
    float dx = (u - 1) / (p - 1);
    float v = abs(uv.y);

    //Default join.
    float r = p;

    //Round.
    if (capStyle == 0)
    {
      r = sqrt(dx * dx + v * v);
    }
    //Triangle out.
    else if (capStyle == 1)
    {
      r = dx + v;
    }
    //Square
    else if (capStyle == 2)
    {
      r = max(dx, v);
    }

    //Compute gradient norm for corner fragments.
    float stepWidth = length(vec2(dFdx(r), dFdy(r)));

    //Compute a color between brush and pen color to corner fragment.
    float t = smoothstep(brushRatio - stepWidth, brushRatio + stepWidth, r);
    vec4 cornerColor = mix(brushColor, penColor, t);

    t = smoothstep(1 - stepWidth, 1 + stepWidth, r);
    cornerColor = mix(cornerColor, vec4(cornerColor.xyz, 0), t);

    //Compute segment fragment color.
    //Compute gradient norm to segment fragments.
    stepWidth = length(vec2(dFdx(v), dFdy(v)));

    //Compute a color between brush and pen color to corner fragment.
    t = smoothstep(brushRatio - stepWidth, brushRatio + stepWidth, v);
    vec4 segmentColor = mix(brushColor, penColor, t);

    t = smoothstep(1 - stepWidth, 1 + stepWidth, v);
    segmentColor = mix(segmentColor, vec4(segmentColor.xyz, 0), t);

    //Compute a color between segment and corner color.
    stepWidth = length(vec2(dFdx(u), dFdy(u)));

    t = smoothstep(1 - stepWidth, 1 + stepWidth, u);
    colorFragment = mix(segmentColor, cornerColor, t);
}
