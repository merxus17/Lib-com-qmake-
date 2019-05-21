#pragma once

#include "GraphicsItem.h"
#include "../Geometry/AxisAligmentBoundingBox.h"
#include "../Shading/ShadingModel.h"

namespace rm
{
class Graphics3DItem: public GraphicsItem
{
public:
    /**
     * Allow a Graphics3DView to set item's shading model.
     */
    friend class Graphics3DView;

    /**
     * Empty Constructor
     */
    Graphics3DItem() = default;

    /**
     * Empty Destructor
     */
    virtual ~Graphics3DItem() override ;

    /**
     * @brief initialize - Virtual function of graphicsitem. Should group shaders and initialize buffers.
     * It needs to be implemented by the class' children.
     */
    virtual void initialize() override = 0;

    /**
     * @brief render - Virtual function of GraphicsItem. It should contain whatever is necessary to draw
     * on the correct context, and should be implemented by the children.
     * @param viewId - View's identifier. Where the item will be rendered.
     */
    virtual void render(int viewId) override = 0;

    /**
     * @brief getAABB - get the current AABB from the object.
     * @return - current AABB from the object.
     */
    virtual const AABB3D& getAABB() const;

    /**
     * @brief getAABB - get the current AABB from the object.
     * @return - current AABB from the object.
     */
    virtual AABB3D& getAABB();

    /**
     * @brief isIntersecting - Virtual function of graphicsitem so it needs to be implemented by the class' children. The method receives a screen point and answer if intersects the item.
     * @param clickPoint - the point that was clicked by the user
     */
    virtual bool isIntersecting(const Point2Df& clickPoint)  const override = 0;

    /**
     * @brief setViewMatrix - Sets the current view matrix.
     * @param viewMatrix - matrix to be set
     */
    void setViewMatrix(const OpenGLMatrix &viewMatrix);
protected:
     /**
      * @brief _shadingModel - The shading model.
      */
    ShadingModel _shadingModel;

     /**
      * @brief _viewMatrix - The current view matrix.
      */
     OpenGLMatrix _viewMatrix;

     /**
      * @brief _aabb - Current 3D AABB.
      */
     AABB3D _aabb;

private:
    /**
     * @brief setShadingModel - Sets a shading model to the 3D item.
     * @param m - the shading model.
     */
    void setShadingModel(ShadingModel m);
};
};

