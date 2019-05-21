#include <string>



#include "EditPointSet2DItemTool.h"


namespace rm
{

EditPointSet2DItemTool::EditPointSet2DItemTool(GraphicsView* view, PointSet2DItem* item)
    :_item(item)
    ,_scene(view->getScene())
{

}



void EditPointSet2DItemTool::mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* )
{
    const Point2Df& point = event->getWorldPosition();
    _indexPointSelected = _item->intersects(point,0,_item->size() - 1);
    if(event->isRightButtonPressed())
    {
       //If intersects a point that is not the preview point
       if(_indexPointSelected != PointSet2DItem::NO_INTERSECTS)
       {
           //Removing points
           _item->remove(_indexPointSelected);
       }

    }
}



void EditPointSet2DItemTool::mouseHoverEvent2D(const GraphicsSceneHoverEvent* event , GraphicsView* view)
{
    Graphics2DView * v = static_cast<Graphics2DView*>(view);
    bool insideABB = _item->isInsideToItemAABB(event->getWorldPosition(),v->getPixelSize());

    if(insideABB)
    {
        unsigned int index = _item->intersects(event->getWorldPosition(),0,_item->size() - 1);

        //If mouse doesn't intersects any point of the pointset
        if(index == PointSet2DItem::NO_INTERSECTS)
        {
            updatePoint(event->getWorldPosition());
        }

        else
        {
            //Update preview point to infinity
            updatePoint(Point2Df(1e8,1e8));
        }
    }
    else
    {
        updatePoint(event->getWorldPosition());
    }

}



void EditPointSet2DItemTool::mouseReleaseEvent2D(const GraphicsScenePressEvent* event , GraphicsView*)
{
    const Point2Df& point = event->getWorldPosition();
    _indexPointSelected = _item->intersects(point,0,_item->size() - 1);

    //Adding point
    if(event->isLeftButtonPressed())
    {
        if(_indexPointSelected == PointSet2DItem::NO_INTERSECTS)
        {
            //Update the point position
            Point2Df p = event->getWorldPosition();

            //Add the next preview point
            _item->add(p);
        }
    }
}



void EditPointSet2DItemTool::mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView* )
{
    if(event->isOnlyLeftButtonPressed())
    {
        if(_indexPointSelected != PointSet2DItem::NO_INTERSECTS )
        {
            const Point2Df& point = event->getWorldPosition();

            //Moving points .
            _item->setPoint(_indexPointSelected, point);
            updatePoint(event->getWorldPosition());
        }
        else
        {
            updatePoint(event->getWorldPosition());
        }
    }
    else
    {
        updatePoint(event->getWorldPosition());
    }
}



void EditPointSet2DItemTool::leaveEvent2D(const GraphicsSceneEvent*, GraphicsView*)
{
     updatePoint(Point2Df(1e8,1e8));
}



void EditPointSet2DItemTool::initialize()
{
    _scene->setFocusItem(_item);
    _item->add(Point2Df(1e8,1e8));
}



void EditPointSet2DItemTool::finalize()
{
    _item->remove(_item->size() - 1);
    _scene->setFocusItem(nullptr);
}



std::string EditPointSet2DItemTool::name()
{
    return "Edit PointSet 2D";
}



bool EditPointSet2DItemTool::hasSupport2D() const
{
    return true;
}



bool EditPointSet2DItemTool::hasSupport3D() const
{
    return false;
}

PointSet2DItem* EditPointSet2DItemTool::getPointSet2DItem()
{
    return _item;
}



void EditPointSet2DItemTool::updatePoint(const Point2Df& worldPos)
{
    _item->setPoint(_item->size() - 1, worldPos);
}
}
