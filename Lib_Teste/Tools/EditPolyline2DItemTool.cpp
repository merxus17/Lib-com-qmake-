#include <string>
#include "../Items/Polyline2DItem.h"
#include "../Core/GraphicsView.h"
#include "../Events/GraphicsScenePressEvent.h"


#include "EditPolyline2DItemTool.h"

namespace rm
{
EditPolyline2DItemTool::EditPolyline2DItemTool(GraphicsView* view, Polyline2DItem* item)
    :_item(item)
    ,_scene(view->getScene())
{
}



EditPolyline2DItemTool::~EditPolyline2DItemTool()
{
}



void EditPolyline2DItemTool::mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* )
{
    //Get the current mouse coordinates in world space.
    const Point2Df& p = event->getWorldPosition();

    if (event->isLeftButtonPressed())
    {
        //Test intersection with all points.
         _indexPointSelected = _item->getPointSetItem().intersects(p);

         //If do not intersect with the points, but intersect with a segment, the point must be inserted over that
         //segment.
        if(_indexPointSelected == PointSet2DItem::NO_INTERSECTS)
        {
            //Test intersetion the polyline segments.
            _indexPointSelected = _item->computeSegmentIntersection(p);
            if (_indexPointSelected != Polyline2DItem::NO_INTERSECTS)
            {
                //Add a point over segment.
                _item->addInPlace(p, _indexPointSelected);

                //Update the current selected point.
                _indexPointSelected++;
            }
        }
    }
}



void EditPolyline2DItemTool::mouseHoverEvent2D(const GraphicsSceneHoverEvent* event, GraphicsView*)
{
    const Point2Df& p = event->getWorldPosition();

    //Compute segment intersection.
    unsigned int segment = _item->computeSegmentIntersection(p);

    //If it intersects the polyline, but do not intersects any point, so it intersects a polyline segment.
    if (segment != Polyline2DItem::NO_INTERSECTS
        && _item->getPointSetItem().intersects(p) == PointSet2DItem::NO_INTERSECTS)
    {
        //TODO: MELHORAR ESSAS TRANSFORMACOES.

        //Get the segment points.
        const std::vector<Point2Df>& pointSet = _item->getPointSetItem().getPointSet();
        const Point2Df& p1 = pointSet[segment];
        const Point2Df& p2 = pointSet[(segment + 1) % pointSet.size()];

        //Get the point in world coordinates.
        const OpenGLMatrix& model = _item->getModelMatrix();
        QMatrix4x4 inverse = model.topMatrix().inverted();
        QVector4D worldPoint = inverse * QVector4D(p.x(), p.y(), 0.0f, 1.0f);
        Point2Df point(worldPoint.x(), worldPoint.y());

        //Project point over segment.
        Point2Df projectedPoint = Point2Df::projectPointOnSegment(point, p1, p2);

        //Compute the transformed point back to model coordinates.
        QVector4D modelPoint = model.topMatrix() * QVector4D(projectedPoint.x(), projectedPoint.y(), 0.0f, 1.0f);

        //Define the new point coordinates and show preview point.
        _item->setPreviewPoint( Point2Df(modelPoint.x(), modelPoint.y()));
        _item->setPreviewVisibility(true);
    }
    else
    {
        _item->setPreviewVisibility(false);
    }
}



void EditPolyline2DItemTool::mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView* )
{
    //Get the current mouse coordinates in world space.
    const Point2Df& p = event->getWorldPosition();

    //Try to delete a point.
    if(event->isRightButtonPressed())
    {
        //Test intersetion with the point set.
        _indexPointSelected = _item->getPointSetItem().intersects(p);
        if(_indexPointSelected != PointSet2DItem::NO_INTERSECTS)
        {
            //Remove the intersected point.
            _item->getPointSetItem().remove(_indexPointSelected);
        }
    }
    //Try to close the polyline.
    else if (event->isLeftButtonPressed() && !_item->isClosed())
    {
        //Get point set information;
        const PointSet2DItem& pointSet  = _item->getPointSetItem();
        const std::vector<Point2Df>& points = pointSet.getPointSet();

        //Verify if the grabbed point is the firt or the last. In this case, try to close the polyline.
        if(points.size() > 2 && (_indexPointSelected == points.size() - 1 || _indexPointSelected == 0))
        {
            //Get the point be move the current point to it.
            Point2Df attractor = (_indexPointSelected == 0 ? points.back() : points.front());

            //Get the point in world coordinates.
            const QMatrix4x4& m =_item->getModelMatrix().topMatrix();
            QVector4D t = m * QVector4D(attractor.x(), attractor.y(), 0.0f, 1.0f);
            attractor = Point2Df(t.x(), t.y());

            //Current mouse point.
            const Point2Df& p = event->getWorldPosition();

            //Compute the displacement vector.
            Point2Df distance = attractor - p;

            //Get the point radius in world metrics.
            float r = pointSet.getWorldRadius();

            //Using the point set radius, test if the current point intersects with the first polyline point.
            if (distance.sqrNorm() <= r * r )
            {
                _item->closed(true);
                _item->getPointSetItem().remove(_indexPointSelected);
            }
        }
    }

    _indexPointSelected = Polyline2DItem::NO_INTERSECTS;
}



void EditPolyline2DItemTool::mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView* )
{
    //The preview point is visible just on hover event. When moving, a mouse button is pressed, so the preview point is
    //set as invisible.
    _item->setPreviewVisibility(false);

    //Get the current mouse position
    const Point2Df& p = event->getWorldPosition();
    if (event->isOnlyLeftButtonPressed())
    {
        //Get point set information;
        const PointSet2DItem& pointSet  = _item->getPointSetItem();
        const std::vector<Point2Df>& points = pointSet.getPointSet();

        if(_indexPointSelected != Polyline2DItem::NO_INTERSECTS)
        {
            //Update the point by the current mouse coordinates.
            _item->getPointSetItem().setPoint(_indexPointSelected, p);

            //Try to close the polyline
            if (!_item->isClosed())
            {
                //Verify if the grabbed point is the firt or the last. In this case, try to close the polyline.
                if (pointSet.size() > 2  && (_indexPointSelected == 0 || _indexPointSelected == pointSet.size() - 1 ))
                {
                    //Get the point to be move the current point to it.
                    Point2Df attractor = (_indexPointSelected == 0 ? points.back() : points.front());

                    //Get the point in world coordinates.
                    const QMatrix4x4& m =_item->getModelMatrix().topMatrix();
                    QVector4D t = m * QVector4D(attractor.x(), attractor.y(), 0.0f, 1.0f);
                    attractor = Point2Df(t.x(), t.y());

                    //Compute the displacement vector.
                    Point2Df distance = attractor - p;

                    //Get the point radius in world metrics.
                    float r = pointSet.getWorldRadius();

                    //Using the point set radius, test if the current point intersects with the first polyline point.
                    if (distance.sqrNorm() <= r * r )
                    {
                        //Atracts the current point to the first point.
                        _item->getPointSetItem().setPoint(_indexPointSelected, attractor);
                    }
                }
            }
        }
    }
}



void EditPolyline2DItemTool::initialize()
{
    _scene->setFocusItem(_item);
    _item->setPreviewVisibility(false);
}



void EditPolyline2DItemTool::finalize()
{
    _scene->setFocusItem(nullptr);
    _item->setPreviewVisibility(false);
}



std::string EditPolyline2DItemTool::name()
{
    return "Edit Polyline 2D";
}



bool EditPolyline2DItemTool::hasSupport2D() const
{
    return true;
}



bool EditPolyline2DItemTool::hasSupport3D() const
{
    return false;
}

Polyline2DItem* EditPolyline2DItemTool::getPolyline2DItem()
{
    return _item;
}
}
