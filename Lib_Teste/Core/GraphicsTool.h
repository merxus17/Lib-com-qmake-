#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <assert.h>
#include <QObject>

namespace rm
{
class GraphicsSceneEvent;
class GraphicsView;
class GraphicsScenePressEvent;
class GraphicsSceneMoveEvent;
class GraphicsSceneHoverEvent;
class GraphicsSceneKeyEvent;
class GraphicsSceneWheelEvent;
class GraphicsSceneDragEnterEvent;
class GraphicsSceneDragLeaveEvent;
class GraphicsSceneDragMoveEvent;

class GraphicsTool
{
public:

    /**
     * Empty Constructor
     */
    GraphicsTool() {}
    /**
     * default destructor
     */
    virtual ~GraphicsTool() = default;

    /**
     * @brief mousePress2D - Define how the tool handles mouse press events in 2D.
     * @param event - A mouse press event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view );

    /**
     * @brief mouseMoveEvent2D - Define how the tool handles mouse move events in 2D.
     * @param event - A mouse move event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void mouseMoveEvent2D(const GraphicsSceneMoveEvent* event,GraphicsView* view);

    /**
     * @brief mouseRelease2D - Define how the tool handles mouse release events in 2D.
     * @param event - A mouse press event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view);

    /**
     * @brief mouseHover2D - Define how the tool handles mouse hover events in 2D.
     * @param event - A mouse hover event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void mouseHoverEvent2D(const GraphicsSceneHoverEvent* event, GraphicsView* view);

    /**
     * @brief mousePress3D - Define how the tool handles mouse press events in 3D.
     * @param event - A mouse press event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void mousePressEvent3D(const GraphicsScenePressEvent* event, GraphicsView* view );

    /**
     * @brief mouseMove3D - Define how the tool handles mouse move events in 3D.
     * @param event - A mouse move event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void mouseMoveEvent3D(const GraphicsSceneMoveEvent* event,GraphicsView* view);

    /**
     * @brief mouseRelease3D - Define how the tool handles mouse release events in 3D.
     * @param event - A mouse press event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void mouseReleaseEvent3D(const GraphicsScenePressEvent* event, GraphicsView* view);

    /**
     * @brief keyPress2D - Defines how the tool handles key 2D events.
     * @param event - A key press event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void keyPressEvent2D(const GraphicsSceneKeyEvent* event, GraphicsView* view);

    /**
     * @brief keyPress3D - Defines how the tool handles key 3D events.
     * @param event - A key press event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void keyPressEvent3D(const GraphicsSceneKeyEvent* event,GraphicsView* view);

    /**
     * @brief dragEnterEvent2D - Defines how the tool handles drag enter 2D events.
     * @param event - A drag enter event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void dragEnterEvent2D(const GraphicsSceneDragEnterEvent* event, GraphicsView* view);

    /**
     * @brief dragMoveEvent2D - Defines how the tool handles drag move 2D events.
     * @param event - A drag move event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void dragMoveEvent2D(const GraphicsSceneDragMoveEvent * event, GraphicsView* view);

    /**
     * @brief dragHoverEvent2D - Defines how the tool handles drag hover 2D events.
     * @param event - A drag hover event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void dragHoverEvent2D(const GraphicsSceneDragMoveEvent * event, GraphicsView* view);

    /**
     * @brief dragLeaveEvent2D - Defines how the tool handles drag leave 2D events.
     * @param event - A drag leave event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void dragLeaveEvent2D(const GraphicsSceneDragLeaveEvent* event, GraphicsView* view);

    /**
     * @brief wheelEvent2D - Defines how the tool handles wheel 2D events.
     * @param event - A wheel event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void wheelEvent2D(const GraphicsSceneWheelEvent* event , GraphicsView* view );

    /**
     * @brief dragEnterEvent3D - Defines how the tool handles drag enter 3D events.
     * @param event - A drag enter event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void dragEnterEvent3D(const GraphicsSceneDragEnterEvent* event,GraphicsView* view);

    /**
     * @brief dragMoveEvent3D - Defines how the tool handles drag move 3D events.
     * @param event - A drag move event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void dragMoveEvent3D(const GraphicsSceneDragMoveEvent * event,GraphicsView* view);

    /**
     * @brief dragLeaveEvent3D - Defines how the tool handles drag leave 3D events.
     * @param event - A drag leave event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void dragLeaveEvent3D(const GraphicsSceneDragLeaveEvent* event,GraphicsView* view);

    /**
     * @brief wheelEvent3D - Defines how the tool handles wheel events.
     * @param event - A wheel event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void wheelEvent3D(const GraphicsSceneWheelEvent* event , GraphicsView* view);

    /**
     * @brief dragHoverEvent3D - Defines how the tool handles drag hover 3D events.
     * @param event - A drag hover event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void dragHoverEvent3D(const GraphicsSceneDragMoveEvent * event, GraphicsView* view);

    /**
     * @brief mouseHover3D - Define how the tool handles mouse hover events in 2D.
     * @param event - A mouse hover event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void mouseHoverEvent3D(const GraphicsSceneHoverEvent* event, GraphicsView* view);

    /**
     * @brief leaveEvent - Define how the tool handles leave events in 2D.
     * @param event - A graphics scene event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void leaveEvent2D(const GraphicsSceneEvent* event, GraphicsView* view);

    /**
     * @brief enterEvent - Define how the tool handles enter events in 2D.
     * @param event - A graphics scene event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void enterEvent2D(const GraphicsSceneEvent* event, GraphicsView* view);

    /**
     * @brief leaveEvent - Define how the tool handles leave events in 3D.
     * @param event - A graphics scene event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void leaveEvent3D(const GraphicsSceneEvent* event, GraphicsView* view);

    /**
     * @brief enterEvent - Define how the tool handles enter events in 3D.
     * @param event - A graphics scene event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void enterEvent3D(const GraphicsSceneEvent* event, GraphicsView* view);

    /**
     * @brief initialize - initialize the tool to be used. Set previews, add elements on scene, etc. This function will
     * be automatically called when the tool is set as current by a GraphicsScene object.
     */
    virtual void initialize();

    /**
     * @brief finalize - places a tool in an inert state, i. e., remove render feedbacks, previews, etc, withou deleting
     * the tool. This function will be automatically called when the tool is set as not current by a GraphicsScene
     * object.
     */
    virtual void finalize();

    /**
     * @brief name - Return the tool's name
     * @return - The tool's name
     */

    virtual std::string name() = 0;
    /**
     * @brief hasSupport3D - Returns if a tool is supported in 2D.
     * @return - true or false.
     */
    virtual bool hasSupport2D() const = 0;

    /**
     * @brief hasSupport3D - Returns if a tool is supported in 3D.
     * @return - true or false.
     */
    virtual bool hasSupport3D() const = 0;

};
}
