#pragma once
#include <utility>
#include "PointSet2DItem.h"
#include "../Core/Graphics2DItem.h"
#include "../Core/Graphics2DView.h"
#include "../Events/GraphicsScenePressEvent.h"
#include "../Events/GraphicsSceneMoveEvent.h"
namespace rm
{

class Rectangle2DItem: public Graphics2DItem
{

public:
    /**
     * @brief Rectangle2DItem - Empty constructor.
     */
    explicit Rectangle2DItem();

    /**
     * @brief Rectangle2DItem - Define a rectangle item using a center point, width and height.
     * @param centerPoint - Rectangle's center point
     * @param width - Rectangle's width
     * @param height - Rectangle's height
     */
    explicit Rectangle2DItem(const Point2Df& centerPoint, float width, float height);

    /**
     * @brief Destructor - Free all resource data.
     */
    ~Rectangle2DItem() override;

    /**
     * @brief initialize - Initialize OpenGL resources as programs, buffers and textures.
     */
    void initialize() override;

    /**
     * @brief isInitialized - Check if OpenGL resources are initialized or not.
     * @return - True if all OpenGL resources are initialized and false otherwise.
     */
    bool isInitialized() const;

    /**
     * @brief intersectWith - Check intersection of a point p with the polyline.
     * @param p - Point to be tested with the polyline point.
     * @return - True if the point intersect with polyline and false otherwise.
     */
    bool isIntersecting(const Point2Df& p) const override;

    /**
     * @brief render - Render the current polyline with the current set visualization properties.
     * @param viewId - Identifier of wich view is requesting to render the object.
     */
    void render(int viewId) override;

    /**
     * @brief getWidth - Returns Rectangle's width
     * @return - Current rectangle's width.
     */
    float getWidth();

    /**
     * @brief setWidth - Set a value to width
     * @param width - The new rectangle's width.
     */
    void setWidth(float width);

    /**
     * @brief getHeight - Returns Rectangle's height
     * @return - Current rectangle's height.
     */
    float getHeight();

    /**
     * @brief setHeight - Set a value to height
     * @param height - The new rectangle's height.
     */
    void setHeight(float height);

    /**
     * @brief getCenterPoint - Returns Rectangle's center point.
     * @return - Current rectangle's top left point.
     */
    const Point2Df& getCenterPoint();

    /**
     * @brief getCenterPoint - Set a value of Rectangle's center point.
     * @param point - The new rectangle's top left point.
     */
    void setCenterPoint(const Point2Df& point);

    /**
     * @brief getBorderSize - Gets the border size in pixels.
     * @return - Returns the border size in pixels.
     */
    const float& getBorderSize() const;

    /**
     * @brief setBorderSize - Sets a floating value to the border size of the item.
     * @param s - The floating positive value of the new border size.
     */
    void setBorderSize(float size);

    /**
     * @brief updateRectangle - Sets a floating value to the border size of the item.
     * @param centerPoint - The new rectangle's center point.
     * @param width - The new rectangle's width.
     * @param height - The new rectangle's height.
     */
    void updateRectangle(const Point2Df& centerPoint, float width, float height);

private:

    /**
     * @brief createVao - Create and configure, if necessary, a new VAO. It needs to add the vao id and their pointer to map structure.
     * @param id - View id that is used as identifier to VAO. The OpenGL states that is necessary one VAO per view.
     */
    void createVao(int id);

    /**
     * @brief checkVao - Check if vao was already created to this Item, otherwise call createVao() function to create a
     * new one.
     * @param id - View id that is used as identifier to VAO. The OpenGL states that is necessary one VAO per view.
     */
    void checkVao(int id);

    /**
     * @brief createProgram - Create an OpenGL program.
     */
    void createProgram();

    /**
     * @brief createBuffers - Create OpenGL buffers.
     */
    void createBuffers();

    /**
     * @brief updateVertexBuffer - Updates vertex buffer.
     */
    void updateVertexBuffer();

    /**
     * @brief computeAABB - Computes object's AABB.
     */
    void computeAABB();
    
    /**
     * @brief getDefaultBorderSize - Returns de default border size in pixels.
     * @return - Returns default border size.
     */
    constexpr static float getDefaultBorderSize() { return 2.0f; }


private:
    /**
     * @brief _centerPoint - Center point that define the rectangle.
     */
    Point2Df _centerPoint;

    /**
     * @brief _width - Rectangle's width.
     */
    float _width;

    /**
     * @brief _height - Rectangle's height.
     */
    float _height;

    /**
     * @brief _borderSize - Size of the border in pixels.
     */
    float _borderSize {getDefaultBorderSize()};
        
    /**
     * @brief _program - Shader program that renders the rectangle
     */
    QOpenGLShaderProgram* _program = nullptr;

    /**
     * @brief _vertexbuffer - Buffers identifiers.
     */
    unsigned int _vertexBuffer = static_cast<unsigned int>(-1);

    struct LocationVariables
    {
        /**
         * @brief vp - Opengl identifier for view projection matrix.
         */
        int vp{-1};

        /**
         * @brief m - Opengl identifier for model matrix.
         */
        int m{-1};

        /**
         * @brief radius - Opengl identifier for radius variable.
         */
        int radius{-1};

        /**
         * @brief brushColor - Opengl identifier for brush color variable.
         */
        int brushColor{-1};

        /**
         * @brief penColor - Opengl identifier for pen color variable.
         */
        int penColor{-1};

        /**
         * @brief penRatio - Opengl identifier for pen ratio variable.
         */
        int borderSize{-1};
        /**
         * @brief penRatio - OpenGL identifier for pen ratio variable.
         */
        int brushRatio {-1};
    };

    /**
     * @brief _locations - Store all necessary locations to polyline shader.
     */
    LocationVariables _locations;
};
}
