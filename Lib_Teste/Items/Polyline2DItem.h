#pragma once
#include <utility>
#include "PointSet2DItem.h"
#include "../Core/Graphics2DItem.h"
#include "../Core/Graphics2DView.h"
#include "../Events/GraphicsScenePressEvent.h"
#include "../Events/GraphicsSceneMoveEvent.h"
namespace rm
{

class Polyline2DItem: public Graphics2DItem
{

public:
    /**
     * @brief The PenCapStyle - Enum defines how the end points of lines are drawn Polyline2DItem. The cap style only
     * apply to wide lines, i.e. when the width is 1 or greater. The PenCapStyle enum provides the following styles:
     */
    enum class PenCapStyle : unsigned char
    {
      None        = 255,
      Round       = 0,
      TriangleOut = 1,
      Square      = 2
    };

public:
    /**
     * @brief Polyline2DItem - Empty constructor.
     */
    explicit Polyline2DItem();

    /**
     * @brief Polyline2DItem - Define a polyline using a set of points p.
     * @param p - Set of points sorted to define the new polyline.
     * @param isClosed - Define if the polyline is closed or not.
     */
    explicit Polyline2DItem(const std::vector<Point2Df>& p, bool isClosed = false);

    /**
     * @brief Destructor - Free all resource data.
     */
    ~Polyline2DItem() override;

    /**
     * @brief add - Add a point to the end of polyline's point data structure.
     * @param p - Point to add.
     * @return - The number of point in the current vector.
     */
    unsigned int add(const Point2Df& p);

    /**
     * @brief addInPlace - Add a point between specifc positions.
     * @param p - Point to be added.
     * @param index - Position to be inserted after.
     * @return - The number of point in the current vector.
     */
    int addInPlace(const Point2Df& p, unsigned int index);

    /**
     * @brief closed - Define if the current Polyline2DItem is closed or not.
     * @param c - True to close the polyline and false to open it.
     */
    void closed(bool c);

    /**
     * @brief computeSegementIntersection - Check if a point intersects with any polyline segment.
     * @param p - Point to be tested against the polyline segments.
     * @return - The index of the first segment point, or Polyline2DItem::NO_INTERSECTS if the point no intersects any
     * polyline segment.
     */
    unsigned int computeSegmentIntersection(const Point2Df& p) const;

    /**
     * @brief getAABB - Get the current AABB from the object. This AABB do not take in account the line width or point
     * point size. If you need this information, see getAABBRender method.
     * @return - Current AABB from the object.
     */
    const AABB2D& getAABB() const override;

    /**
     * @brief getAABBRender - Get the current AABB from the object. This AABB take in account the objects render
     * attributes like point radius, line thickeness...If you want just AABB information, withou line width or point
     * size, see getAABB() method.
     * @param pixelSize - The pixel size used to compute the aabb.
     * @return - Current AABB from the object.
     */
    AABB2D getAABBRender(const Point2Df& pixelSize = {0,0}) const override;

    /**
     * @brief getPenCapStyle - get the current pattern that defines how the end points of lines are drawn.
     * @return - enum value that defines how the end points of lines are drawn.
     */
    PenCapStyle getPenCapStyle() const;

    /**
     * @brief getPointSetItem - Returns the point set item used to trace this polyline.
     * @return - The polyline point set item.
     */
    PointSet2DItem& getPointSetItem();

    /**
     * @brief getPointSetItem - Returns the point set item used to trace this polyline.
     * @return - The polyline point set item.
     */
    const PointSet2DItem& getPointSetItem() const;

    /**
     * @brief getPreviewPointSetItem - Return the point set item used as preview in the polyline.
     * @return  - A reference to pointset item used as preview.
     */
    PointSet2DItem& getPreviewPointSetItem();

    /**
     * @brief getPreviewPointSetItem - Return the point set item used as preview in the polyline.
     * @return  - A const reference to pointset item used as preview.
     */
    const PointSet2DItem& getPreviewPointSetItem() const;

    /**
     * @brief getWorldLineWidth - Get squared line with in world metrics.
     * @return - Squared line with in world metrics.
     */
    float getWorldLineWidth() const;

    /**
     * @brief initialize - Initialize OpenGL resources as programs, buffers and textures.
     */
    void initialize() override;

    /**
     * @brief isClosed - Verify if the current Polyline2DItem is closed or not.
     * @return - True for closed and false otherwise.
     */
    bool isClosed() const;

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
     * @brief multLeftMatrix - Multiply the current matrix to the matrix m on left. This is equivalent to do a = m *a;
     * @param m - Matrix to be multiplied buy the current matrix.
     * @return - A const reference to the new current matrix.
     */
    virtual void multLefModelMatrix(const OpenGLMatrix &m) override;

    /**
     * @brief multMatrix - Multiply the current matrix to the matrix m on righ. This is equivalent to do a *= m;
     * @param m - Matrix to be multiplied buy the current matrix.
     * @return - A const reference to the new current matrix.
     */
    virtual void multModelMatrix(const OpenGLMatrix &m) override;

    /**
     * @brief popModelMatrix - Remove the matrix, if it exists, of the top of the stack e turn it as current matrix.
     */
    virtual void popModelMatrix() override;

    /**
     * @brief pushModelMatrix - Stack a copy of the current model matrix.
     */
    virtual void pushModelMatrix() override;

    /**
     * @brief render - Render the current polyline with the current set visualization properties.
     * @param viewId - Identifier of wich view is requesting to render the object.
     */
    void render(int viewId) override;

    /**
     * @brief setModelMatrix - Define a new model matrix stack.
     * @param m - new model matrix stack.
     */
    void setModelMatrix(const OpenGLMatrix& m) override;

    /**
     * @brief setModelToIdentity - Define the top model matrix to identity.
     */
    void setModelToIdentity() override;

    /**
     * @brief setPenCapStyle - set a new pattern to draw the end points of lines.
     * @param c - New pattern to draw the end points of lines.
     */
    void setPenCapStyle(PenCapStyle c);

    /**
     * @brief translate - Translate the object by the vector t.
     * @param t - Translation vector.
     */
    void translate(const QVector3D& t) override;

    /**
     * @brief scale - Scale the object.
     * @param s - Scales factors.
     */
    void scale(const QVector3D& s) override;

    /**
     * @brief segmentMove - Move a segment.
     */
    void segmentMove();

    /**
     * @brief setProjectionMatrix - Sets a new projection matrix stack of the item.
     * @param proj - New projection matrix stack.
     */
    virtual void setProjectionMatrix(const OpenGLMatrix& proj) override;

    /**
     * @brief setPreviewPoint - Sets preview point coordiantes.
     * @param point - The point that is going to be set.
     */
    void setPreviewPoint(const Point2Df& p);

    /**
     * @brief setPreviewVisibility - Sets if the preview is visible or not
     * @param visibility - Is true or false
     */
    void setPreviewVisibility(bool visibility);

    /**
     * @brief getAABB2DItem - Get the AABBItem reponsible for render the objects AABB.
     * @return - A pointer to AABB2DItem.
     */
    AABB2DItem* getAABB2DItem() override;

    /**
     * @brief getBorderSize - Gets the border size in pixels.
     * @return - Returns the border size in pixels.
     */
    const float& getBorderSize() const;

    /**
     * @brief setBorderSize - sets a floating value to the border size of the item.
     * @param size - The floating positive value of the new border size.
     */
    void setBorderSize(float size);

    /**
     * @brief getLineTol - Compute line tolerance for intersection purposes.
     * @param pixelSize - pixel size used to calculate tolerance.
     * @return - Tolerance to be considered.
     */
    float getLineTol(const Point2Df& pixelSize) const;

private:
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
        int brushRatio{-1}; //TODO: remover

        /**
         * @brief type - OpenGL identifier for cap style.
         */
        int capStyle{-1};
    };

    /**
     * @brief _pointSetItem - A point set instance to control the points that define the polyline.
     */
    PointSet2DItem _pointSetItem;

    /**
     * @brief _previewPoint - Point set used to visualize a preview point.
     */
    PointSet2DItem _previewPoint;

    /**
     * @brief _borderSize - size of the border in px.
     */
    float _borderSize {borderSizeDefaultValue()};

    /**
     * @brief _isClosed - Define if the polyline a closed o opened.
     */
    bool _isClosed{isClosedDefaultValue()};

    /**
     * @brief _program - Shader program that renders the polyline.
     */
    QOpenGLShaderProgram* _program = nullptr;

    /**
     * @brief _capStyle - Defines how the end points of lines are drawn Polyline2DItem.
     */
    PenCapStyle _capStyle = PenCapStyle::Round;

    /**
     * @brief _locations - Store all necessary locations to polyline shader.
     */
    LocationVariables _locations;

private:
    /**
     * @brief createVao - Create and configure, if necessary, a new VAO. It needs to add the vao id and their pointer to map structure.
     * @param id - View id that is used as identifier to VAO. The OpenGL states that is necessary one VAO per view.
     */
    void createVao(int id);

    /**
     * @brief checkVao - Check if vao was already created to this Item, otherwise call createVao() function to create a
     * new on.
     * @param id - View id that is used as identifier to VAO. The OpenGL states that is necessary one VAO per view.
     */
    void checkVao(int id);

    /**
     * @brief createProgram - create an OpenGL program.     
     */
    void createProgram();

    /**
     * @brief createBuffers - Create OpenGL buffers.
     */
    void createBuffers();

    /**
     * @brief borderSizeDefaultValue - Gets the border size default
     * @return - Default border size value.
     */
    constexpr static float borderSizeDefaultValue() { return 2.0f; }

    /**
     * @brief isClosedDefaultValue - Define a default value to isClosed parameter, which in turn defines if the current
     * polyline is opened or closed.
     * @return - Default to state to define if a polyline is closed or not.
     */
    constexpr static bool isClosedDefaultValue() { return false; }
};
}

