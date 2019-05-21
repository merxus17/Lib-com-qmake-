
#include <string>
#include <iostream>
#include <assert.h>
#include "../Items/Polyline2DItem.h"
#include "../Core/GraphicsTool.h"
#include "../Geometry/Vector2D.h"
#include "../Core/Graphics2DView.h"
#include "../Events/GraphicsSceneKeyEvent.h"
#include "../Events/GraphicsScenePressEvent.h"
#include "../Events/GraphicsSceneMoveEvent.h"



namespace rm
{
class CreatePolyline2DItemTool : public GraphicsTool
{
public:
    /**
     * @brief getPolylineItem - Get the new polyline item. Warning: the polyline is created just when the tool is
     * initialized.
     * @return - new polyline item.
     */
    Polyline2DItem* getPolylineItem();

	/**
	 * Constructor 
	 */
	CreatePolyline2DItemTool(GraphicsView* view);

    /**
     * @brief mouseRelease2D
     * @param event - mouse release event received
     * @param view - view in which the event occurred
     */
    virtual void mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view) override;

    /**
     * @brief mouseMove2D
     * @param event - mouse move event received
     * @param view - view in which the event occurred
     */
    virtual void mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView* view) override;

    /**
     * @brief mouseHoverEvent2D
     * @param event - mouse hover event received
     * @param view - view in which the event occurred
     */
    virtual void mouseHoverEvent2D(const GraphicsSceneHoverEvent *event, GraphicsView* view) override;

    /**
     * @brief leaveEvent - Define how the tool handles leave events in 2D.
     * @param event - A graphics scene event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void leaveEvent2D(const GraphicsSceneEvent* event, GraphicsView* view) override;

    /**
     * @brief initialize - Prepare tool to be used.
     */
    void initialize() override;

    /**
     * @brief finalize - Finalize tool and remove previews.
     */
    void finalize() override;

    /**
     * @brief name - Return the tool's name
     * @return The tool's name
     */

    std::string name() override;

    /**
     * @brief hasSupport2D
     * @return - boolean ,if has support true if no false
     */
    bool hasSupport2D() const override;

    /**
     * @brief hasSupport2D
     * @return - boolean ,if has support true if no false
     */
    bool hasSupport3D() const override;

    /**
     * Destructor
     */
    ~CreatePolyline2DItemTool() override = default;

private:
    /**
     * @brief CreatePointSet2DItemTool::updatePoint'
     * @param worldPos
     */
    void updatePoint(const Point2Df& worldPos);

    /**
     * @brief closePolylinePreview - If it is near enough atracts the preview point to the first polyline point.
     * @param p - Current preview point.
     */
    void closePolylinePreview(const Point2Df& p);
private:

    /**
     * @brief _item - item created by the class
     */
    Polyline2DItem* _item = {nullptr};

    /**
     * @brief _scene - scene of the view that called the tool
     */
    GraphicsScene* _scene = {nullptr};

    /**
     * @brief _isFinalizedTool - Determine if a tool if finalized or not. When finalized no events is processed.
     */
    bool _isToolFinalized = {false};
};
}

