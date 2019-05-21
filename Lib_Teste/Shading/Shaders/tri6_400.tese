#version 400 core

layout(triangles, equal_spacing, ccw) in;

uniform mat4 mvp;

struct PatchInfo
{
    vec3 p[6];
    vec3 t[6];
};

in patch PatchInfo outPatch;
out vec3 uvw;
void main()
{
    float l0 = gl_TessCoord.x;
    float l1 = gl_TessCoord.y;
    float l2 = gl_TessCoord.z;

    float n0 = l0 * (2 * l0 - 1);
    float n1 = l1 * (2 * l1 - 1);
    float n2 = l2 * (2 * l2 - 1);
    float n3 = 4 * l0 * l1;
    float n4 = 4 * l1 * l2;
    float n5 = 4 * l2 * l0;

    //Compute vertex position.
    vec3 position = n0 * outPatch.p[0] + n1 * outPatch.p[1] + n2 * outPatch.p[2] +
                    n3 * outPatch.p[3] + n4 * outPatch.p[4] + n5 * outPatch.p[5];

    //Compute texture coordinates to wireframe view.
    uvw = n0 * outPatch.t[0] + n1 * outPatch.t[1] + n2 * outPatch.t[2] +
            n3 * outPatch.t[3] + n4 * outPatch.t[4] + n5 * outPatch.t[5];

    //Project vertex coordiantes.
    gl_Position = mvp * vec4(position, 1.0f);
}
