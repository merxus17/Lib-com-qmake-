#include "../Core/GraphicsTool.h"
#include "../Geometry/Vector2D.h"
namespace rm
{
class PointSet2DItem;
class Graphics2DView;
class GraphicsSceneKeyEvent;
class GraphicsScenePressEvent;
class GraphicsSceneMoveEvent;
class GraphicsScene;

class CreatePointSet2DItemTool : public GraphicsTool
{
public:
	/**
	 * Constructor 
	 */
    CreatePointSet2DItemTool(GraphicsView* view);

    /**
     * Destructor
     */
    ~CreatePointSet2DItemTool() override = default;

    /**
     * @brief mouseReleaseEvent2D - Define how the tool handles mouse release events in 2D.
     * @param event A mouse press event to deal with
     * @param view The view that will be affected by the tool
     */
    virtual void mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view) override;

    /**
     * @brief mouseMoveEvent2D - Define how the tool handles mouse move events in 2D.
     * @param event A mouse move event to deal with
     * @param view The view that will be affected by the tool
     */
    virtual void mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView *view) override;

    /**
     * @brief mouseHoverEvent2D - Define how the tool handles mouse hover events in 2D.
     * @param event A mouse hover event to deal with
     * @param view The view that will be affected by the tool
     */
    virtual void mouseHoverEvent2D(const GraphicsSceneHoverEvent* event, GraphicsView* view) override;

    /**
     * @brief leaveEvent - Define how the tool handles leave events in 2D.
     * @param event - A graphics scene event to deal with
     * @param view - The view that will be affected by the tool
     */
    virtual void leaveEvent2D(const GraphicsSceneEvent* event, GraphicsView* view) override;

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

    /**
     * @brief getPointSet2DItem - Get the new point set item. Warning: the point set is created just when the tool is
     * initialized.
     * @return - new point set item.
     */
    PointSet2DItem* getPointSet2DItem();    

private:

    /**
     * @brief updatePoint - Updates the last point of the point set.
     * @param worldPos
     */
    void updatePoint(const Point2Df& worldPos);

private:
    /**
     * @brief _item - item created by the class
     */
    PointSet2DItem* _item = {nullptr};

    /**
     * @brief _scene - scene of the view that called the tool
     */
    GraphicsScene* _scene = {nullptr};
};
}
