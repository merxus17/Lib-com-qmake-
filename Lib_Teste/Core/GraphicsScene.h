#pragma once

#include <vector>
#include <list>
#include <stack>
#include <QColor>
#include <QFont>
#include <QRect>
#include <QVector3D>
#include <utility>
#include <list>
#include <QRectF>
#include <QTransform>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include "GraphicsTool.h"
#include "../Shading/ShadingModel.h"
#include "../Events/EventConstants.h"
#include "../Geometry/AxisAligmentBoundingBox.h"


namespace rm
{
class Path;
class Polyline;
class GraphicsItem;
class GraphicsSceneEvent;
class GraphicsView;
class Graphics2DView;
class Graphics3DView;
class SelectionGroup2DItem;
class GraphicsScene
{    
public:
    /**
     * Allow a GraphicsView object to remove itself from a scene.
     */
    friend class GraphicsView;

    /**
     * @brief GraphicsScene Creates a new scene, without any items.
     */
    GraphicsScene();

    /**
     * @brief create2DView - create a new 2D view to this scene. This object cannot be deleted externally. Use the
     * eleteView (deleteAllViews) methods to destroy a view (all views).
     * @param parent - parent widget where the view will be created.
     * @return - return a 2D view to current scene.
     */
    Graphics2DView* create2DView(QWidget *parent = nullptr);

    /**
     * @brief create3DView - create a new 3D view to this scene. This object cannot be deleted externally. Use the
     * deleteView (deleteAllViews) methods to destroy a view (all views).
     * @param parent - parent widget where the view will be created.
     * @return - return a 3D view to current scene.
     */
    Graphics3DView* create3DView(QWidget *parent = nullptr);

    /**
     * @brief deleteView - destroy a view that was allocated by this scene (not by another scene).
     * @param view - view to be deleted.
     * @return - true if the view was deleted and false otherwise. In case of false return, the view does not belong
     * to view set that render this scene on this moment.
     */
    bool deleteView(GraphicsView* view);

    /**
     * @brief computeAABB2D - compute the AABB for 2D items.
     * @return - the AABB for 2D items.
     */
    const AABB2D computeAABB2D() const;

    /**
     * @brief computeAABB3D - compute the AABB for 3D items.
     * @return - the AABB for 3D items.
     */
    const AABB3D computeAABB3D() const;

    /**
     * @brief deleteAllViews - delete all current allocated views.
     */
    void deleteAllViews();

    /**
     * @brief Destroy the scene, when the program finishes
     */
    ~GraphicsScene();

    /**
     * @brief isEnabled - checks if a specific event is enabled
     * @param type - event type to be checked
     * @return - true if event is enabled and false otherwise.
     */
    bool isEventEnabled(EventType type);

    /**
     * @brief enableEvent - enables the EventType passed as argument
     * @param type - type of event to be enabled
     */
    void enableEvent(EventType type);

    /**
     * @brief disableEvent - disables the EventType passed as argument
     * @param type - type of event to be disabled
     */
    void disableEvent(EventType type);

    /**
     * @brief enableMouseEvents - enables all the EventTypes related to the mouse
     */
    void enableMouseEvents();

    /**
     * @brief disableEvent - eisables all the EventTypes related to the mouse
     */
    void disableMouseEvents();

    /**
     * @brief enableAllEvents - enables all events
     */
    void enableAllEvents();

    /**
     * @brief disableAllEvents - disables all events
     */
    void disableAllEvents();

    /**
     * @brief makeCurrent - makes the scene context current.
     */
    void makeCurrent();

    /**
     * @brief doneCurrent - release the scene context.
     */
    void doneCurrent();

    /**
     * @brief addItem - Receives an GraphicsItem object and adds to data structure. Emits a changed() signal if object’s
     *  attribute _visible is True.
     * @param item - GraphicsItem to add
     */
    void addItem(GraphicsItem * item);

    /**
     * @brief addItem - Receives an GraphicsItem object and adds to data structure. Emits a changed() signal if object’s
     *  attribute _visible is True.
     * @param item - GraphicsItem to add
     * @param pos - Inserts value before pos
     */
    void addItem(GraphicsItem* item, std::list<GraphicsItem*>::const_iterator pos);

    /**
     * @brief isItemOnScene - Verify if the item is on scene.
     * @param item - Item to be checked.
     * @return - True if the item is on scene and false otherwise.
     */
    bool isItemOnScene(const GraphicsItem *item) const;

    /**
     * @brief removeItem - Receives an GraphicsItem object and removes from data structure. Emits a changed() signal if object’s
     *  attribute _visible is True.
     * @param item - GraphicsItem to remove
     */
    std::list<GraphicsItem*>::const_iterator removeItem(GraphicsItem * item);

    /**
     * @brief Edit coordinates of the given item according to the parameters values, rotating the item in an angle
     *  theta, around the vector received. Emits a changed() signal.
     * @param item
     * @param theta angle
     * @param vector
     */
    void editItem(GraphicsItem * item, double theta, QVector3D vector);

    /**
     * @brief editItem
     * @param item GraphicsItem to edit
     * @param sx Amount to translate in the x direction
     * @param sy Amount to translate in the y direction
     * @param sz Amount to translate in the z direction
     */
    void editItem(GraphicsItem * item, double sx, double sy, double sz);

    /**
     * TODO: mesmo cabeçalho, não pode.
     * Edit coordinates of the given item, according to the values passed as parameters, translating the  item to the
     *  position (dx,dy,dz) . Emits a changed() signal.
     */
    //void editItem(GraphicsItem * item, double dx, double dy, double dz);

    /**
     * @brief editItemPoint Edit the point that is on the given 3D item's list. The index of the point is pointID and
     *  the translateFactor is how much the point coordenate will be translated from the position it is now.
     * @param item GraphicsItem to edit
     * @param pointID Id of point
     * @param translateFactor Amount to translate in each direction
     */
    void editItemPoint(GraphicsItem* item, int pointID, const QVector3D translateFactor);

    /**
     * @brief editItemPoint Edit the point that is on the given 2D item's list. The index of the point is pointID and
     *  the translateFactor is how much the point coordenate will be translated from the position it is now.
     * @param item GraphicsItem to edit
     * @param pointID Id of point
     * @param translateFactor Amount to translate in each direction
     */
    void editItemPoint(GraphicsItem* item, int pointID, std::pair<float,float> translateFactor);

    /**
     * @brief editItemSegment Edit a segment of the given item. The segment starts on the point with the index equal to
     * the first element of the pair of pointID and finishes in the point with the index equal to the second element.
     * @param item GraphicsItem to edit
     * @param pointsID If of point
     * @param translateFactor
     */
    void editItemSegment(GraphicsItem * item, std::list<QRectF> pointsID, QVector3D translateFactor);

    /**
     * @brief Clears focus from the scene. If any item has focus when this function is called, it will lose focus,
     *  and regain focus again once the scene regains focus.
     */
    void clearFocus();

    /**
     * TODO: Returns the vector of itens that collide with a given item.
     */
    /**
     * @brief collidingItens Returns the vector of itens that collide with a given item.
     * @param item GraphicsItem to test collision
     * @return List of collided items
     */
    const std::list<GraphicsItem*>& collidingItens(GraphicsItem * item) const;

    /**
     * @brief collidingWith  Receives two itens and returns if the they are colliding or not.
     * @param item GraphicsItem1 to test collision
     * @param itemColliding GraphicsItem2 to test collision
     * @return True or false if collides or not
     */
    bool collidingWith(GraphicsItem * item, GraphicsItem * itemColliding);

    /**
     * @brief itemIscolliding Returns if the given item is colliding with any other item in the scene or not.
     * @param item GraphicsItem to check collision with any other GraphicsItem on the scene
     * @return True if the given item is colliding with any other item from scene and False does not.
     */
    bool itemIscolliding(GraphicsItem * item);

    /**
     * @brief Returns the scene's item in focus
     * @return item currently in focus
     */
    GraphicsItem * focusItem() const;

    /**
     * @brief setFocusItem Set the given item as the item currently in focus
     * @param item an item
     */
    void setFocusItem(GraphicsItem* item);

    /**
     * @brief Returns the font been used in the scene
     */
    QFont font() const;

    /**
     * @brief Sets the font to be used in the scene
     */
    void setFont(QFont* font);

    /**
     * @brief Returns the color associated with scene foreground
     */
    QColor foregroundColor() const;

    /**
     * @brief Returns the color associated with scene's background
     */
    QColor backgroundColor() const;

    /**
     * @brief Sets the scene's foreground color
     */
    void setForegroundColor(QColor color);

    /**
     * @brief Sets the scene's background color
     */
    void setBackgroundColor(QColor color);

    /**
     * @brief Returns if the scene is active or not. If it is, it has to be shown.
     */
    bool isActive() const;

    /**
     * @brief Returns the topmost visible item at the specified position, or 0 if there are no items at this position.
     */
    const GraphicsItem* itemAt(double x, double y, QTransform transform) const;

    /**
     * @brief itens - Returns an ordered list of all items on the scene. The order is by visibility on the scene.
     * @return - list of all items.
     */
    const std::list<GraphicsItem*>& items() const;

    /**
     * @brief itens - Returns an ordered list of all items on the scene. The order is by visibility on the scene.
     * @return - list of all items.
     */
    std::list<GraphicsItem*>& items();

    /**
     * @brief Returns the current item that is grabbed , or null if no item is currently grabbed by the mouse. The mouse
     * grabber item is the item that receives all mouse events sent to the scene.
     */
    const GraphicsItem * mouseGrabberItem() const;

    /**
     * @brief Returns a list of all currently selected items. The items are returned in no particular order.
     */
    const std::list<GraphicsItem*>& selectedItens() const;

    /**
     * @brief Returns the selection area that was previously set with setSelectionArea(),
     *  or an empty Path if no selection
     * area has been set.
     */
    const Path& selectionArea() const;

    /**
     * @brief Sets the selection area to path. All items within this area are immediately selected,
     *  and all items outside are unselected. You can get the list of all selected items by calling selectedItems()
     */
    //void setSelectionArea(const Polyline* path, const QTransform transform);

    /**
     * @brief sendEvent Send an GraphicScene event to a list of GraphicsItem that are in focus.
     * @param item List of GraphicsItem to send the event
     * @param event Which event to send
     * @return
     */
    bool sendEvent(std::list<GraphicsItem*>& item, GraphicsSceneEvent* event);

    /**
     * @brief This property holds the item indexing method.
     */
    void* itemIndexMethod();

    /**
     * @brief Sets the ItemIndexMethod using a function that is given by parameter.
     */
    void setItemIndexMethod(void* indexMethod);

    /**
     * @brief This function returns the  box that involves scene.
     */
    QRect boundingBox() const;

    /**
     * @brief This function sets the  box (rectangle) that involves scene.
     */
    void setBoudingBox(QRect rect);

    /**
     * @brief Schedules a redraw of the area rect on the scene.
     */
    void update(const QRectF & rect);

    /**
     * @brief Schedules a redraw of the entire scene.
     */
    void update();

    /**
     * @brief Removes and deletes all items from the scene, but otherwise leaves the state of the scene unchanged.
     */
    void clear();

    /**
     * @brief Clears the current selections.
     */
    void clearSelection();

    /**
     * @brief changed This signal is emitted  when control reaches the event loop, if the scene content changes.
     * @param region Where it chenaged
     */
    void changed(const std::list<QRectF>& region);

    /**
     * @brief focusItemChanged This signal is emitted by GraphicsScene whenever focus changes in the scene
     * (i.e., when an item gains or loses input focus, or when focus passes from one item to another).
     * @param newFocusItem New Item in focus
     * @param oldFocusItem Old item in focus
     */
    void focusItemChanged(GraphicsItem* newFocusItem, GraphicsItem* oldFocusItem);

    /**
     * @brief sceneBoundingBoxChanged This signal is emitted by GraphicsScene whenever the scene rect changes.
     *  The rect parameter is the new scene rectangle.
     * @param rect
     */
    void sceneBoundingBoxChanged(const QRect& rect);

    /**
     * @brief selectionChanged This signal is emitted  whenever the selection changes. You can call selectedItems()
     * to get the new list of selected items.
     */
    void selectionChanged();

    /**
     * @brief Processes the GraphicSceneEvent event, and dispatches it to the respective event handlers.
     */
    //bool event(GraphicsSceneEvent* event);

    /**
     * @brief createShadingModel - Create a new ShadingModel object and add to a list of models avaliable.
     * @return - new shading model created by graphics scene.
     */
    ShadingModel createShadingModel();

    /**
     * @brief getShadingModel - Return a ShadingModel giving a id.
     * @param id -  Id to look for.
     * @return - a shading model with identifier id.
     */
    ShadingModel getShadingModel(unsigned int id);

    /**
     * @brief getShadingModels -  Returns all shading models avaliable to the scene.
     * @return All shading models avaliable to the scene.
     */
    std::vector<ShadingModel>& getShadingModels();

    /**
     * @brief popTool - Removes the tool in the top of the stack
     */
    void popTool();

    /**
     * @brief pushTool - Put the tool passed as parameter in the top of the stack
     * @param tool - A GraphicsTool object
     */
    void pushTool(GraphicsTool* tool);

    /**
     * @brief topTool - Returns a reference to the tool that is currently at the top of the stack.
     * @return The tool at the top of the stack.
     */
    GraphicsTool* topTool();

    /**
     * @brief isToolStackEmpty Returns if the tool stack is empty or not.
     * @return true or false.
     */
    bool isToolStackEmpty() const;

    /**
     * @brief bringForward - Change the order of items inside of a scene rendering one layer closer to the screen.
     * Consequently it appears for the user one layer closer.
     * @param item - selected items to be sent forward
     */
    void bringForward(const SelectionGroup2DItem *item);

    /**
     * @brief sendBackward - Change the order of items inside of a scene rendering one layer further away from the screen.
     * Consequently it appears for the user one layer closer.
     * @param item - selected itens to be sent backward
     */
    void sendBackward(const SelectionGroup2DItem *item);

    /**
     * @brief bringToFront - Change the order of items inside of a scene rendering to the closest layer of the screen.
     * Consequently it appears for the user as in front of others items.
     * @param items - Group of selected items to send to front.
     */
    void bringToFront(const SelectionGroup2DItem* items);

    /**
     * @brief sendToBack - Change the order of items inside of a scene rendering to the furthest layer away from the screen.
     * Consequently it appears for the user as behind of others items.
     * @param items - Group of selected items to send to back.
     */
    void sendToBack(const SelectionGroup2DItem* items);

protected:

private:
    /**
     * @brief removeView - remove a view from view's conteiner but not delete it. This is a special function used for
     * GraphicsView class to remove a view from Graphics Scene before to be deleted for your parent.
     * @param view - view to be removed.
     * @return - return true if this view is on GraphicsScene container and false otherwise.
     */
    bool removeView(GraphicsView* view);
private:
    /**
     * @brief _itemsList List of itens in the scene.
     */
    std::list<GraphicsItem*>_itemsList;

    /**
     * @brief _views Store all views that render this scene.
     */
    std::map<int, GraphicsView*> _views;

    /**
     * @brief _shadingModels Store _shadingModels avaliable to the scene.
     */
    std::vector<ShadingModel> _shadingModels;

    /**
     * @brief _mouseGrabberItem Pointer to the grabbed item
     */
    GraphicsItem* _mouseGrabberItem;

    /**
     * @brief _ItemIndexMethod Pointer to the function that indexes itens to the scene.
     */
	void* _ItemIndexMethod;

    /**
     * @brief _backGroundColor Background color of the scene
     */
    QColor _backGroundColor;

    /**
     * @brief _foregroundColor Foreground color of the scene
     */
    QColor _foregroundColor;

    /**
     * @brief _font Font of the letter used to write something on the scene
     */
    QFont* _font;

    /**
     * @brief _active Says if the scene is active or not ( if yes it has to be shown)
     */
	bool _active;

    /**
     * @brief _sceneBoundingBox Box that involves all the scene
     */
    QRect _sceneBoundingBox;

    /**
     * @brief _itemInFocus Pointer to the item that is in focus.
     */
    GraphicsItem* _itemInFocus;

    /**
     * @brief _glContext OpenGL context to create all OpenGL buffers to scene.
     */
    QOpenGLContext _glContext;

    /**
     * @brief _surface Surface to be used with QOpenGLContext  to create resources;
     */
    QOffscreenSurface _surface;

    /**
     * @brief _enableEvents Store all current enable events.
     */
    SceneEvents _enableEvents;

    /**
     * @brief _toolStack Stack with all tools used by the views
     */
    std::stack<GraphicsTool*> _toolStack;
};
}
