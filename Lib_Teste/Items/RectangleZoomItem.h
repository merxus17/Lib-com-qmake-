#pragma once

#include <vector>

#include "../Geometry/Vector2D.h"
#include "../Core/Graphics2DItem.h"
#include "../Core/GraphicsScene.h"

namespace rm
{
class RectangleZoomItem : public GraphicsItem
{
public:
    RectangleZoomItem() = default;

    /**
     * @brief Destructor
     */
    ~RectangleZoomItem() override;

    /**
     * @brief setFirstPoint - Updates first point position.
     * @param p - First point new position
     */
    void setFirstPoint(const Point2Df& p);

    /**
     * @brief setSecondPoint - Updates second point position.
     * @param p - Second point new position
     */
    void setSecondPoint(const Point2Df& p);

    /**
     * @brief render - render the current object on current context.
     * @param viewId - context that must to be used to render the item.
     */
    void render(int viewId) override;

    /**
     * @brief isInitialized - verify if the item has been already initiazed.
     * @return - true if the item is initialized and false otherwise.
     */
    bool isInitialized() const;

    /**
     * @brief initialize - initialize opengl objects to render.
     */
    virtual void initialize() override;

    /**
     * @brief isIntersecting Check intersection of screen point with item
     * @param clickPoint
     * @return bool
     */
    bool isIntersecting(const Point2Df& clickPoint) const override;

    /**
     * @brief getFirstPoint  - Return the bottom left point defining the zoom rectangle
     * @return Zoom rectangle bottom left point
     */
    const Point2Df& getFirstPoint() const;

    /**
     * @brief getFirstPoint  - Return the bottom left point defining the zoom rectangle
     * @return Zoom rectangle bottom left point
     */
    const Point2Df& getSecondPoint() const;

    /**
     * @brief setCanvasWidth - set the canvas width.
     * @param w - new canvas width.
     */
    void setCanvasWidth(int w);

    /**
     * @brief setCanvasHeight - set the canvas height.
     * @param h - new canvas height.
     */
    void setCanvasHeight(int h);

private:
    struct LocationVariables
    {
        /**
         * @brief mvp - opengl identifier for model view projection matrix.
         */
        int mvp{-1};

        /**
         * @brief brushColor - opengl identifier for brush color variable.
         */
        int brushColor{-1};
    };

    /**
     * @brief _points - Array with four points defining the zoom rectangle.
     */
    Point2Df _points[4];

    /**
     * @brief _firstPoint - First point defining the zoom rectangle.
     */
    Point2Df _firstPoint;

    /**
     * @brief _secondPoint - Second point defining the zoom rectangle.
     */
    Point2Df _secondPoint;

    /**
     * @brief _vertexbuffer - buffers identifiers.
     */
    unsigned int _vertexbuffer = static_cast<unsigned int>(-1);

    /**
     * Quad shader program.
     */
    QOpenGLShaderProgram* _program {nullptr};

    /**
     * @brief _quadLocations - Stores all necessary locations to quad shader.
     */
    LocationVariables _locations;

    /**
     * @brief _width - canvas width.
     */
    int _width {0};

    /**
     * @brief _height - canvas height.
     */
    int _height {0};

private:

    /**
     * @brief createVao  - Create and configure new VAO. It needs to add the vao id and their pointer to map structure.
     * @param id View ID
     */
    void createVao(int id);

    /**
     * @brief checkVao  - Check if vao was already created to this Item
     * @param id View ID
     */
    void checkVao(int id);

    /**
     * @brief createProgram - Create the OpenGL program.
     */
    void createProgram();

    /**
     * @brief createBuffers - Create OpenGL buffers.
     */
    void createBuffers();

    /**
     * @brief updateVertexBuffer - Update vertex buffer.
     */
    void updateVertexBuffer();

    /**
     * @brief fixPointOrder - Fix points order so opengl draws triangles correctly no matter from where user started to
     * draw.
     */
    void fixPointOrder();
};
};
