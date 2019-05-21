#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <QMatrix4x4>

#include "../Geometry/Vector2D.h"
#include "../Core/Graphics3DItem.h"
#include "../Events/GraphicsScenePressEvent.h"
#include "../Events/GraphicsSceneHoverEvent.h"

using Vector3Df = QVector3D;
using Point3Df = QVector3D;

namespace rm
{
class QuadMesh3DItem : public Graphics3DItem
{
public:
    /**
     * @brief QuadMesh3DItem  - Constructor
     */
    QuadMesh3DItem(std::vector<unsigned int>& mesh, std::vector<Point3Df>& points);

    /**
     * @brief QuadMesh3DItem  - Destructor
     */
    ~QuadMesh3DItem() override;

    /**
     * @brief initialize  - Link and bind vertex, fragment and geometry shader, initialize VBO
     */
    void initialize() override;

    /**
     * @brief isInitialized  - Check if item's shaders are linked
     */
    bool isInitialized();

    /**
     * @brief render  - Render function. Receives a view as parameter to know where to draw. It should group shaders and draw functions.
     * @param viewId - identifier of view to be draw
     */
    void render(int viewId) override;

    /**
     @brief isIntersecting - Receives a screen point and answer if intersects mesh.
     * @param p - Screen point to be tested for intersection
     * @return If the point intersects the mesh or not
     */
    bool isIntersecting(const Point2Df& p) const override;

    /**
     * @brief cellSelect - Receives a screen point and answer which cell intersects(index).
     * @param p - Screen point
     * @return The index of the cell there is being intersected
     */
    int cellSelect(const Point2Df& p);


    /**
     * @brief getLineThickness -  Wireframe line thickness
     * @return The tickness used to draw wireframe
     */
    float getWireframeLineThickness();

    /**
     * @brief setLineThickNess - Sets the wireframe line thickness
     * @param thickness - New thickness
     */
    void setWireframeLineThickNess(float thickness);

private:

    /**
     * @brief createVao - Create and configure new VAO. It needs to add the vao id and their pointer to map structure.
     * @param id - VAO's id
     */
    void createVao(int id);

    /**
     * @brief checkVao - Check if vao was already created to this Item
     * @param id - VAO's id
     */
    void checkVao(int id);

    /**
     * @brief computeAABB - compute the QuadMesh3D AABB.
     */
    void computeAABB();

    /**
     * @brief createPrograms - Creates the shader programs
     */
    void createPrograms();

    /**
     * @brief createBuffers - Create OpenGL buffers
     */
    void createBuffers();

    /**
     * @brief updateVertexBuffer - Update vertex buffer
     */
    void updateVertexBuffer();

    /**
     * @brief createProgram - Create an OpenGL program.
     */
    void createProgram();

    /**
     * @brief createWireframeTexture  - Create wireframe texture and assign to atributte _wireframeTexture.
     */
    void createWireframeTexture();

    /**
     * @brief computeNormals - Computes quad mesh normals
     */
    void computeNormals();

    /**
     * @brief quadToTriangleMesh - Transforms the QuadMesh into a TriangleMesh with the proper configuration
     *                             for the wireframe.
     */
    void quadToTriangleMesh();

private:
    struct LocationVariables
    {
        /**
         * @brief mvp - OpenGL identifier for model view projection matrix.
         */
        int mvp{-1};

        /**
         * @brief lpos - Light position.
         */
        int lpos{-1};

        /**
         * @brief penColor - OpenGL identifier for pen color variable.
         */
        int penColor{-1};

        /**
         * @brief diffuseLight - Diffuse light component.
         */
        int diffuseLight{-1};

        /**
         * @brief ambientLight - Ambient light component.
         */
        int ambientLight{-1};

        /**
         * @brief diffuseMaterial - Diffuse material component.
         */
        int diffuseMaterial{-1};

        /**
         * @brief ambientMaterial - Ambient material component.
         */
        int ambientMaterial{-1};

           /**
         * @brief normalMatrix - Matrix to transform normals.
         */
        int normalMatrix{-1};

        /**
         * @brief mv - Model view matrix location.
         */
        int mv{-1};

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
     * @brief _mesh - Vector holding mesh element indexes
     */
    std::vector<unsigned int> _mesh;

    /**
     * @brief _wireframeTexture -  Wireframe texture
     */
    GLuint _wireframeTexture = static_cast<GLuint>(-1);

    /**
     * @brief _wireframeLineThickness - Line thickness for wireframe texture.
     */
    float _wireframeLineThickness = 0.8f;

    /**
     * @brief _vertexBuffer - Buffer holding quad mesh 3d vertexes
     */
    unsigned int _vertexBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _normalsBuffer - Buffer holding quad mesh 3d normals
     */
    unsigned int _normalsBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _elementBuffer - Holding quad mesh 3d elementBuffer
     */
    unsigned int _elementBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _program - Quad mesh 3d shader program.
     */
    QOpenGLShaderProgram* _program {nullptr};

    /**
     * @brief _points - Quad mesh points
     */
    std::vector<Point3Df> _points;

    /**
     * @brief _normals - Quad mesh normals
     */
    std::vector<Vector3Df> _normals;
};
};
