#include <string>
#include "../Core/GraphicsTool.h"
#include "../Geometry/Vector2D.h"

namespace rm
{

class Rectangle2DItem;
class GraphicsView;
class GraphicsSceneMoveEvent;
class GraphicsScene;

class CreateRectangle2DItemTool : public GraphicsTool
{
public:
    /**
     * Constructor
     */
    CreateRectangle2DItemTool(GraphicsView* view);

    /**
     * Destructor
     */
    ~CreateRectangle2DItemTool() override = default;

    /**
     * @brief getRectangle2DItem - Get the new rectangle item. Warning: the rectangle is created just when the tool is
     * initialized.
     * @return - new rectangle item.
     */
    Rectangle2DItem* getRectangle2DItem();

    /**
     * @brief mousePressEvent2D - Defines how the tool handles mouse press events in 2D.
     * @param event - A mouse press event to deal with.
     * @param view - The view that will be affected by the tool.
     */
    virtual void mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view) override;

    /**
     * @brief mouseMoveEvent2D - Define how the tool handles mouse move events in 2D.
     * @param event - A mouse move event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView *view) override;

    /**
     * @brief mouseReleaseEvent2D  - Define how the tool handles mouse release events in 2D.
     * @param event - Mouse release event received
     * @param view - View in which the event occurred
     */
    virtual void mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view) override;

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

private:
    /**
     * @brief updateRectangle - Compute new height, width and center point of the rectangle and update it.
     * @param worldPos
     */
    void updateRectangle(const Point2Df& worldPos);

private:
    /**
     * @brief _item - Item created by the class
     */
    Rectangle2DItem* _item = {nullptr};

    /**
     * @brief _scene - Scene of the view that called the tool
     */
    GraphicsScene* _scene = {nullptr};

    /**
     * @brief _previewCorner - Point of the corner of the rectangle, its the base for it construction.
     */
    Point2Df _previewCorner;

    /**
     * @brief _isFinalizedTool - Determine if a tool if finalized or not. When finalized no events is processed.
     */
    bool _isToolFinalized = {false};
};
}
