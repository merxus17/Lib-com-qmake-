#include <string>
#include "CreatePolyline2DItemTool.h"

namespace rm
{
CreatePolyline2DItemTool::CreatePolyline2DItemTool(GraphicsView* view):
    _scene(view->getScene())
{

}



rm::Polyline2DItem* CreatePolyline2DItemTool::getPolylineItem()
{
    return _item;
}



void CreatePolyline2DItemTool::mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView*)
{
    if (!_isToolFinalized)
    {
        if(event->isLeftButtonPressed())
        {
            //Update the point position
            Point2Df p = event->getWorldPosition();
            updatePoint(p);
            const PointSet2DItem& pointSet  = _item->getPointSetItem();
            const std::vector<Point2Df>& points = pointSet.getPointSet();
            if(points.size() > 2)
            {
                //Compute the displacement vector.
                Point2Df distance = points[0] - p;

                //Get the point radius in world metrics.
                float r = pointSet.getWorldRadius();

                //Using the point set radius, test if the current point intersects with the first polyline point.
                if (distance.sqrNorm() <= r * r)
                {
                    _item->closed(true);
                    unsigned int size = pointSet.size();
                    _item->getPointSetItem().remove(size - 1);
                    _isToolFinalized = true;
                }
                else
                {
                    //Add the next preview point
                    _item->add(p);
                }
            }
            else
            {
                //Add the next preview point
                _item->add(p);
            }
        }
    }
}



void CreatePolyline2DItemTool::mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView* )
{
    if (!_isToolFinalized)
    {
        if(event->isLeftButtonPressed())
        {
            //Current mouse point.
            const Point2Df& p = event->getWorldPosition();

            //Update the current point.
            updatePoint(p);

            //If it is possible, show a preview of a closed polyline
            closePolylinePreview(event->getWorldPosition());
        }
    }
}



void CreatePolyline2DItemTool::mouseHoverEvent2D(const GraphicsSceneHoverEvent *event, GraphicsView*)
{
    if (!_isToolFinalized)
    {
        //Update the current point.
        updatePoint(event->getWorldPosition());
        if(_item->getPointSetItem().size() > 2)
        {
            //If it is possible, show a preview of a closed polyline
            closePolylinePreview(event->getWorldPosition());
        }
    }
}



void CreatePolyline2DItemTool::leaveEvent2D(const GraphicsSceneEvent*, GraphicsView*)
{
    if (!_isToolFinalized)
    {
        //If Polyline has at least 1 point without considering the preview point
        if(_item->getPointSetItem().size() > 1)
        {
            //If mouse leaves canvas, preview point stays over the last added point of the polyline
            const std::vector<Point2Df>& polylinePointSet = _item->getPointSetItem().getPointSet();
            updatePoint(polylinePointSet[_item->getPointSetItem().size() - 2]);
        }
        else
        {
            //If there is no point added at the polyline and the mouse leaves canvas, then the preview point goes to infinity
            updatePoint(Point2Df(1e8,1e8));
        }
    }
}



void CreatePolyline2DItemTool::initialize()
{
    if (_item == nullptr)
    {
        _item = new Polyline2DItem();

        //Define the line initial layout
        _item->setLineWidth(6.0f);
        _item->setBorderSize(1.0f);
        _item->setBrushColor({0.41f,0.78f,0.88f,1.0f});
        _item->setPenColor({0.0f,0.0f,0.0f,1.0f});

        //Define the pointset initial layout
        PointSet2DItem& pointSet = _item->getPointSetItem();
        pointSet.setPointSize(15.0f);
        pointSet.setBorderSize(1.0f);
        pointSet.setBrushColor({0.23f,0.73f,0.93f,1.0f});
        pointSet.setPenColor({0.0f,0.0f,0.0f,1.0f});

        _item->add(Point2Df(1e8,1e8));
    }

    if (!_scene->isItemOnScene(_item))
    {
        _scene->addItem(_item);
    }

    _scene->setFocusItem(_item);
    _isToolFinalized = false;
}



void CreatePolyline2DItemTool::finalize()
{
    //If the polyline was open, remove the preview point.
    if (!_isToolFinalized)
    {
        _item->getPointSetItem().remove(_item->getPointSetItem().size() - 1);
    }

    _item = nullptr;
    _scene->setFocusItem(nullptr);
    _isToolFinalized = false;
}



std::string CreatePolyline2DItemTool::name()
{
        return "Create Polyline 2D";
}



bool CreatePolyline2DItemTool::hasSupport2D() const
{
    return true;
}



bool CreatePolyline2DItemTool::hasSupport3D() const
{
    return false;
}



void CreatePolyline2DItemTool::updatePoint(const Point2Df& worldPos)
{
    unsigned int size = _item->getPointSetItem().size();
    _item->getPointSetItem().setPoint(size - 1, worldPos);
}



void CreatePolyline2DItemTool::closePolylinePreview(const Point2Df &p)
{
    const PointSet2DItem& pointSet  = _item->getPointSetItem();
    const std::vector<Point2Df>& points = pointSet.getPointSet();
    if(points.size() > 2)
    {
        //Compute the displacement vector.
        Point2Df distance = points[0] - p;

        //Get the point radius in world metrics.
        float r = pointSet.getWorldRadius();

        //Using the point set radius, test if the current point intersects with the first polyline point.
        if (distance.sqrNorm() <= r * r )
        {
            //Atracts the current point to the first point.
            updatePoint(points[0]);
        }
    }
}
}

