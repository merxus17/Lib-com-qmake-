#include "ViewControllerTool.h"
#include "../Events/GraphicsSceneMoveEvent.h"
#include "../Events/GraphicsScenePressEvent.h"
#include "../Events/GraphicsSceneWheelEvent.h"
#include "../Core/GraphicsView.h"
#include "../Geometry/AxisAligmentBoundingBox.h"
#include "../Items/TriangleMesh2DItem.h"


namespace rm
{
ViewControllerTool::ViewControllerTool(GraphicsScene* scene) : GraphicsTool(),
    _scene(scene)
{


}



ViewControllerTool::~ViewControllerTool()
{

}



void ViewControllerTool::mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view )
{
    if(event->isLeftButtonPressed())
    {
        _isLeftMouseButtonPressed = true;
    }
    else if (event->isRightButtonPressed())
    {
        _isRightMousePressed = true;
        RectangleZoomItem& rectangleZoom = view->getRectangleZoomItem();

        rectangleZoom.setFirstPoint(event->getLocalPosition());
        rectangleZoom.setSecondPoint(event->getLocalPosition());
        rectangleZoom.visible(true);
    }
}



void ViewControllerTool::mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView* view)
{
    if(_isLeftMouseButtonPressed)
    {
        view->pan(event->getFirstWorldPosition(), event->getWorldPosition());
    }
    else if (_isRightMousePressed)
    {
        RectangleZoomItem& rectangleZoom = view->getRectangleZoomItem();

        rectangleZoom.setCanvasWidth(view->width());
        rectangleZoom.setCanvasHeight(view->height());
        rectangleZoom.setSecondPoint(event->getLocalPosition());
    }
}



void ViewControllerTool::mouseReleaseEvent2D(const GraphicsScenePressEvent* , GraphicsView* view)
{
    _isLeftMouseButtonPressed = false;
    if (_isRightMousePressed)
    {
        RectangleZoomItem& rectangleZoom = view->getRectangleZoomItem();
        const Point2Df& p1 = rectangleZoom.getFirstPoint();
        const Point2Df& p2 = rectangleZoom.getSecondPoint();
        rectangleZoom.visible(false);

        view->zoomArea(view->convertFromScreenToWorld(p1), view->convertFromScreenToWorld(p2));

        _isRightMousePressed = false;
    }
}



void ViewControllerTool::mousePressEvent3D(const GraphicsScenePressEvent* event, GraphicsView* view )
{
    if (event->isRightButtonPressed())
    {
        _isRightMousePressed = true;

        RectangleZoomItem& rectangleZoom = view->getRectangleZoomItem();
        rectangleZoom.setFirstPoint(event->getLocalPosition());
        rectangleZoom.setSecondPoint(event->getLocalPosition());
        rectangleZoom.visible(true);
    }
    else if (event->isLeftButtonPressed())
    {
        _isLeftMouseButtonPressed = true;

        Graphics3DView* view3D =  dynamic_cast<Graphics3DView*>(view);
        view3D->arcballInit();

        _lastPosition =  event->getLocalPosition();
        _lastPosition[1] = view->height() - 1 - _lastPosition[1];
    }
}



void ViewControllerTool::mouseMoveEvent3D(const GraphicsSceneMoveEvent* event,GraphicsView* view)
{
    if (_isRightMousePressed)
    {

        RectangleZoomItem& rectangleZoom = view->getRectangleZoomItem();

        rectangleZoom.setCanvasWidth(view->width());
        rectangleZoom.setCanvasHeight(view->height());
        rectangleZoom.setSecondPoint(event->getLocalPosition());
    }

    if (_isLeftMouseButtonPressed)
    {
       Point2Df p1 = _lastPosition;
       Point2Df p2 = event->getLocalPosition();
       p2[1] = view->height() - 1 - p2[1];

       if (p1 != p2)
       {
           Graphics3DView* view3D =  dynamic_cast<Graphics3DView*>(view);
           view3D->arcballRotationQuat(p1, p2);
       }
       _lastPosition = p2;
    }
}



void ViewControllerTool::mouseReleaseEvent3D(const GraphicsScenePressEvent* , GraphicsView* view)
{
    _isLeftMouseButtonPressed = false;

    if (_isRightMousePressed)
    {
        RectangleZoomItem& rectangleZoom = view->getRectangleZoomItem();

        const Point2Df& p1 = rectangleZoom.getFirstPoint();
        const Point2Df& p2 = rectangleZoom.getSecondPoint();
        rectangleZoom.visible(false);
        view->zoomArea(p1, p2);

        _isRightMousePressed = false;
    }
}



void ViewControllerTool::wheelEvent2D(const GraphicsSceneWheelEvent* event , GraphicsView* view )
{
    if(event->getDelta() > 0)
    {
        view->zoomInOut(0.95f);
    }
    else
    {
        view->zoomInOut(1.05f);
    }
}



void ViewControllerTool::wheelEvent3D(const GraphicsSceneWheelEvent* event , GraphicsView* view)
{
    const AABB3D& aabb = view->getScene()->computeAABB3D();
    Graphics3DView *view3D = dynamic_cast<Graphics3DView*>(view);
    if(event->getDelta() > 0)
    {
        view3D->zoomInOut(0.05f,aabb);
    }
    else
    {
        view3D->zoomInOut(-0.05f,aabb);
    }
}



void ViewControllerTool::keyPressEvent2D(const GraphicsSceneKeyEvent* event, GraphicsView* view)
{
    if(event->getKey() == Qt::Key_Space)
    {
        view->fit();
    }
}



void ViewControllerTool::keyPressEvent3D(const GraphicsSceneKeyEvent* event,GraphicsView* view)
{
    if(event->getKey() == Qt::Key_Space)
    {
        view->fit();
    }
}



std::string ViewControllerTool::name()
{
    return "View Controller";
}



bool ViewControllerTool::hasSupport2D() const
{
    return true;
}



bool ViewControllerTool::hasSupport3D() const
{
    return true;
}
}
