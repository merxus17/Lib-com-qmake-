#pragma once


namespace rm
{
/**
 * @brief buildWireframeTexture - build a texture to be used at wireframe algorithm. The texture is build taking account
 * the desired line thickeness.
 * @param lineThickness - desired wireframe line thickness.
 * @return - OpenGL texture id.
 */
unsigned int buildWireframeTexture(float lineThickness);

/**
 * @brief uptadeWireframeTexture -  update texture with a new wireframe line thickness.
 * @param textureId - OpenGL texture identifier.
 *@param lineThickness - new wirefram line thickeness.
 */
void uptadeWireframeTexture(unsigned int textureId, float lineThickness);
}

