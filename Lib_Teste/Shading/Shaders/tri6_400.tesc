#version 400 core

//Define the number of output vertices.
layout(vertices = 1) out;

//Input control points.
in vec3 posV[];

//Path infomartion.
struct PatchInfo
{
    //Control points.
    vec3 p[6];

    //Texture coordinates.
    vec3 t[6];
};

//Constant texture coordinates to form functions.
const vec3 texCoor[6] = vec3[](vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 1.0), vec3(0.0, 1.0, 1.0),
                         vec3(1.0, 0.5, 0.5), vec3(0.5, 0.5, 1.0), vec3(0.5, 1.0, 0.5));

//Output patch attributes.
out patch PatchInfo outPatch;

void main()
{
    if (gl_InvocationID == 0)
    {
        //Copy output information.
        for (int i = 0; i < 6; i++)
        {
            outPatch.p[i] = posV[i];
            outPatch.t[i] = texCoor[i];
        }

        //TODO: ajust it dynamically.
        //Define the subdivion levels.
        gl_TessLevelInner[0] = 20;

        gl_TessLevelOuter[0] = 10;
        gl_TessLevelOuter[1] = 10;
        gl_TessLevelOuter[2] = 10;
    }
}
