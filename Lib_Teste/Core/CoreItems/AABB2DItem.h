#pragma once
#include "../../Geometry/AxisAligmentBoundingBox.h"

namespace rm
{
/**
 * Forward declarations.
 */
class Polyline2DItem;
class PointSet2DItem;
class OpenGLMatrix;
/**
 * This class is responsible for rendering an AABB2D of an object.
 * The AABB2D is passed as a reference and will always be in accordance to the
 * item's aabb it is supposed to wrap.
 * The method update(), however, must be called every time the aabb changes,
 * so the item can update its points in the vbo.
 * Before calling the render() method, the user is supposed to set the desired
 * model matrix (probably the same matrix of the aabb's object)
 */
class AABB2DItem
{
public:
	/**
     * @brief Constructor.
     */
    explicit AABB2DItem();

    /**
     * @brief getPointSetItem - Returns the point set item used to trace this AABB.
     * @return - The AABB point set item.
     */
    PointSet2DItem& getPointSetItem();

    /**
     * @brief getPointSetItem - Returns the point set item used to trace this AABB.
     * @return - The AABB point set item.
     */
    const PointSet2DItem& getPointSetItem() const;

    /**
     * @brief getPreviewPointSetItem - Returns the point set item used as preview in the AABB.
     * @return - A reference to pointset item used as preview.
     */
    PointSet2DItem& getPreviewPointSetItem();

    /**
     * @brief getPreviewPointSetItem - Returns the point set item used as preview in the AABB.
     * @return - A reference to pointset item used as preview.
     */
    const PointSet2DItem& getPreviewPointSetItem() const;

    /**
     * @brief initialize - Compile and links shaders and initializes buffers.
     */
    void initialize();

    /**
     * @brief isInsideToItemAABB - Verify if a point p, in world coordinates, is inside to AABB item.
     * @param p - point in world coordinates to be tested.
     * @param pixelSize - pixelSize of the view.
     * @return - true if the point is inside to AABB and false othewise.
     */
    bool isInsideToItemAABB(const Point2Df& p, const Point2Df& pixelSize) const;

    /**
     * @brief isVisible - Verifies if the current item is visible.
     * @return - Returns true if the item is visible and false otherwise.
     */
    bool isVisible() const;

    /**
     * @brief render - Render function. At this point, it should contains whatever is necessary to draw on the correct
     * context.
     * @param viewId - View's identifier. Where the item will be rendered.
     * @param projection - Projection matrix provided by canvas to be used to draw the aabb.
     * @param model - Model matrix provided by object that has the AABB to be used to draw the aabb.
     * @param pixelSize - Pixel size information provided by 2D canvas.
     */
    void render(int viewId, const OpenGLMatrix& projection, const OpenGLMatrix &model, const Point2Df& pixelSize);

    /**
     * @brief setBrushColor - Sets color to fill the object.
     * @param color - New color to fill the object.
     */
    void setBrushColor(const QVector4D& color);

    /**
     * @brief setLineWidth - Defines a new line width value. This value is used to render the item lines.
     * @param width - New line width value.
     */
    void setLineWidth(float width);

    /**
     * @brief setPenColor - Sets color used to paint lines on object contour.
     * @param color - New color used to paint lines on object contour.
     */
    void setPenColor(const QVector4D& color);

	/**
     * @brief showHandles - Sets the visibility of the box editing handles.
     * @param show - True to show the handles and false to hide them.
     */
	void showHandles(bool show);

    /**
     * @brief update - Recomputes and update the pointset points based on the aabb provided.
     */
    void update(const AABB2D& aabb);

    /**
     * @brief visible - Defines the object visibility.
     * @param visible - True to turn on the visibility and false otherwise.
     */
    void visible(bool v);

    /**
     * @brief ~AABB2DItem - Destructor.
     */
    ~AABB2DItem();
private:
    /**
     * @brief _aabbItem - Object to represent the AABB.
     */
    Polyline2DItem* _polyline = nullptr;
};
}
