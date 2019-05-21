#pragma once
#include <set>
#include "../Core/Graphics2DItem.h"
#include "../Geometry/Vector2D.h"

/**
 * Foward declarations.
 */
class  QVector3D;


namespace rm
{
class Group2DItem : public Graphics2DItem
{
public:
    /**
     * @brief GroupItem2D - Contructs the group item with a list of itens
     * @param itens - vector of  itens
     */
    Group2DItem(const std::set<Graphics2DItem*>& itens);

    /**
     * @brief ~Group2DItem - Destructor. Clears the the group item then deletes it
     */
    virtual ~Group2DItem() override;

    /**
     * @brief initialize - Virtual function of graphicsitem. Should group shaders and initialize buffers. Needs to be implemented
     * by the class' children.
     */
    virtual void initialize() override;

    /**
     * @brief render - Virtual function of render. It should contain whatever is necessary to draw on the correct context.
     * @param viewId - View's identifier. Where the item will be rendered.
     */
    virtual void render(int viewId) override;

    /**
     * @brief isIntersecting - Checks if a given point intersects the item. Should be implemented by all itens.
     * @param clickPoint
     * @return bool
     */
    virtual bool isIntersecting(const Point2Df& point) const override ;

    /**
     * @brief -  It only clears the set containter of items and returns ownership of the itens to the scene. It's the
     * tool responsability re-add to the scene each item from group before calling the ungroup function, or else will
     * lost track of the item's pointers.
     * @return - A set containing the group items for re-add to the scene.
     */
    std::set<Graphics2DItem*> ungroup();

    /**
     * @brief scale - Scale the object.
     * @param s - scales factors.
     */
    void scale(const QVector3D& s) override;

    /**
     * @brief translate - Translate the object by the vector t.
     * @param t - translation vector.
     */
    void translate(const QVector3D& t) override;

    /**
     * @brief setBrushColor - Set color to fill the object.
     * @param color - New color to fill the object.
     */
    virtual void setBrushColor(const QVector3D& color) override;

    /**
     * @brief setPenColor - set color used to paint lines on object contour
     * @param color - new color used to paint lines on object contour
     */
    virtual void setPenColor(const QVector3D& color) override;

    /**
     * @brief visible - Define the object visibility.
     * @param v - True to turn on the visibility and false otherwise.
     */
    virtual void visible(bool v) override;

    /**
     * @brief setLineWidth - Define a new line width value. This value is used to render the item lines.
     * @param width - New line width value.
     */
    virtual void setLineWidth(float width) override;

    /**
     * @brief - rotates the itens in the group
     * @param r - rotate factors.
     * @param angle - angle of the rotation.
     */
    void rotate(const QVector3D& r, const float angle) override;

    /**
     * @brief setModelMatrix - Define a new model matrix.
     * @param m - new model matrix.
     */
    void setModelMatrix(const OpenGLMatrix& m) override;

    /**
     * @brief Sets projection matrix of the item
     */
    void setProjectionMatrix(const OpenGLMatrix &proj) override;

    /**
     * @brief pushModelMatrix - Stack a copy of the current model matrix.
     */
    virtual void pushModelMatrix() override;

    /**
     * @brief popModelMatrix - Remove the matrix, if it exists, of the top of the stack e turn it as current matrix.
     */
    virtual void popModelMatrix() override;

    /**
     * @brief multMatrix - Multiply the current matrix to the matrix m on righ. This is equivalent to do a *= m;
     * @param m - Matrix to be multiplied buy the current matrix.
     * @return - A const reference to the new current matrix.
     */
    virtual void multModelMatrix(const OpenGLMatrix &m) override;

    /**
     * @brief multLeftMatrix - Multiply the current matrix to the matrix m on left. This is equivalent to do a = m *a;
     * @param m - Matrix to be multiplied buy the current matrix.
     * @return - A const reference to the new current matrix.
     */
    virtual void multLefModelMatrix(const OpenGLMatrix &m) override;

    /**
     * @brief setOnFocus - Set this item as the item on focus
     * @param onFocus true to set the item on focus, false to unset
     */
    void onFocus(bool onFocus) override;

    /**
     * @brief size - Get the number of elements inside container.
     * @return - Return the size of set.
     */
    size_t size() const;

    /**
     * @brief containsItem - Check if item is in the current group
     * @param item - item to be searched in the group
     * @return - true if the group contains item and false otherwise
     */
    bool containsItem(Graphics2DItem* item) const;

    /**
     * @brief getItems - Method to get the items inside of the set container.
     * @return - Reference for the items container.
     */
    const std::set<Graphics2DItem*>& getItems() const;

    /**
     * @brief getAABBRender - Get the current AABB from the object. This AABB take in account the objects render
     * attributes like point radius, line thickeness...
     * @param pixelSize - The pixel size used to compute the aabb
     * @return - Current AABB from the object.
     */
    virtual AABB2D getAABBRender(const Point2Df& pixelSize = {0,0}) const override;

    /**
     * @brief getAABB - Get the current AABB from the object.
     * @return - Current AABB from the object.
     */
    virtual const AABB2D& getAABB() const override;

protected:

    /**
     * @brief - calculates aabb of the itens as a group
     */
    void computeAABB();

protected:
     /**
     * @brief _items - list of the itens within the group
     */
    std::set<Graphics2DItem*> _items ;

private:
    /**
     * @brief _recomputeAABB - Determine if the AABB must to be recomputed or not.
     */
    mutable bool _recomputeAABB = false;
};
}
