#include <string>
#include <iostream>
#include "../Items/Polyline2DItem.h"

#include "../Core/GraphicsTool.h"

namespace rm
{
class GraphicsScene;
class GraphicsSceneMoveEvent;
class GraphicsScenePressEvent;
class GraphicsSceneKeyEvent;
class Graphics2DView;
class EditPolyline2DItemTool : public GraphicsTool
{

public:
    /**
     * Constructor
     */
    EditPolyline2DItemTool(GraphicsView* view, Polyline2DItem* item);

    /**
     * @brief mousePress2D
     * @param event - mouse press event received
     * @param view - view in which the event occurred
     */
    virtual void  mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view) override;

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
     * @brief mouseHover2D
     * @param event - mouse hover event received
     * @param view - view in which the event occurred
     */
    virtual void mouseHoverEvent2D(const GraphicsSceneHoverEvent* event, GraphicsView* view) override;

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

    Polyline2DItem* getPolyline2DItem();

protected:
    /**
     * Destructor
     */
    ~EditPolyline2DItemTool() override;

private:
    /**
     * @brief _item - The item that is being edited
     */
    Polyline2DItem* _item;

    /**
     * @brief _scene - Save scene pointer to set focus item.
     */
    GraphicsScene* _scene;

    /**
     * @brief _indexPointSelected - Index of the the point of the pointSet that is being clicked
     */
    unsigned int _indexPointSelected{Polyline2DItem::NO_INTERSECTS};
};
}
