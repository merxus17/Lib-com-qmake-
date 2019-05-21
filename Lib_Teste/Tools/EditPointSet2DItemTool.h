
#include <string>
#include <iostream>
#include <assert.h>

#include "../Core/GraphicsTool.h"
#include "../Geometry/Vector2D.h"
#include "../Core/Graphics2DView.h"
#include "../Events/GraphicsSceneKeyEvent.h"
#include "../Events/GraphicsScenePressEvent.h"
#include "../Events/GraphicsSceneMoveEvent.h"
#include "../Items/PointSet2DItem.h"


namespace rm
{
class EditPointSet2DItemTool : public GraphicsTool
{

public:

    /**
     * Constructor
     */
    EditPointSet2DItemTool(GraphicsView* view, PointSet2DItem *item);

    /**
     * @brief mousePress2D
     * @param event - mouse press event received
     * @param view - view in which the event occurred
     */
    virtual void  mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view) override;

    /**
     * @brief mouseMoveEvent2D - Define how the tool handles mouse move events in 2D.
     * @param event A mouse move event to deal with
     * @param view The view that will be affected by the tool
     */

    virtual void mouseHoverEvent2D(const GraphicsSceneHoverEvent *event, GraphicsView* view );
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
     * @brief leaveEvent - Define how the tool handles leave events in 2D.
     * @param event A graphics scene event to deal with
     * @param view The view that will be affected by the tool
     */
    virtual void leaveEvent2D(const GraphicsSceneEvent* event, GraphicsView* view);

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
    virtual bool hasSupport2D()const override;

    /**
     * @brief hasSupport2D
     * @return - boolean ,if has support true if no false
     */
    virtual bool hasSupport3D()const override;

    PointSet2DItem* getPointSet2DItem();

protected:
    /**
     * Destructor
     */
    ~EditPointSet2DItemTool() override = default;

private:

    /**
     * @brief updatePoint - Updates the last point of the point set.
     * @param worldPos -  Mouse position in world coordinates
     */
    void updatePoint(const Point2Df& worldPos);

private:

    /**
     * @brief _indexPointSelected - Index of the the point of the pointSet that is being clicked
     */
    unsigned int _indexPointSelected{PointSet2DItem::NO_INTERSECTS};

    /**
     * @brief _item - The item that is being edited
     */
    PointSet2DItem* _item;

    /**
     * @brief _scene - Scene of the view that called the tool
     */
    GraphicsScene* _scene;

};
}
