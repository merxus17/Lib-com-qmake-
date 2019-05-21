#include "GraphicsScene.h"
#include "GraphicsItem.h"
#include "Graphics3DItem.h"
#include "Graphics2DView.h"
#include "Graphics3DView.h"
#include "../Items/SelectionGroup2DItem.h"
#include "../Items/PointSet2DItem.h"
#include "../Items/Polyline2DItem.h"
#include "../Events/GraphicsSceneKeyEvent.h"
#include "../Events/GraphicsSceneMoveEvent.h"
#include "../Events/GraphicsScenePressEvent.h"
#include "../Events/GraphicsSceneWheelEvent.h"
#include <iostream>
#include <algorithm>
#include <iterator>

namespace rm
{
GraphicsScene::GraphicsScene() :
    _mouseGrabberItem(nullptr),
    _itemInFocus(nullptr),
    _enableEvents(static_cast<SceneEvents>(EventType::NoEvent))
{
    //Create a surface.
    _surface.create();

    if (!_surface.isValid())
    {
        printf("Surface is not valid\n");
    }

    _glContext.setShareContext(QOpenGLContext::globalShareContext());

    //Create a context.
    if(!_glContext.create())
    {
        printf("Context was not created.\n");
    }

    if (!QOpenGLContext::areSharing(&_glContext, QOpenGLContext::globalShareContext()))
    {
        printf("Scene doesn't share with global\n");
    }

    //Create default shading model
    createShadingModel();
}



GraphicsScene::~GraphicsScene()
{
    makeCurrent();

    //Delete all tools in the stack. As tools can contain items, they must be deleted before the items.
    while(!_toolStack.empty())
    {
        //Finalize the top tool.
        _toolStack.top()->finalize();

        //Delete the top toll.
        delete _toolStack.top();

        //Remove it from the top.
        _toolStack.pop();
    }

    //Delete all items in the scene
    for (GraphicsItem* item : _itemsList)
    {
        delete item;
    }
    doneCurrent();

    _itemsList.clear();

    deleteAllViews();

    _surface.destroy();
    
    //Destroy all lighting models of the scene
    for (size_t i = 0; i < _shadingModels.size(); i++)
    {
        _shadingModels[i].destroy();
    }
	
    _shadingModels.clear();
}



bool GraphicsScene::removeView(GraphicsView* view)
{
    //Remove view from conteiner.
    return _views.erase( view->id()) > 0;
}



ShadingModel GraphicsScene::createShadingModel()
{
    //Create a new shading model.
    ShadingModel m(true);
    m.createLightSource(LightType::Directional);

    //Save the shading model.
    _shadingModels.push_back(m);

    //return the new shading model.
    return m;
}



ShadingModel GraphicsScene::getShadingModel(unsigned int id)
{
    return _shadingModels[id];
}



std::vector<ShadingModel>& GraphicsScene::getShadingModels()
{
    return _shadingModels;
}



void GraphicsScene::popTool()
{

    topTool()->finalize();
    _toolStack.pop();

    if( !isToolStackEmpty())
    {
        topTool()->initialize();
    }
    update();
}



void GraphicsScene::pushTool(GraphicsTool* tool)
{
    makeCurrent();
    if( !isToolStackEmpty())
    {
        topTool()->finalize();
    }

    tool->initialize();
    _toolStack.push(tool);
    doneCurrent();

    update();
}



GraphicsTool* GraphicsScene::topTool()
{
    return _toolStack.top();
}



bool GraphicsScene::isToolStackEmpty() const
{
    return _toolStack.empty();
}



void GraphicsScene::bringForward(const SelectionGroup2DItem *item)
{
   if(_itemsList.size() > 1)
   {
       for (auto rit = std::next(_itemsList.rbegin()); rit != _itemsList.rend(); rit++)
        {
           auto previousItem = std::prev(rit);

           if( item->containsItem(dynamic_cast<Graphics2DItem*>(*rit)))
           {
               if(!item->containsItem(dynamic_cast<Graphics2DItem*>(*previousItem)))
               {
                   std::swap(*previousItem, *rit);
               }
           }
        }
   }
}



void GraphicsScene::sendBackward(const SelectionGroup2DItem* item)
{
   if(_itemsList.size() > 1)
   {
       for (auto it = std::next(_itemsList.begin()); it != _itemsList.end(); it++)
       {
           auto previousItem = std::prev(it);

           if( item->containsItem(dynamic_cast<Graphics2DItem*>(*it)))
           {
               if(!item->containsItem(dynamic_cast<Graphics2DItem*>(*previousItem)))
               {
                   std::swap(*previousItem, *it);
               }
           }

       }
   }
}



void GraphicsScene::bringToFront(const SelectionGroup2DItem* items)
{
    //Save the number of element on list to guarantee that will not be executed more loops than number of elements.
    size_t n = _itemsList.size();

    //Current loop index.
    size_t l = 0;
    for(auto it = _itemsList.begin(); it != _itemsList.end() && l < n; l++)
    {
        Graphics2DItem* item2D = dynamic_cast<Graphics2DItem*>(*it);
        if (items->containsItem(item2D))
        {
            //Erase the current element and insert it at the final list.
            it = _itemsList.erase(it);
            _itemsList.insert(_itemsList.end(), item2D);
        }
        else
        {
            ++it;
        }
    }
}



void GraphicsScene::sendToBack(const SelectionGroup2DItem *items)
{
    //Save the number of element on list to guarantee that will not be executed more loops than number of elements.
    size_t n = _itemsList.size();

    //Current loop index.
    size_t l = 0;
    for(auto rit = _itemsList.rbegin(); rit != _itemsList.rend() && l < n; l++)
    {
        Graphics2DItem* item2D = dynamic_cast<Graphics2DItem*>(*rit);
        if (items->containsItem(item2D))
        {
            //Get a normal iterator to use erase function. Each reverse iterator has a normal iterator that points to
            //the next element. We get this iterator using the function base.
            auto it = --rit.base();

            //Remove the element and save the new iterator.
            it = _itemsList.erase(it);

            //Update the revere iterator with the normal iterator.
            rit = std::list<GraphicsItem*>::reverse_iterator(it);

            //Insert the removed element as the new first element of the list.
            _itemsList.insert(_itemsList.begin(), item2D);
        }
        else
        {
            rit++;
        }
    }
}



Graphics2DView* GraphicsScene::create2DView(QWidget* parent)
{
    //Create a new view.
    Graphics2DView* new2DView = new Graphics2DView(this, parent);

    //Connect signals to slots.
    for (auto it = _views.begin(); it != _views.end(); it++)
    {
        //connect update to all former created views.
        QObject::connect(new2DView, SIGNAL(signalUpdate()),
                         it->second, SLOT(onUpdate()));
        QObject::connect(it->second, SIGNAL(signalUpdate()),
                         new2DView, SLOT(onUpdate()));
    }

    //Get the view id.
    int id = new2DView->id();

    //Add to current views container.
    _views[id] = new2DView;

    return new2DView;
}



Graphics3DView* GraphicsScene::create3DView(QWidget* parent)
{
    //Create a new view.
    Graphics3DView* new3DView = new Graphics3DView(this, parent);

    new3DView->setShadingModel(_shadingModels[0]);

    //Connect signals to slots.
    for (auto it = _views.begin(); it != _views.end(); it++)
    {
        //connect update to all former created views.
        QObject::connect(new3DView, SIGNAL(signalUpdate()),
                         it->second, SLOT(onUpdate()));
        QObject::connect(it->second, SIGNAL(signalUpdate()),
                         new3DView, SLOT(onUpdate()));
    }

    //Get the view id.
    int id = new3DView->id();

    //Add to current views container.
    _views[id] = new3DView;

    return new3DView;
}



bool GraphicsScene::deleteView(GraphicsView* view)
{
    //Remove view from conteiner.
    int id = view->id();
    if (_views.erase(id) > 0)
    {
        delete view;
        return true;
    }
    else
    {
        printf("GraphicsView with id = %d does not bellongs to this scene. GraphicsView will be not deleted\n", id);
    }
    return false;
}



const AABB2D GraphicsScene::computeAABB2D() const
{
    AABB2D sceneAABB;

    //TODO: Currently, this search is necessary to find the first valid 2d aabb.
    //A concept of invalid aabb while doing a += operation could avoid this.
    for(auto item : _itemsList)
    {
        Graphics2DItem* item2d = dynamic_cast<Graphics2DItem*>(item);

        if(item2d && item2d->isVisible())
        {
            sceneAABB = item2d->getAABBRender(item2d->getPixelSize());
            break;
        }
    }

    for(auto item : _itemsList)
    {
        Graphics2DItem* item2d = dynamic_cast<Graphics2DItem*>(item);

        if(item2d && item2d->isVisible())
        {
            sceneAABB += item2d->getAABBRender(item2d->getPixelSize());
        }
    }

    return sceneAABB;
}



const AABB3D GraphicsScene::computeAABB3D() const
{
    AABB3D sceneAABB;

    //TODO: Currently, this search is necessary to find the first valid 2d aabb.
    //A concept of invalid aabb while doing a += operation could avoid this.
    for(auto item : _itemsList)
    {
        Graphics3DItem* item3d = dynamic_cast<Graphics3DItem*>(item);

        if(item3d && item3d->isVisible())
        {
            sceneAABB = item3d->getModelMatrix().topMatrix() * item3d->getAABB();
            break;
        }
    }

    for(auto item : _itemsList)
    {
        Graphics3DItem* item3d = dynamic_cast<Graphics3DItem*>(item);

        if(item3d && item3d->isVisible())
        {
            sceneAABB += item3d->getModelMatrix().topMatrix() * item3d->getAABB();
        }
    }

    return sceneAABB;
}



void GraphicsScene::deleteAllViews()
{
    //Delete all views.
    for (auto it = _views.begin(); it != _views.end();)
    {
        //Please, do not change this peace of code without testing the remotion of views by scene and by Qt. The delete
        //operator remove a view from the container. This avoids that GraphicsView try to do that.
        GraphicsView* currentView = it->second;
        it = _views.erase(it);

        //After to remove this view from container, delete the object.
        delete currentView;
    }
}



bool GraphicsScene::isEventEnabled(EventType type)
{
    if ((_enableEvents & static_cast<SceneEvents>(type)) != 0)
	{
		return true;
	}
	return false;
}



void  GraphicsScene::enableEvent(EventType type)
{
    _enableEvents = _enableEvents | static_cast<SceneEvents>(type);
}



void GraphicsScene::disableEvent(EventType type)
{
    _enableEvents = _enableEvents & ~static_cast<SceneEvents>(type);
}



void GraphicsScene::enableAllEvents()
{
	
	_enableEvents = static_cast<SceneEvents>(EventType::AllEvents) ;
}



void GraphicsScene::disableAllEvents()
{
	_enableEvents = static_cast<SceneEvents>(EventType::NoEvent);
}



void  GraphicsScene::enableMouseEvents()
{
    enableEvent(EventType::MoveEvent);
    enableEvent(EventType::PressEvent);
    enableEvent(EventType::DragEnterEvent);
    enableEvent(EventType::DragLeaveEvent);
    enableEvent(EventType::DragMoveEvent);
    enableEvent(EventType::DropEvent);
    enableEvent(EventType::WheelEvent);
    enableEvent(EventType::MouseDoubleClickEvent);
    enableEvent(EventType::HoverEvent);
    enableEvent(EventType::LeaveEvent);
    enableEvent(EventType::EnterEvent);
}



void  GraphicsScene::disableMouseEvents()
{
    disableEvent(EventType::MoveEvent);
    disableEvent(EventType::PressEvent);
    disableEvent(EventType::DragEnterEvent);
    disableEvent(EventType::DragLeaveEvent);
    disableEvent(EventType::DragMoveEvent);
    disableEvent(EventType::DropEvent);
    disableEvent(EventType::WheelEvent);
    disableEvent(EventType::MouseDoubleClickEvent);
    disableEvent(EventType::HoverEvent);
}



void GraphicsScene::makeCurrent()
{
    _glContext.makeCurrent(&_surface);
}



void GraphicsScene::doneCurrent()
{
    _glContext.doneCurrent();
}



void GraphicsScene::addItem(GraphicsItem* item)
{
    _itemsList.push_back( item );
    makeCurrent();
    item->initialize();
    doneCurrent();
}



void GraphicsScene::addItem(GraphicsItem* item, std::list<GraphicsItem*>::const_iterator pos)
{
    _itemsList.insert( pos, item );
    makeCurrent();
    item->initialize();
    doneCurrent();
}



bool GraphicsScene::isItemOnScene(const GraphicsItem *item) const
{
     return std::find(_itemsList.begin(), _itemsList.end(), item) != _itemsList.end();
}



std::list<GraphicsItem*>::const_iterator GraphicsScene::removeItem(GraphicsItem* item)
{
    std::list<GraphicsItem*>::const_iterator it = std::find(_itemsList.begin(), _itemsList.end(), item);

    if (it != _itemsList.end())
    {
        it = _itemsList.erase(it);
    }
    else
    {
        std::cout << "NOT FOUND!" << std::endl;
    }
    update();

    return it;
}



void GraphicsScene::editItem(GraphicsItem* , double , QVector3D )
{
}



void GraphicsScene::editItem(GraphicsItem* , double , double , double )
{
}



//void GraphicsScene::editItem(GraphicsItem* item, double dx, double dy, double dz)
//{
//}



void GraphicsScene::editItemPoint(GraphicsItem* , int , const QVector3D )
{
}



void GraphicsScene::editItemPoint(GraphicsItem* , int , std::pair<float,float> )
{
}



void GraphicsScene::editItemSegment(GraphicsItem* , std::list<QRectF> , QVector3D )
{
}



void GraphicsScene::clearFocus()
{
}



/*const std::vector<GraphicsItem*>& GraphicsScene::collidingItens(GraphicsItem* item) const
{
    return nullptr;
}*/



bool GraphicsScene::collidingWith(GraphicsItem* , GraphicsItem* )
{
	return false;
}



bool GraphicsScene::itemIscolliding(GraphicsItem* )
{
	return false;
}



GraphicsItem* GraphicsScene::focusItem() const
{
    return _itemInFocus;
}



void GraphicsScene::setFocusItem(GraphicsItem *item)
{

    if (_itemInFocus != nullptr)
    {
        _itemInFocus->onFocus(false);
    }
    if (item != nullptr)
    {
        item->onFocus(true);
    }
    _itemInFocus = item;
    update();
}



QFont GraphicsScene::font() const
{
    return QFont();
}



void GraphicsScene::setFont(QFont* )
{
}



QColor GraphicsScene::foregroundColor() const
{
    return QColor();
}



QColor GraphicsScene::backgroundColor() const
{
    return QColor();
}



void GraphicsScene::setForegroundColor(QColor )
{
}



void GraphicsScene::setBackgroundColor(QColor )
{
}



bool GraphicsScene::isActive() const
{
	return false;
}



const GraphicsItem* GraphicsScene::itemAt(double , double , QTransform ) const
{
    return nullptr;
}



const std::list<GraphicsItem *> &GraphicsScene::items() const
{
    return _itemsList;
}



std::list<GraphicsItem *> &GraphicsScene::items()
{
    return _itemsList;
}



const GraphicsItem* GraphicsScene::mouseGrabberItem() const
{
    return nullptr;
}



/*const std::vector<GraphicsItem*>& GraphicsScene::selectedItens() const
{

}*/



/*const Path& GraphicsScene::selectionArea() const
{
}*/



/*void GraphicsScene::setSelectionArea(const Polyline* path, const QTransform transform)
{
}*/



bool GraphicsScene::sendEvent(std::list<GraphicsItem *> & , GraphicsSceneEvent* )
{
	return false;
}



void* GraphicsScene::itemIndexMethod()
{
    return nullptr;
}



void GraphicsScene::setItemIndexMethod(void* )
{
}



QRect GraphicsScene::boundingBox() const
{
    return QRect();
}



void GraphicsScene::setBoudingBox(QRect )
{
}



void GraphicsScene::update(const QRectF & )
{
}



void GraphicsScene::update()
{
    if (_views.size() > 0)
    {
        _views.begin()->second->update();
        emit _views[0]->signalUpdate();
    }
}



void GraphicsScene::clear()
{
}



void GraphicsScene::clearSelection()
{
}



void GraphicsScene::changed(const std::list<QRectF>& )
{
}



void GraphicsScene::focusItemChanged(GraphicsItem* newFocusItem, GraphicsItem* oldFocusItem)
{
    if (_itemInFocus == oldFocusItem)
    {
        _itemInFocus->onFocus(false);
        newFocusItem->onFocus(true);
        _itemInFocus = newFocusItem;
    }
}



void GraphicsScene::sceneBoundingBoxChanged(const QRect& )
{
}



void GraphicsScene::selectionChanged()
{
}



//bool GraphicsScene::event(GraphicsSceneEvent* event)
//{
//    makeCurrent();
//    if (event->getEventType() == EventType::MoveEvent)
//    {
//        // Convert event to correct type event.
//        GraphicsSceneMoveEvent* moveEvent = static_cast<GraphicsSceneMoveEvent*>(event);

//        //Send event move event to all items.
//        //TODO: check items intersection.
//        //check if event affects this item
//        if (_mouseGrabberItem != nullptr)
//        {
//            _mouseGrabberItem->mouseMoveEvent(moveEvent);
//        }
//    }
//    else if (event->getEventType() == EventType::PressEvent)
//    {
//        // Convert event to correct type event.
//        GraphicsScenePressEvent* pressEvent = static_cast<GraphicsScenePressEvent*>(event);

//        //Send event press/release event to all items.
//        //TODO: check items intersection.
//        switch (pressEvent->getMouseButtonAction())
//        {
//        case MouseButtonAction::Press:
//            for(GraphicsItem* item : _itemsList)
//            {
//                if (item->isIntersecting(pressEvent->getWorldPosition()))
//                {
//                    printf("Intersecta Linha\n");
//                    _mouseGrabberItem = item;
//                    item->mousePressEvent(pressEvent);
//                }
//            }
//            break;

//        case rm::MouseButtonAction::Release:
//            for(GraphicsItem* item : _itemsList)
//            {
//                item->mouseReleaseEvent(pressEvent);
//            }
//            //Button was release. Release any mouse grabber item
//            _mouseGrabberItem = nullptr;
//            break;

//        default:
//            printf("Error: unrecognized button action\n");
//            break;
//        }
//    }
//    else if (event->getEventType() == EventType::WheelEvent)
//    {
//        // Convert event to correct type event.
//        GraphicsSceneWheelEvent* wheelEvent = static_cast<GraphicsSceneWheelEvent*>(event);

//        //Send event wheel event to all items.
//        //TODO: check items intersection.
//        for(GraphicsItem* item : _itemsList)
//        {
//            item->wheelEvent(wheelEvent);
//        }
//    }
//    else if (event->getEventType() == EventType::keyEvent)
//    {
//        GraphicsSceneKeyEvent* keyEvent = static_cast<GraphicsSceneKeyEvent*>(event);

//        switch(keyEvent->getKeyEventType())
//        {

//        case rm::KeyAction::pressEvent:
//            for(GraphicsItem* item : _itemsList)
//            {
//                item->keyPressEvent(keyEvent);
//            }
//            break;
//        case rm::KeyAction::releaseEvent:
//            for(GraphicsItem* item : _itemsList)
//            {
//                item->keyReleaseEvent(keyEvent);
//            }
//            break;
//        default:
//            std::cout<< "Error: unrecognized key action"<< std::endl;
//            break;

//        }
//    }
//    else if (event->getEventType() == EventType::HoverEvent)
//    {
//        GraphicsSceneHoverEvent* hoverEvent = static_cast<GraphicsSceneHoverEvent*>(event);
//        for(GraphicsItem* item : _itemsList)
//        {
//            item->mouseHoverEvent(hoverEvent);
//        }
//    }
//    else
//    {
//       doneCurrent();
//       return false;
//    }
//    doneCurrent();
//    return true;
//}
}
