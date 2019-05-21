#include "AABB2DItem.h"
#include "../../Items/Polyline2DItem.h"

namespace rm
{
AABB2DItem::AABB2DItem()
{
    _polyline = new Polyline2DItem();
    _polyline->closed(true);
}



PointSet2DItem &AABB2DItem::getPointSetItem()
{
    return  _polyline->getPointSetItem();
}



const PointSet2DItem &AABB2DItem::getPointSetItem() const
{
    return  _polyline->getPointSetItem();
}



PointSet2DItem &AABB2DItem::getPreviewPointSetItem()
{
    return _polyline->getPreviewPointSetItem();
}



void AABB2DItem::initialize()
{
    _polyline->initialize();
}



bool AABB2DItem::isInsideToItemAABB(const Point2Df &p, const Point2Df &pixelSize) const
{
    return _polyline->isInsideToItemAABB(p, pixelSize);
}



bool AABB2DItem::isVisible() const
{
    return _polyline->isVisible();
}



void AABB2DItem::render(int viewId, const OpenGLMatrix &projection, const OpenGLMatrix &model, const Point2Df &pixelSize)
{
    _polyline->setProjectionMatrix(projection);
    _polyline->setModelMatrix(model);
    _polyline->setPixelSize(pixelSize);
    _polyline->render(viewId);
}



const PointSet2DItem &AABB2DItem::getPreviewPointSetItem() const
{
    return _polyline->getPreviewPointSetItem();
}



void AABB2DItem::setBrushColor(const QVector4D &color)
{
    _polyline->setBrushColor(color);
}



void AABB2DItem::setLineWidth(float width)
{
    _polyline->setLineWidth(width);
}



void AABB2DItem::setPenColor(const QVector4D &color)
{
    _polyline->setPenColor(color);
}



void AABB2DItem::update(const AABB2D& aabb)
{
    Point2Df maxCorner = aabb.getMaxCornerPoint();
    Point2Df minCorner = aabb.getMinCornerPoint();

    //Compute new selected box points.
    Point2Df topLeft(minCorner.x(), maxCorner.y());
    Point2Df bottonRight(maxCorner.x(), minCorner.y());
    Point2Df topMid((maxCorner.x() + minCorner.x())/2, maxCorner.y());
    Point2Df botMid(topMid.x(), minCorner.y());
    Point2Df leftMid(maxCorner.x(), (maxCorner.y() + minCorner.y())/2);
    Point2Df rightMid(minCorner.x(), leftMid.y());

    //Points order pattern.
    //6-------7-------0
    //|               |
    //|               |
    //|               |
    //5               1
    //|               |
    //|               |
    //|               |
    //4-------3-------2
    std::vector<Point2Df> selBoxPoints
    {
        maxCorner,
        leftMid,
        bottonRight,
        botMid,
        minCorner,
        rightMid,
        topLeft,
        topMid
    };

    //Define the new points on polyline.
    getPointSetItem().setNewPointSet(selBoxPoints);
}



void AABB2DItem::visible(bool v)
{
    _polyline->visible(v);
}



AABB2DItem::~AABB2DItem()
{
    delete _polyline;
}



void AABB2DItem::showHandles(bool show)
{
    getPointSetItem().visible(show);
}
}
