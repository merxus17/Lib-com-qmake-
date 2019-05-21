#pragma once
#include "../Core/GraphicsTool.h"
#include "../Core/Graphics3DView.h"
#include "../Core/Graphics2DView.h"
#include "../Items/RectangleZoomItem.h"

namespace rm
{

class ViewControllerTool: public GraphicsTool
{
public:
    /**
     * Default Constructor
     */
    ViewControllerTool(GraphicsScene* scene);

    /**
     * Default Destructor
     */
    ~ViewControllerTool() override;

    /**
     * @brief mousePress2D - Implements mouse press events in 2D.
     * @param event A mouse press event to deal with
     * @param view The view that will be affected by the tool
     */
    void mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view ) override;

    /**
     * @brief mouseMove2D - Implements mouse move events in 2D.
     * @param event A mouse move event to deal with
     * @param view The view that will be affected by the tool
     */
    void mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView* view) override;

    /**
     * @brief mouseRelease2D - Implements mouse release events in 2D.
     * @param event A mouse press event to deal with
     * @param view The view that will be affected by the tool
     */
    void mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView *view)  override;

    /**
     * @brief mousePress3D - Implements mouse press events in 3D.
     * @param event A mouse press event to deal with
     * @param view The view that will be affected by the tool
     */
    void mousePressEvent3D(const GraphicsScenePressEvent* event, GraphicsView* view )  override;

    /**
     * @brief mouseMove3D - Implements mouse move events in 3D.
     * @param event A mouse move event to deal with
     * @param view The view that will be affected by the tool
     */
    void mouseMoveEvent3D(const GraphicsSceneMoveEvent* event, GraphicsView* view)  override;

    /**
     * @brief mouseRelease3D - Implements mouse release events in 3D.
     * @param event A mouse press event to deal with
     * @param view The view that will be affected by the tool
     */
    void mouseReleaseEvent3D(const GraphicsScenePressEvent* event, GraphicsView* view)  override;

    /**
     * @brief wheelEvent2D - Implements wheel 2D events.
     * @param event A wheel event to deal with
     * @param view The view that will be affected by the tool
     */
    void wheelEvent2D(const GraphicsSceneWheelEvent* event , GraphicsView* view )  override;

    /**
     * @brief wheelEvent3D - Implements wheel events.
     * @param event A wheel event to deal with
     * @param view The view that will be affected by the tool
     */
    void wheelEvent3D(const GraphicsSceneWheelEvent* event , GraphicsView* view)  override;

    /**
     * @brief keyPressEvent3D - Implements key events.
     * @param event A key event to deal with
     * @param view The view that will be affected by the tool
     */
    void keyPressEvent3D(const GraphicsSceneKeyEvent *event, GraphicsView *view) override;

    /**
     * @brief keyPressEvent2D - Implements key events.
     * @param event A key event to deal with
     * @param view The view that will be affected by the tool
     */
    void keyPressEvent2D(const GraphicsSceneKeyEvent *event, GraphicsView *view) override;

    /**
     * @brief name - Return the tool's name
     * @return The tool's name
     */

    std::string name() override;

    /**
     * @brief hasSupport3D - Returns if a tool is supported in 2D.
     * @return true or false.
     */
    bool hasSupport2D() const override;

    /**
     * @brief hasSupport3D - Returns if a tool is supported in 3D.
     * @return true or false.
     */
    bool hasSupport3D() const override;

private:
    /**
     * @brief _isLeftMouse2DPressed - Store the state of mouse left button
     */
    bool _isLeftMouseButtonPressed {false};

    /**
     * @brief _isLeftMouse2DPressed - Store the state of mouse left button
     */
    bool _isRightMousePressed {false};

    /**
     * @brief _scene - Pointer used to insert the zoom rectangle into the scene.
     */
    GraphicsScene* _scene;

    /**
     * @brief _oldPoint - Last mouse position.
     */
    Point2Df _lastPosition;
};
}
