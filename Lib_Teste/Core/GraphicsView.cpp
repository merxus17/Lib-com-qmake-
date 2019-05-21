#include <iostream>
#include "GraphicsView.h"
#include "GraphicsItem.h"
#include "../Items/RectangleZoomItem.h"
#include "../Geometry/Vector2D.h"
#include "../Events/GraphicsSceneMoveEvent.h"
#include "../Events/GraphicsSceneKeyEvent.h"
#include "../Events/GraphicsSceneWheelEvent.h"
#include <QMenu>
#include <QMouseEvent>

namespace rm
{
int GraphicsView::_globalViewId = 0;

GraphicsView::GraphicsView(GraphicsScene* scene, QWidget *parent, ViewType type, bool preview)
    :QOpenGLWidget(parent)
    ,_scene(scene)
    ,_preview(preview)
    , _type(type)
{
    setAutoFillBackground(false);
    setFocusPolicy(Qt::StrongFocus);
    _viewId = _globalViewId;
    _globalViewId++;
    setMouseTracking(true);
    _rectangleZoom.visible(false);
}



GraphicsView::~GraphicsView()
{
    _scene->makeCurrent();
    _scene->removeView(this);
}



void GraphicsView::mouseMoveEvent(QMouseEvent* event)
{    
    if (!isPreview())
    {
        //Mouse hover.
        if (event->buttons() == Qt::MouseButton::NoButton && _scene->isEventEnabled(EventType::HoverEvent))
        {
            //Set the global mouse position.
            _mouseHover.setGlobalPosition( Point2Df(static_cast<float>(event->globalPos().x()),
                                                  static_cast<float>(event->globalPos().y())) );

            //Set the local mouse position.
            _mouseHover.setLocalPosition( Point2Df(static_cast<float>(event->localPos().x()),
                                                 static_cast<float>(event->localPos().y())) );

            //Set the screen mouse position.
            _mouseHover.setScreenPosition( Point2Df(static_cast<float>(event->screenPos().x()),
                                                 static_cast<float>(event->screenPos().y())) );

            //Set the dialog mouse position.
            _mouseHover.setWindowPosition( Point2Df(static_cast<float>(event->windowPos().x()),
                                                 static_cast<float>(event->windowPos().y())) );

            //Set old world coordinate.
            _mouseHover.setOldWorldPosition(_mouseHover.getWorldPosition());

            //Convert to world coordinate. This is the current world coordinate right now.
            const Point2Df& localPosition = _mouseHover.getLocalPosition();
            _mouseHover.setWorldPosition( convertFromScreenToWorld(localPosition) );

            //Set key modifiers.
            _mouseHover.setKeyboardModifiers(_mouseHover.convertKeyboardModifiers(static_cast<unsigned int>(event->modifiers())));

            if (!_scene->isToolStackEmpty())
            {
                makeCurrent();
                if (_type == ViewType::VIEW_2D && _scene->topTool()->hasSupport2D())
                {
                    _scene->topTool()->mouseHoverEvent2D(&_mouseHover, this);
                }
                else if (_scene->topTool()->hasSupport3D())
                {
                    _scene->topTool()->mouseHoverEvent3D(&_mouseHover, this);
                }
                doneCurrent();
            }
        }
        //Mouse move.
        else if ( _scene->isEventEnabled(EventType::MoveEvent))
        {
            //Set the global mouse position.
            _mouseMove.setGlobalPosition( Point2Df(static_cast<float>(event->globalPos().x()),
                                                  static_cast<float>(event->globalPos().y())) );

            //Set the local mouse position.
            _mouseMove.setLocalPosition( Point2Df(static_cast<float>(event->localPos().x()),
                                                 static_cast<float>(event->localPos().y())) );

            //Set the screen mouse position.
            _mouseMove.setScreenPosition( Point2Df(static_cast<float>(event->screenPos().x()),
                                                 static_cast<float>(event->screenPos().y())) );

            //Set the dialog mouse position.
            _mouseMove.setWindowPosition( Point2Df(static_cast<float>(event->windowPos().x()),
                                                 static_cast<float>(event->windowPos().y())) );

            //Set old world coordinate.
            _mouseMove.setOldWorldPosition(_mouseMove.getWorldPosition());

            //Convert to world coordinate. This is the current world coordinate right now.
            const Point2Df& localPosition = _mouseMove.getLocalPosition();
            _mouseMove.setWorldPosition( convertFromScreenToWorld(localPosition) );

            //Set the pressed buttons.
            _mouseMove.setButtons( _mouseMove.convertMouseButtons(static_cast<unsigned int>(event->buttons())));

            //Set key modifiers.
            _mouseMove.setKeyboardModifiers( _mouseMove.convertKeyboardModifiers(static_cast<unsigned int>(event->modifiers())));


            if (!_scene->isToolStackEmpty())
            {
                makeCurrent();
                if (_type == ViewType::VIEW_2D && _scene->topTool()->hasSupport2D())
                {
                    _scene->topTool()->mouseMoveEvent2D(&_mouseMove, this);
                }
                else if (_scene->topTool()->hasSupport3D())
                {
                    _scene->topTool()->mouseMoveEvent3D(&_mouseMove, this);
                }
                doneCurrent();
            }
        }
        update();
        emit signalUpdate();
    }
}



void GraphicsView::mousePressEvent(QMouseEvent* event)
{
    if(!isPreview() && _scene->isEventEnabled(EventType::PressEvent))
    {
        //Set mouse button action.
        _mousePress.setMouseButtonAction(MouseButtonAction::Press);

        //Set the global mouse position.
        _mousePress.setGlobalPosition( Point2Df(static_cast<float>(event->globalPos().x()),
                                              static_cast<float>(event->globalPos().y())) );

        //Set the local mouse position.
        _mousePress.setLocalPosition( Point2Df(static_cast<float>(event->localPos().x()),
                                             static_cast<float>(event->localPos().y())) );

        //Set the screen mouse position.
        _mousePress.setScreenPosition( Point2Df(static_cast<float>(event->screenPos().x()),
                                             static_cast<float>(event->screenPos().y())) );

        //Set the dialog mouse position.
        _mousePress.setWindowPosition( Point2Df(static_cast<float>(event->windowPos().x()),
                                             static_cast<float>(event->windowPos().y())) );

        //Convert to world coordinate. This is the current world coordinate right now.
        const Point2Df& localPosition = _mousePress.getLocalPosition();
        _mousePress.setWorldPosition( convertFromScreenToWorld(localPosition) );

        //Set the pressed buttons.
        _mousePress.setButton(static_cast<MouseButton>(event->button()));

        //Get key modifiers.
        _mousePress.setKeyboardModifiers( _mousePress.convertKeyboardModifiers(static_cast<unsigned int>(event->modifiers())));

        //Initializing move event data
        //Set old world coordinate.
        _mouseMove.setFirstWorldPosition(_mousePress.getWorldPosition());
        _mouseMove.setOldWorldPosition( _mouseMove.getFirstWorldPosition() );
        _mouseMove.setWorldPosition( _mouseMove.getFirstWorldPosition() );

        if (!_scene->isToolStackEmpty())
        {
            makeCurrent();
            if (_type == ViewType::VIEW_2D && _scene->topTool()->hasSupport2D())
            {
                _scene->topTool()->mousePressEvent2D(&_mousePress, this);
            }
            else if (_scene->topTool()->hasSupport3D())
            {
                _scene->topTool()->mousePressEvent3D(&_mousePress, this);
            }
            doneCurrent();
        }

        update();
        emit signalUpdate();
    }
}



void GraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if(!isPreview() && _scene->isEventEnabled(EventType::PressEvent))
    {
        //Set mouse button action.
        _mousePress.setMouseButtonAction(MouseButtonAction::Release);

        //Set the global mouse position.
        _mousePress.setGlobalPosition( Point2Df(static_cast<float>(event->globalPos().x()),
                                              static_cast<float>(event->globalPos().y())) );

        //Set the local mouse position.
        _mousePress.setLocalPosition( Point2Df(static_cast<float>(event->localPos().x()),
                                             static_cast<float>(event->localPos().y())) );

        //Set the screen mouse position.
        _mousePress.setScreenPosition( Point2Df(static_cast<float>(event->screenPos().x()),
                                             static_cast<float>(event->screenPos().y())) );

        //Set the dialog mouse position.
        _mousePress.setWindowPosition( Point2Df(static_cast<float>(event->windowPos().x()),
                                             static_cast<float>(event->windowPos().y())) );

        //Convert to world coordinate. This is the current world coordinate right now.
        const Point2Df& localPosition = _mousePress.getLocalPosition();
        _mousePress.setWorldPosition( convertFromScreenToWorld(localPosition) );

        //Set the pressed buttons.
        _mousePress.setButton(static_cast<MouseButton>(event->button()));

        //Get key modifiers.
        _mousePress.setKeyboardModifiers( _mousePress.convertKeyboardModifiers(static_cast<unsigned int>(event->modifiers())) );

        if (!_scene->isToolStackEmpty())
        {
            makeCurrent();
            if (_type == ViewType::VIEW_2D && _scene->topTool()->hasSupport2D())
            {
                _scene->topTool()->mouseReleaseEvent2D(&_mousePress, this);
            }
            else if (_scene->topTool()->hasSupport3D())
            {
                _scene->topTool()->mouseReleaseEvent3D(&_mousePress, this);
            }
            doneCurrent();
        }
        update();
        emit signalUpdate();
    }
}



void GraphicsView::keyPressEvent(QKeyEvent* event)
{

    if(!isPreview() && _scene->isEventEnabled(EventType::keyEvent))
    {

        //Set key event type
        _keyEvent.setKeyEventType(KeyAction::pressEvent);

        //Set auto representation
        _keyEvent.setAutoRep(event->isAutoRepeat());

        //Set text
        _keyEvent.setText(event->text().toStdString());

        //Set count
        _keyEvent.setCount(event->count());

        //Set key modifiers
        _keyEvent.setKeyboardModifiers(_keyEvent.convertKeyboardModifiers(static_cast<unsigned int>(event->modifiers())));

        //Set key
        _keyEvent.setKey(event->key());

        if (!_scene->isToolStackEmpty())
        {
            makeCurrent();
            if (_type == ViewType::VIEW_2D && _scene->topTool()->hasSupport2D())
            {
                _scene->topTool()->keyPressEvent2D(&_keyEvent, this);
            }
            else if (_scene->topTool()->hasSupport3D())
            {
                _scene->topTool()->keyPressEvent3D(&_keyEvent, this);
            }
            doneCurrent();
        }
        update();
        emit signalUpdate();
    }
}



void GraphicsView::keyReleaseEvent(QKeyEvent* event)
{
    if(!isPreview() && _scene->isEventEnabled(EventType::keyEvent))
    {
        //Set key event type
        _keyEvent.setKeyEventType(KeyAction::releaseEvent);

        //Set auto representation
        _keyEvent.setAutoRep(event->isAutoRepeat());

        //Set text
        _keyEvent.setText(event->text().toStdString());

        //Set count
        _keyEvent.setCount(event->count());

        //Set key modifiers
        _keyEvent.setKeyboardModifiers(_keyEvent.convertKeyboardModifiers(static_cast<unsigned int>(event->modifiers())));

        //Set key
        _keyEvent.setKey(event->key());

        update();
        emit signalUpdate();
    }
}



void GraphicsView::contextMenuEvent(QContextMenuEvent* )
{
}



void GraphicsView::dragEnterEvent(QDragEnterEvent* )
{

}



void GraphicsView::dragLeaveEvent(QDragLeaveEvent* )
{
}




void GraphicsView::dragMoveEvent(QDragMoveEvent* )
{
}



void GraphicsView::dropEvent(QDropEvent* )
{
}



void GraphicsView::focusInEvent(QFocusEvent* )
{
}



bool GraphicsView::focusNextPrevChild(bool )
{
	return false;
}



void GraphicsView::focusOutEvent(QFocusEvent* )
{
}



void GraphicsView::inputMethodEvent(QInputMethodEvent* )
{
}



void GraphicsView::mouseDoubleClickEvent(QMouseEvent* )
{
}



void GraphicsView::scrollContentsBy(int , int )
{
}



void GraphicsView::showEvent(QShowEvent* )
{
}



//void GraphicsView::changeEvent(QChangeEvent* )
//{
//}



//void GraphicsView::closeEvent(CloseEvent* )
//{
//}



void GraphicsView::enterEvent(QEvent* )
{
    if(!isPreview() && _scene->isEventEnabled(EventType::EnterEvent))
    {
        if (!_scene->isToolStackEmpty())
        {
            makeCurrent();
            if (_type == ViewType::VIEW_2D && _scene->topTool()->hasSupport2D())
            {
                _scene->topTool()->enterEvent2D(&_enterEvent, this);
            }
            else if (_scene->topTool()->hasSupport3D())
            {
                _scene->topTool()->enterEvent3D(&_enterEvent, this);
            }
            doneCurrent();
        }
        update();
        emit signalUpdate();
    }
}



void GraphicsView::leaveEvent(QEvent* )
{
    if(!isPreview() && _scene->isEventEnabled(EventType::LeaveEvent))
    {
        if (!_scene->isToolStackEmpty())
        {
            makeCurrent();
            if (_type == ViewType::VIEW_2D && _scene->topTool()->hasSupport2D())
            {
                _scene->topTool()->leaveEvent2D(&_leaveEvent, this);
            }
            else if (_scene->topTool()->hasSupport3D())
            {
                _scene->topTool()->leaveEvent3D(&_leaveEvent, this);
            }
            doneCurrent();
        }

        update();
        emit signalUpdate();
    }
}



void GraphicsView::hideEvent(QHideEvent * )
{
}



//void GraphicsView::viewportEvent(QEvent* )
//{
//}



void GraphicsView::wheelEvent(QWheelEvent* event)
{
     if(!isPreview() && _scene->isEventEnabled(EventType::WheelEvent))
     {
         //Set the mouse position.
         _wheelEvent.setPos(Point2Df(static_cast<float>(event->pos().x()),
                                     static_cast<float>(event->pos().y())));

         //Set the global mouse position.
         _wheelEvent.setGlobalPos( Point2Df(static_cast<float>(event->globalPos().x()),
                                               static_cast<float>(event->globalPos().y())) );

         //Set delta value.
         _wheelEvent.setDelta(event->delta());

         if(_wheelEvent.getDelta() > 0)
         {
             _wheelEvent.setOrientation(Orientation::Foward);
         }
         else
         {
             _wheelEvent.setOrientation(Orientation::Backward);
         }

         //Set the pressed buttons.
         _wheelEvent.setButtons( _wheelEvent.convertMouseButtons(static_cast<unsigned int>(event->buttons())) );

         //Set key modifiers.
         _wheelEvent.setKeyboardModifiers( _wheelEvent.convertKeyboardModifiers(static_cast<unsigned int>(event->modifiers())) );

         if (!_scene->isToolStackEmpty())
         {
             makeCurrent();
             if (_type == ViewType::VIEW_2D && _scene->topTool()->hasSupport2D())
             {
                 _scene->topTool()->wheelEvent2D(&_wheelEvent, this);
             }
             else if (_scene->topTool()->hasSupport3D())
             {
                 _scene->topTool()->wheelEvent3D(&_wheelEvent, this);
             }
             doneCurrent();
         }

         update();
         emit signalUpdate();
     }
}



bool GraphicsView::isPreview() const
{
    return _preview;
}



void GraphicsView::preview(bool preview)
{
    _preview = preview;
}



void GraphicsView::fit()
{
}



void GraphicsView::fitIn(Point3D& , Point3D& )
{
}



RectangleZoomItem& GraphicsView::getRectangleZoomItem()
{
    return _rectangleZoom;
}



const RectangleZoomItem& GraphicsView::getRectangleZoomItem() const
{
    return _rectangleZoom;
}



void GraphicsView::viewFrom()
{
}



void GraphicsView::setScene(GraphicsScene& scene)
{
    this->_scene = &scene;
}



GraphicsScene* GraphicsView::getScene()
{
    return this->_scene;
}



int GraphicsView::id()
{
    return _viewId;
}



int GraphicsView::numberOfViews()
{
    return _globalViewId;
}



void GraphicsView::onUpdate()
{
    update();
}



void GraphicsView::zoomInOut(float )
{

}
}
