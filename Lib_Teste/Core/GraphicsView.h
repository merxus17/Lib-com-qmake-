#pragma once
#include <vector>
#include <QOpenGLContext>
#include <QOpenGLWidget>


#include "../Events/EventConstants.h"
#include "GraphicsScene.h"

#include "../Items/RectangleZoomItem.h"
#include "../Core/GraphicsSceneEvent.h"
#include "../Events/GraphicsScenePressEvent.h"
#include "../Events/GraphicsSceneMoveEvent.h"
#include "../Events/GraphicsSceneHoverEvent.h"
#include "../Events/GraphicsSceneKeyEvent.h"
#include "../Events/GraphicsSceneWheelEvent.h"

//TODO: include correct Qt and RM class events.

class QEvent;
class QMouseEvent;
namespace rm
{
class QChangeEvent;
class Point3D;
class CloseEvent;
//class HideEvent;


class GraphicsView : public QOpenGLWidget
{
    Q_OBJECT

public:
    /**
     * It allows to GraphicsView destructor to call a special function to remove the current view from your scene.
     * This is important because when a view is deleted, the view's conteiner must be updated.
     */
    friend class GraphicsScene;

    /**
     * @brief ~GraphicsView - Destructor.
     */
    virtual ~GraphicsView() override;

    /**
     * @brief paintGL  This function should be called whenever you need to repain the widget
     */
    virtual void paintGL() override = 0;

    /**
     * @brief resizeGL The new size is passed in w and h. It changes the size of the widget
     * W and H in pixels
     * @param w Width in pixels
     * @param h Height in pixels
     */
    virtual void resizeGL(int w, int h) override = 0;

    /**
     * @brief initializeGL This function should set up any required OpenGL resources and state
     */
    virtual void initializeGL() override = 0;

    /**
     * @brief fit - Fit all visiable objects at view.
     */
    virtual void fit() = 0;

    /**
     * @brief focusNextPrevChild Finds a new widget to give the keyboard focus to, as appropriate for Tab and Shift+Tab.
     * @param next If next is true, this function searches forward, if next is false, it searches backward.
     * @return Returns true if it can find a new widget, or false if it can't.
     */
    virtual bool focusNextPrevChild(bool next) override;

    /**
     * @brief scrollContentsByCalls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param dx Amount in the x direction
     * @param dy Amount in the y direction
     */
	virtual void scrollContentsBy(int dx, int dy);

    /**
     * @brief pan - make a pan action on canvas.
     * @param oldPoint - the old point.
     * @param newPoint - the new point.
     */
    virtual void pan(const Point2Df& oldPoint, const Point2Df& newPoint) = 0;

    /**
     * @brief zoomInOut - Function that implements the Zoom in and Zoom out
     * @param zoomfactor - The factor used to implement zoom
     */
    virtual void zoomInOut(float zoomfactor);

    /**
     * @brief fitIn This function should change the projection matrix so it fit in the selected area without distorting the scene.
     * In other words, you should keep the aspect ration of the scene giving priority to one of the input values or try
     * to find a balence between the height and width
     * @param iniPoint initialPoint
     * @param finalPoint finalPoint
     */
    void fitIn(Point3D& iniPoint, Point3D& finalPoint);

    /**
     * @brief zoomArea Draw a rectagle around an area that should be displayed in the view
     * @param iniPoint zoom area initial point
     * @param finalPoint zoom area final point
     */
    virtual void zoomArea(const Point2Df& iniPoint, const Point2Df& finalPoint) = 0;

    /**
     * @brief getRectangleZoomItem - Return the current rectangle zoom item object.
     * @return - current rectangle zoom item object.
     */
    RectangleZoomItem& getRectangleZoomItem();

    /**
     * @brief getRectangleZoomItem - Return the current rectangle zoom item object.
     * @return - current rectangle zoom item object.
     */
    const RectangleZoomItem& getRectangleZoomItem() const;

    /**
     * @brief viewFrom - This is an equivalent of lookAt at view3D, we are keeping this in case view2D has an equal or similar lookAt
     * function
     */
    virtual void viewFrom();

    /**
     * @brief setScene - Sets the scene in the view
     * @param scene
     */
	void setScene(GraphicsScene& scene);

    /**
     * @brief getScene - Get the scene which the view is from
     * @return Returns the scene that the view has
     */
    GraphicsScene* getScene();

    /**
     * @brief id - Return the view identifier.
     * @return - integer that represents the view id.
     */
    int id();

    /**
     * @brief numberOfViews - Get the number of view that was allocated.
     * @return - number of view that was allocated.
     */
    static int numberOfViews();

    /**
     * @brief isPreview - Get the value of the preview
     * @return _preview
     */
    bool isPreview() const;

    /**
     * @brief preview - Sets the value of the preview
     * @param boolean that sets the value of the preview
     */
    void preview(bool preview);

    /**
     * @brief convertFromScreenToWorld - Convert from the canvas coordinate system (screen) to world coordinate system.
     * @param screen - point on canvas coordinate system.
     * @return - point on world coordinate system.
     */
    virtual Point2Df convertFromScreenToWorld(const Point2Df& screen) = 0;

public slots:

    void onUpdate();

signals:

    void signalUpdate();

protected:

enum class ViewType
{
    VIEW_2D,
    VIEW_3D
};

protected:

    /**
     * @brief _scene - Scene which the view is looking.
     */
    GraphicsScene* _scene {nullptr};

    /**
     * @brief _rectangleZoom - Special item to be used on zoom area operation. It must to belongs to one view.
     */
    RectangleZoomItem _rectangleZoom;

protected:
    /**
     * @brief GraphicsView - Graphics view construtor.
     * @param scene - scene that this view must render.
     * @param parent - parent widget.
     * @param type - view type.
     * @param preview - boolean if the view is or not a preview
     */
    GraphicsView(GraphicsScene* scene, QWidget *parent, ViewType type, bool preview = false);

    /**
     * @brief mouseMoveEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void mouseMoveEvent(QMouseEvent* event) override;

    /**
     * @brief mousePressEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void mousePressEvent(QMouseEvent* event) override;

    /**
     * @brief mouseReleaseEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void mouseReleaseEvent(QMouseEvent* event) override;

    /**
     * @brief keyPressEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void keyPressEvent(QKeyEvent* event) override;

    /**
     * @brief keyReleaseEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void keyReleaseEvent(QKeyEvent* event) override;

    /**
     * @brief contextMenuEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void contextMenuEvent(QContextMenuEvent* event) override;

    /**
     * @brief dragEnterEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void dragEnterEvent(QDragEnterEvent* event) override;

    /**
     * @brief dragLeaveEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    void dragLeaveEvent(QDragLeaveEvent* event) override;

    /**
     * @brief dragMoveEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void dragMoveEvent(QDragMoveEvent * event) override;

    /**
     * @brief dropEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void dropEvent(QDropEvent* event) override;

    /**
     * @brief focusInEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void focusInEvent(QFocusEvent * event) override;

    /**
     * @brief focusOutEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void focusOutEvent(QFocusEvent * event) override;

    /**
     * @brief inputMethodEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    void inputMethodEvent(QInputMethodEvent * event) override;

    /**
     * @brief mouseDoubleClickEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

    /**
     * @brief paintEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    //virtual void paintEvent(QPaintEvent* event);

    /**
     * @brief resizeEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    //virtual void resizeEvent(QResizeEvent* event);

    /**
     * @brief showEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void showEvent(QShowEvent* event) override;

    /**
     * @brief changeEvent - Handle view state changes. GraphicView exclusive event. Does not porpagate to the scene or items.
     * @param event - Qt event
     */
    //void changeEvent(QChangeEvent* event);

    /**
     * @brief closeEvent - Occurs when the view is closed. Does not propagate to scene or items.
     * @param event - Qt event
     */
    //void closeEvent(CloseEvent* event);

    /**
     * @brief enterEvent - Event called when mouse enters the view
     * @param event - Qt event
     */
    void enterEvent(QEvent* event) override;

    /**
     * @brief leaveEvent - Event called when mouse leaves the view
     * @param event - Qt event
     */
    void leaveEvent(QEvent* event) override;

    /**
     * @brief hideEvent - Event called when the view is hidded
     * @param event - Qt event
     */
    void hideEvent(QHideEvent* event) override;

    /**
     * @brief viewportEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    //virtual void viewportEvent(QEvent* event) override;

    /**
     * @brief wheelEvent - Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * @param event - Qt event
     */
    virtual void wheelEvent(QWheelEvent* event) override;

    /**
     * @brief translateQEvent - Receive a QEvent and translate to the appropriate version of GraphicsSceneEvent
     * @param event - Qt event
     * @return
     */
    GraphicsSceneEvent& translateQEvent(QEvent* event);

    /**
     * Calls translateQEvent and propagates the GraphicsSceneEvent to the GraphicsScene
     * TODO: is this function an override from QOpenGLWidget?
     */
    //virtual GraphicsSceneEvent& event(QEvent* event);

private:

    /**
     * @brief _context - The OpenGL context.
     */
    QOpenGLContext _context;

    /**
     * @brief _globalViewId - Variable to store a view identifier that is incremented to each new window.
     */
    static int _globalViewId;

    /**
     * @brief _viewId - Identifier for the current object.
     */
    int _viewId {-1};

    /**
     * @brief _preview - Identifier to check is the view is a preview or not.
     */
    bool _preview {false};

    /**
     * Event objetcs.
     */

    /**
     * @brief _mousePress - Mouse press event object.
     */
    GraphicsScenePressEvent _mousePress;

    /**
     * @brief _mouseMove - Mouse move event object.
     */
    GraphicsSceneMoveEvent _mouseMove;

    /**
     * @brief _mouseHover - Mouse hover event object.
     */
    GraphicsSceneHoverEvent _mouseHover;

    /**
     * @brief _keyEvent - Mouse key event object.
     */
    GraphicsSceneKeyEvent _keyEvent;

    /**
     *  @brief _wheelEvent - Wheel event object.
     */
    GraphicsSceneWheelEvent _wheelEvent;

    /**
     *  @brief _leaveEvent - Leave event object.
     */
    GraphicsSceneEvent _leaveEvent{EventType::LeaveEvent};

    /**
     *  @brief _enterEvent - Enter event object.
     */
    GraphicsSceneEvent _enterEvent{EventType::EnterEvent};

    /**
     * @brief _type - View type.
     */
    ViewType _type;
};
}
