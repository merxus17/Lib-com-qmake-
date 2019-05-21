#pragma once
#include <set>
#include "../Core/Graphics3DItem.h"

/**
 * Foward declarations.
 */
class QVector3D;
namespace rm
{


class Group3DItem  : public Graphics3DItem
{
public:
    /**
     * @brief GroupItem3D - Contructs the group item with a list of itens
     * @param itens - vector of  itens
     */
    Group3DItem(const std::set<Graphics3DItem*>& itens);

    /**
     * @brief - Clears the the group item then deletes it
     */
    virtual ~Group3DItem() override ;

    /**
     * @brief -  Virtual function of graphicsitem. Should group shaders and initialize buffers. Needs to be implemented by the class' children.
     */
    virtual void initialize() override;

    /**
     * @brief render - Virtual function of GraphicsItem. It should contain whatever is necessary to draw
     * on the correct context, and should be implemented by the children.
     * @param viewId - View's identifier. Where the item will be rendered.
     */
    virtual void render(int viewId) override;

    /**
     * @brief Checks if a given point intersects the item. Should be implemented by all itens.
     * @param clickPoint
     * @return bool
     */
    virtual bool isIntersecting(const Point2Df& point) const override ;

    /**
     * @brief ungroup - It only clears the set containter of items and returns ownership of the itens to the scene. It's the
     * tool responsability re-add to the scene each item from group before calling the ungroup function.
     * @return - A set containing the group items for re-add to the scene.
     */
    std::set<Graphics3DItem*> ungroup();

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
     * @param visible - True to turn on the visibility and false otherwise.
     */
    virtual void visible(bool visible) override;

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
     * @brief setOnFocus - Set this item as the item on focus
     * @param onFocus true to set the item on focus, false to unset
     */
    void onFocus(bool onFocus) override;

    /**
     * @brief getSize - Get the number of elements inside container.
     * @return - Return the size of set.
     */
    size_t getSize();
protected:

    /**
     * @brief - calculates aabb of the itens as a group
     */
    void computeAABB();

protected:
    /**
     * @brief _items - list of the itens within the group
     */
    std::set<Graphics3DItem*> _items ;
};
}
