#include "CreateRectangle2DItemTool.h"
#include "../Items/Rectangle2DItem.h"
#include "../Core/Graphics2DView.h"
#include "../Events/GraphicsSceneMoveEvent.h"
#include "../Geometry/Vector2D.h"

namespace rm
{
CreateRectangle2DItemTool::CreateRectangle2DItemTool(GraphicsView* view):
    _scene(view->getScene())
{

}



void CreateRectangle2DItemTool::mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView*)
{
    if (!_isToolFinalized)
    {
        if(event->isLeftButtonPressed())
        {
            // Initialize the preview corner of the creation tool with valid values.
            _previewCorner = event->getWorldPosition();
        }
    }
}



void CreateRectangle2DItemTool::mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView*)
{
    if (!_isToolFinalized)
    {
        if(event->isLeftButtonPressed())
        {
            //Current mouse point.
            const Point2Df& p = event->getWorldPosition();

            //Update the rectangle.
            updateRectangle(p);
        }
    }
}



void CreateRectangle2DItemTool::mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView*)
{
    if (!_isToolFinalized)
    {
        if(event->isLeftButtonPressed())
        {
            // Get the world position of the mouse pointer.
            Point2Df p = event->getWorldPosition();

            //Update the rectangle position
            updateRectangle(p);

            // Finilize the usage of the tool.
            _isToolFinalized = true;
        }
    }
}



void CreateRectangle2DItemTool::initialize()
{
    if (_item == nullptr)
    {
        _item = new Rectangle2DItem();
        _item->setPenColor({0.98f,0.91f,0.15f,1.0f});  //yellow
        _item->setBrushColor({0.98f,0.91f,0.15f,0.5}); //semitransparent yellow

        _item->setBorderSize(4);
    }

    if (!_scene->isItemOnScene(_item))
    {
        _scene->addItem(_item);
    }

    _scene->setFocusItem(_item);
    _isToolFinalized = false;
}



void CreateRectangle2DItemTool::finalize()
{
    _scene->setFocusItem(nullptr);
    _item = nullptr;
    _isToolFinalized = true;
}



std::string CreateRectangle2DItemTool::name()
{
    return "Create Rectangle 2D";
}



Rectangle2DItem* CreateRectangle2DItemTool::getRectangle2DItem()
{
    return _item;
}



bool CreateRectangle2DItemTool::hasSupport2D() const
{
    return true;
}



bool CreateRectangle2DItemTool::hasSupport3D() const
{
    return false;
}



void CreateRectangle2DItemTool::updateRectangle(const Point2Df& worldPos)
{
    // Compute the new width and height of the rectangle.
    Vector2D<float> newSize = worldPos - _previewCorner;

    // Compute the new center point.
    Point2Df newCenter = _previewCorner + (newSize * 0.5f);

    // Update the rectangle width, height and center point.
    _item->updateRectangle(newCenter, newSize.x(), newSize.y());
}
}

