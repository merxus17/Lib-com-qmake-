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
class TriangleMesh3DItem : public Graphics3DItem
{
public:
	/**
     * @brief TriangleMesh3DItem  - Constructor
	 */
    TriangleMesh3DItem(std::vector<unsigned int>& mesh, std::vector<Point3Df>& points);

	/**
     * @brief TriangleMesh3DItem  - Deconstructor
	 */
    ~TriangleMesh3DItem() override;

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
     * @param id identifier of view to be draw
     */
    void render(int id) override;

    /**
     @brief isIntersecting - Receives a screen point and answer if intersects mesh.
     * @param p - screen point to be tested for intersection
     * @return
     */
    bool isIntersecting(const Point2Df& p) const override;

    /**
     * @brief cellSelect - Receives a screen point and answer which cell intersects(index).
     * @param p - screen point
     * @return
     */
    int cellSelect(const Point2Df& p);

    /**
     * @brief getLineThickness Wireframe line thickness
     * @return the tickness used to draw wireframe
     */
    float getWireframeLineThickness();

    /**
     * @brief setLineThickNess Sets the wireframe line thickness
     * @param thickness New thickness
     */
    void setWireframeLineThickNess(float thickness);

private:

    /**
     * @brief createVao - Create and configure new VAO. It needs to add the vao id and their pointer to map structure.
     * @param id vao id
     */
    void createVao(int id);

    /**
     * @brief checkVao - Check if vao was already created to this Item
     * @param id vao id
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
     * @brief updateVertexBuffer - update vertex buffer
     */
    void updateVertexBuffer();

    /**
     * @brief createProgram - create an OpenGL program.
     */
    void createProgram();

    /**
     * @brief createWireframeTexture  - Create wireframe texture and assign to atributte _wireframeTexture.
     */
    void createWireframeTexture();

    /**
     * @brief computeNormals - Computes triangle mesh normals
     */
    void computeNormals();

private:
    struct LocationVariables
    {
        /**
         * @brief mvp - opengl identifier for model view projection matrix.
         */
        int mvp{-1};

        /**
         * @brief lpos - light position.
         */
        int lpos{-1};

        /**
         * @brief penColor - opengl identifier for pen color variable.
         */
        int penColor{-1};

        /**
         * @brief diffuseLight - diffuse light component.
         */
        int diffuseLight{-1};

        /**
         * @brief ambientLight - ambiet light component.
         */
        int ambientLight{-1};

        /**
         * @brief diffuseMaterial - diffuse material component.
         */
        int diffuseMaterial{-1};

        /**
         * @brief ambientMaterial - ambiet material component.
         */
        int ambientMaterial{-1};

        /**
         * @brief normalMatrix - matriz to transform normals.
         */
        int normalMatrix{-1};

        /**
         * @brief mv - view matriz location.
         */
        int mv{-1};

        /**
         * @brief wireframe - wireframe texture location.
         */
        int wireframe{-1};
    };

    /**
     * @brief _locations - store all necessary locations to polyline shader.
     */
    LocationVariables _locations;

    /**
     * @brief _mesh Vector holding mesh element indexes
     */
    std::vector<unsigned int> _mesh;

    /**
     * @brief _wireframeTexture wireframe texture
     */
    GLuint _wireframeTexture = static_cast<GLuint>(-1);

    /**
     * @brief _wireframeLineThickness Line thickness for wireframe texture.
     */
    float _wireframeLineThickness = 1.0f;

    /**
     * @brief _vertexBuffer buffer holding triangle mesh 3d vertexes
     */
    unsigned int _vertexBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _normalsBuffer buffer holding triangle mesh 3d normals
     */
    unsigned int _normalsBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _elementBuffer holding triangle mesh 3d elementBuffer
     */
    unsigned int _elementBuffer = static_cast<unsigned int>(-1);

    /**
     * @brief _program triangle mesh 3d shader program.
     */
    QOpenGLShaderProgram* _program {nullptr};

    /**
     * @brief _points triangle mesh points
     */
    std::vector<Point3Df> _points;

    /**
     * @brief _normals triangle mesh normals
     */
    std::vector<Vector3Df> _normals;
};
};
