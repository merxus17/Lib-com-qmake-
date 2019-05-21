#include "Graphics2DItem.h"
#include "CoreItems/AABB2DItem.h"
#include "../Items/Polyline2DItem.h"
#include <iostream>
namespace rm
{

Graphics2DItem::Graphics2DItem() : GraphicsItem()
{
}



Graphics2DItem::~Graphics2DItem()
{
    delete _aabbItem;
}



void Graphics2DItem::setPixelSize(const Point2Df& p)
{
    _pixelSize = p;

    //Set the aabb item to be updated.
    _isAABBItemOutdated = true;
}



const Point2Df& Graphics2DItem::getPixelSize() const
{
    return _pixelSize;
}



const AABB2D& Graphics2DItem::getAABB() const
{
    return _aabb;
}



void Graphics2DItem::setAABB(const AABB2D &aabb)
{
    //Define the new AABB.
    _aabb = aabb;

    //Set the aabb item to be updated.
    _isAABBItemOutdated = true;
}



Point2Df Graphics2DItem::worldSpace2ModelSpace(const Point2Df &p) const
{
    //Transform the point by matrix.
    QVector4D t = _modelMatrix.topMatrix().inverted() * QVector4D(p.x(), p.y(), 0.0f, 1.0f);
    return Point2Df(t.x(), t.y());
}



Point2Df Graphics2DItem::modelSpace2WorldSpace(const Point2Df &p) const
{
    //Transform the point by matrix.
    QVector4D t = _modelMatrix.topMatrix() * QVector4D(p.x(), p.y(), 0.0f, 1.0f);
    return Point2Df(t.x(), t.y());
}



AABB2D Graphics2DItem::getAABBRender(const Point2Df&) const
{
    return _aabb;
}



AABB2DItem *Graphics2DItem::getAABB2DItem()
{
    if (_aabbItem == nullptr)
    {
        //Create a new aabb item.
        _aabbItem = new AABB2DItem();
        _aabbItem->initialize();
        _aabbItem->visible(false);
    }

    if (_isAABBItemOutdated)
    {
        //Update the aabb item with the AABB points.
        _aabbItem->update(getAABBRender(_pixelSize));

        _isAABBItemOutdated = false;
    }

    return _aabbItem;
}



bool Graphics2DItem::isInsideToItemAABB(const Point2Df& p, const Point2Df& pixelSize) const
{
    Point2Df modelPoint = worldSpace2ModelSpace(p);
    return (getAABBRender(pixelSize)).isAABBInside(modelPoint);
}



Point2Df Graphics2DItem::updatePixelSize(const Point2Df& pixelSize) const
{
    return Point2Df(pixelSize.x() / getModelMatrix().sX(), pixelSize.y() / getModelMatrix().sY());
}
}
