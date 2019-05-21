#include "WireframeTextureBuilder.h"
#include <QOpenGLFunctions>
namespace rm
{
unsigned int buildWireframeTexture(float lineThickness)
{
    //Texture wireframe Id.
    unsigned int wireframeId;

    //Create a new texture.
    glGenTextures( 1, &wireframeId );

    //Update the texture with correct thickness.
    uptadeWireframeTexture(wireframeId, lineThickness);

    //Return wireframe texture identifier.
    return wireframeId;
}



void uptadeWireframeTexture(unsigned int textureId, float lineThickness)
{
    //Build texture vector.
    const int TAM = 1024;
    unsigned char wireframeTexture[TAM];
    memset( wireframeTexture, 0, TAM * sizeof (unsigned char ) );

    int idx = 0;
    while (lineThickness > 1.0f && idx < TAM)
    {
        wireframeTexture[TAM - 1 - idx] = 255;
        lineThickness = lineThickness - 1.0f;
        idx++;
    }

    if (idx < TAM)
    {
        wireframeTexture[TAM - 1 - idx] = static_cast<unsigned char>(255 * lineThickness);
    }

    //Turn the texture as current.
    glBindTexture( GL_TEXTURE_1D, textureId );

    //Build mipmap pyramid.
    int i = 0, j = TAM;
    for (i = 0, j = TAM; j > 4; j /= 2, i++)
    {
        glTexImage1D( GL_TEXTURE_1D, i, GL_RED, j, 0, GL_RED, GL_UNSIGNED_BYTE, wireframeTexture + ( TAM - j ) );
    }

    //Avoiding image saturation for rendering dense meshes.
    for (int s = 0; s <= idx; s++)
    {
        wireframeTexture[TAM - 1 - s] /= 3;
    }

    for (; j > 0; j /= 2, i++)
    {
        glTexImage1D( GL_TEXTURE_1D, i, GL_RED, j, 0, GL_RED, GL_UNSIGNED_BYTE, wireframeTexture + ( TAM - j ) );
    }

    //Define filters.
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
}
}
