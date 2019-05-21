#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>

#include "../Geometry/Vector2D.h"
#include "../Core/Graphics2DItem.h"

namespace rm
{
class TriangleMesh2DItem : public Graphics2DItem
{
public:
    enum class TriangleType: unsigned char
    {
        TRI3 = 0,
        TRI6 = 1
    };

public:
    /**
     * @brief TriangleMesh2DItem - create a new TriangleMesh2DItem with a mesh topology and points.
     * @param mesh - mesh topology.
     * @param points - mesh points.
     * @param t - triangle type. By defautl the triangle type is a linear triangle.
     */
    TriangleMesh2DItem(std::vector<unsigned int>& mesh, std::vector<Point2Df>& points,
                       TriangleType t = TriangleType::TRI3);

    /**
     * @brief ~TriangleMesh2DItem - Destructor.
     */
    virtual ~TriangleMesh2DItem() override;

    /**
     * @brief initialize - Create all necessary resources.
     */
    void initialize() override;

    /**
     * @brief isInitialized - Check if all OpenGL resources are initialized.
     * @return - True if all OpenGL resources are initialized and false otherwise.
     */
    bool isInitialized();

    /**
     * @brief render - Render function. Receives a view id as parameter to know where to draw. It should group shaders
     * and draw functions.
     * @param viewId - View's identifier.
     */
    void render(int viewId) override;

    /**
     * @brief getWireframeLineThickness - Get the current wireframe line thickness.
     * @return - Current wireframe line thickness.
     */
    float getWireframeLineThickness();

    /**
     * @brief setWireframeLineThickNess - Define a new wireframe line thickness value.
     * @param thickness - New wireframe line thickness value.
     */
    void setWireframeLineThickNess(float thickness);

    /**
     * @brief isIntersecting - Verify if the mesh is intersecting by a mouse click.
     * @param point - Mouse coordinates.
     * @return - True if the mesh is intersecting by a mouse click and false otherwise.
     */
    bool isIntersecting(const Point2Df& point) const override;

    /**
     * @brief cellSelect - Compute the cell the is selected by a mouse click.
     * @param mousePosition - Mouse coordinates.
     * @return - Index of a selected cell or -1 if no cell is intersected.
     */
    int cellSelect(const Point2Df& mousePosition);

private:
    /**
     * @brief createVao - Create and configure new VAO. It needs to add the vao id and their pointer to map structure.
     * @param id - GraphicsView id.
     */
    void createVao(int id);

    /**
     * @brief checkVao - Check if vao was already created to this Item.
     * @param id -  - GraphicsView id.
     */
    void checkVao(int id);

    /**
     * @brief computeAABB - compute object AABB;
     */
    void computeAABB();

    /**
     * @brief createProgram - create a GLSL program for TRI3 triangles..
     */
    void createTri3Program();

    /**
     * @brief createTri6Program - create a GLSL program for TRI6 triangles.
     */
    void createTri6Program();

    //TODO REMOVE: just for research tests.
    void createTri6ProgramForDebug();

    /**
     * @brief createBuffers - Create OpenGL buffers.
     */
    void createBuffers();

    /**
     * @brief createWireframeTexture - Create wireframe texture and assign to atributte _wireframeTexture.
     */
    void createWireframeTexture();

    /**
     * @brief updateVertexBuffer - Update the verterx buffer if it is initialized.
     */
    void updateVertexBuffer();

private:
    struct LocationVariables
    {
        /**
         * @brief mvp - OpenGL identifier for model view projection matrix.
         */
        int mvp {-1};

        /**
         * @brief radius - OpenGL identifier for radius variable.
         */
        int radius {-1};

        /**
         * @brief brushColor - OpenGL identifier for brush color variable.
         */
        int brushColor {-1};

        /**
         * @brief penColor - OpenGL identifier for pen color variable.
         */
        int penColor {-1};

        /**
         * @brief penRatio - OpenGL identifier for pen ratio variable.
         */
        int brushRatio {-1};

        /**
         * @brief wireframe - Wireframe texture location.
         */
        int wireframe{-1};
    };

    /**
     * @brief _locations - Store all necessary locations to polyline shader.
     */
    LocationVariables _locations;

    /**
     * @brief _mesh - Triangle mesh topology.
     */
    std::vector<unsigned int> _mesh;

    /**
     * @brief _wireframeTexture - Wireframe texture id.
     */
    GLuint _wireframeTexture = static_cast<GLuint>(-1);

    /**
     * @brief _wireframeLineThickness - Wireframe line thickness.
     */
    float _wireframeLineThickness {1.0f};

    /**
     * @brief _program - GLSL program.
     */
    QOpenGLShaderProgram* _program {nullptr};

    /**
     * @brief _points - Triangle mesh points.
     */
    std::vector<Point2Df> _points;

    /**
     * @brief _vbo - The item's vertex buffer object
     */
    unsigned int _vboPoints =  static_cast<unsigned int>(-1);

    //TODO REMOVE: just for T6 tests.
    unsigned int _vboColor =  static_cast<unsigned int>(-1);

    /**
     * @brief _ebo - The item's elements buffer object
     */
    unsigned int _ebo = static_cast<unsigned int>(-1);

    /**
     * @brief _type - triangle type. It determines the number of vertex for each triangle.
     */
    TriangleType _type;
};
}
