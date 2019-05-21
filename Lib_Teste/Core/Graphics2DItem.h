#pragma once

#include "GraphicsItem.h"
#include "../Geometry/AxisAligmentBoundingBox.h"
#include "../Geometry/Vector2D.h"

namespace rm
{
class AABB2DItem;

class Graphics2DItem: public GraphicsItem
{
public:
    /**
     * @brief Graphics2DItem Empty Constructor
     */
    Graphics2DItem();

    /**
     * @brief Destructor
     */
    virtual ~Graphics2DItem() override;

    /**
     * @brief setPixelSize - Set a new pixel size values to define the relationship between world and screen metric.
     * @param p - New pixel size.
     */
    void setPixelSize(const Point2Df& p);

    /**
     * @brief getPixelSize - Get the pixel size value.
     * @return - The pixel size value.
     */
    const Point2Df& getPixelSize() const;

    /**
     * @brief initialize - Virtual function of graphicsitem. Should group shaders and initialize buffers. Needs to be
     * implemented by the class' children.
     */
    void initialize() override = 0;

    /**
     * @brief render - Virtual function of render. It should contain whatever is necessary to draw on the correct
     * context.
     * @param viewId - View's identifier. Where the item will be rendered.
     */
    void render(int viewId) override = 0;

    /**
     * @brief getAABB - Get the current AABB from the object.
     * @return - Current AABB from the object.
     */
    virtual const AABB2D& getAABB() const;

    /**
     * @brief setAABB - Update the current AABB with a new AABB.
     * @param aabb - New AABB.
     */
    void setAABB(const AABB2D& aabb);

    /**
     * @brief getAABBRender - Get the current AABB from the object. This AABB take in account the objects render
     * attributes like point radius, line thickeness...
     * @param pixelSize - The pixel size used to compute the aabb
     * @return - Current AABB from the object.
     */
    virtual AABB2D getAABBRender(const Point2Df& pixelSize = {0,0}) const;

    /**
     * @brief getAABB2DItem - Get the AABBItem reponsible for render the objects AABB.
     * @return - A pointer to AABB2DItem.
     */
    virtual AABB2DItem* getAABB2DItem();

    /**
     * @brief isIntersecting - Virtual function of graphicsitem so it needs to be implemented by the class' children.
     * The method receives a screen point and answer if intersects the item.
     * @param point - Point in world coordinates.
     * @return bool - True if intersects the object and false otherwise.
     */
    virtual bool isIntersecting(const Point2Df& point) const override = 0;

    /**
     * @brief isInsideToItemAABB - Verify if a point p, in world coordinates, is inside to AABB item.
     * @param p - point in world coordinates to be tested.
     * @param pixelSize - pixelSize of the view.
     * @return - true if the point is inside to AABB and false othewise.
     */
    virtual bool isInsideToItemAABB(const Point2Df& p, const Point2Df& pixelSize) const;

    /**
     * @brief worldSpace2ModelSpace - Transforms a point from world space to model space.
     * @param p - A world space point.
     * @return - The point p on model space.
     */
    Point2Df worldSpace2ModelSpace(const Point2Df& p) const;

    /**
     * @brief modelSpace2WorldSpace - Transforms a point from model space to world space.
     * @param p - A model space point.
     * @return - The point p on world space.
     */
    Point2Df modelSpace2WorldSpace(const Point2Df& p) const;

    /**
     * @brief updatePixelSize - Update pixelSize by item current scale.
     * @param pixelSize - Item pixel size.
     * @return - Pixel size corrected.
     */
    Point2Df updatePixelSize(const Point2Df& pixelSize) const;

private:
    /**
     * @brief _aabb - AABB representation object.
     */
    AABB2D _aabb;

protected:

    /**
     * @brief _pixelSize - the relationship between pixel and 2D world.
     */
    Point2Df _pixelSize;

    /**
     * @brief _aabbItem - Item that renders the object's AABB.
     */
    AABB2DItem* _aabbItem{nullptr};

    /**
     * @brief _isAABBItemOutdated - Flag to determine if its necessary to update the aabb item. This update will
     * be made just at the moment that the aabb item is requested.
     */
    bool _isAABBItemOutdated = {true};
};
}
