#include "CreatePointSet2DItemTool.h"
#include "../Items/PointSet2DItem.h"
#include "../Geometry/Vector2D.h"
#include "../Core/Graphics2DView.h"
#include "../Events/GraphicsSceneKeyEvent.h"
#include "../Events/GraphicsScenePressEvent.h"
#include "../Events/GraphicsSceneMoveEvent.h"

namespace rm
{
CreatePointSet2DItemTool::CreatePointSet2DItemTool(GraphicsView* view)
    :_scene(view->getScene())
{
}


PointSet2DItem* CreatePointSet2DItemTool::getPointSet2DItem()
{
    return _item;
}



void CreatePointSet2DItemTool::mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView*)
{
    const Point2Df& point = event->getWorldPosition();
    unsigned int indexPointSelected = _item->intersects(point,0,_item->size() - 1);

    //Adding point
    if(event->isLeftButtonPressed())
    {
        if(indexPointSelected == PointSet2DItem::NO_INTERSECTS)
        {
            //Update the point position
            Point2Df p = event->getWorldPosition();

            //Add the next preview point
            _item->add(p);
        }
    }
}



void CreatePointSet2DItemTool::mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView*)
{
     updatePoint(event->getWorldPosition());
}



void CreatePointSet2DItemTool::mouseHoverEvent2D(const GraphicsSceneHoverEvent* event, GraphicsView*)
{
    //Update preview point
    updatePoint(event->getWorldPosition());
}



void CreatePointSet2DItemTool::leaveEvent2D(const GraphicsSceneEvent*, GraphicsView*)
{
    //Mouse leaved canvas so preview point needs to be updated to infinity
    updatePoint(Point2Df(1e8,1e8));
}



void CreatePointSet2DItemTool::initialize()
{
    if (_item == nullptr)
    {
        _item = new PointSet2DItem();

        //Define the initial layout
        _item->setPointSize(15.0f);
        _item->setBorderSize(1.0f);
        _item->setBrushColor({0.12f,0.96f,0.62f,1.0f});
        _item->setPenColor({0.0f,0.0f,0.0f,1.0f});

        //Adding preview point at infinity
        _item->add(Point2Df(1e8,1e8));
    }

    if (!_scene->isItemOnScene(_item))
    {
        _scene->addItem(_item);
    }

    _scene->setFocusItem(_item);
}



void CreatePointSet2DItemTool::finalize()
{
    //Removes preview point
    _item->remove(_item->size() - 1);

    _item = nullptr;
    _scene->setFocusItem(nullptr);
}



std::string CreatePointSet2DItemTool::name()
{
    return "Create PointSet 2D";
}



bool CreatePointSet2DItemTool::hasSupport2D() const
{
    return true;
}



bool CreatePointSet2DItemTool::hasSupport3D() const
{
    return false;
}



void CreatePointSet2DItemTool::updatePoint(const Point2Df& worldPos)
{
    unsigned int size = static_cast<unsigned int>(_item->size());
    _item->setPoint(size - 1, worldPos);
}
}
