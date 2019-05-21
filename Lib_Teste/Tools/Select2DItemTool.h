#include "../Core/GraphicsTool.h"
#include "../Geometry/Vector2D.h"
#include "../Core/Graphics2DItem.h" /* for _boxSelectedPoint */

namespace rm
{
class GraphicsScene;
class SelectionGroup2DItem;
class GraphicsSceneKeyEvent;
class GraphicsSceneMoveEvent;

class Select2DItemTool : public GraphicsTool
{
protected:
    /**
     * Destructor
     */
    ~Select2DItemTool() override;

public:
    /**
     * Constructor
     */
    Select2DItemTool(GraphicsScene* scene);

    /**
     * @brief mousePressEvent2D - Defines how the tool handles mouse press events in 2D.
     * @param event - A mouse press event to deal with.
     * @param view - The view that will be affected by the tool.
     */
    void mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view) override;

    /**
     * @brief mouseReleaseEvent2D - Defines how the tool handles mouse release events in 2D.
     * @param event - A mouse press event to deal with.
     * @param view - The view that will be affected by the tool.
     */

    void mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view) override;
    /**
     * @brief mouseMoveEvent2D - Defines how the tool handles mouse move events in 2D.
     * @param event - A mouse move event to deal with.
     * @param view - The view that will be affected by the tool.
     */
    void mouseMoveEvent2D(const GraphicsSceneMoveEvent* event,GraphicsView* view) override;

    /**
     * @brief mouseHoverEvent2D - Defines how the tool handles mouse hover events in 2D.
     * @param event - A mouse hover event to deal with.
     * @param view - The view that will be affected by the tool.
     */
    void mouseHoverEvent2D(const GraphicsSceneHoverEvent* event, GraphicsView* view) override;

    /**
     * @brief keyPress2D - Defines how the tool handles key 2D events.
     * @param event - A key press event to deal with.
     * @param view - The view that will be affected by the tool.
     */
    void keyPressEvent2D(const GraphicsSceneKeyEvent* event, GraphicsView* view) override;

    /**
     * @brief initialize - Initializes the tool, adds the element on GraphicsScene object.
     */
    void initialize() override;

    /**
     * @brief finalize - Finalizes the tool and remove the rectangle from scene.
     */
    void finalize() override;

    /**
     * @brief name - Returns the tool's name.
     * @return The tool's name.
     */
    std::string name() override;

    /**
     * @brief hasSupport2D -
     * @return - boolean ,if has support true if no false
     */
    virtual bool hasSupport2D() const override;

    /**
     * @brief hasSupport2D
     * @return - boolean ,if has support true if no false
     */
    virtual bool hasSupport3D() const override;

private:

    /**
     * @brief The InterationMode enum - Define the current iteration.
     */
    enum class InterationMode
    {
        NONE,      //No operation.
        SELECTION, //Selection operation.
        SCALE,     //Scale operation.
        ROTATE,    //Rotate operation.
        TRANSLATE  //Translate operation.
    };

private:

    /**
     * @brief checkItemIntersection - Verify if the mouse click intersects a 2D item.
     * @brief pixelSize - view pixel size information.
     * @return - the item intersected or nullptr in case of none.
     */
    Graphics2DItem* checkItemIntersection(const Point2Df& p, const Point2Df& pixelSize);

    /**
     * @brief getSelectionBoxPoint - Computes and returns the selection box point based on AABB item and the index point
     * following the bellow convention:
     *6-------7-------0
     *|               |
     *|               |
     *|               |
     *5               1
     *|               |
     *|               |
     *|               |
     *4-------3-------2
     * @param aabb - The item AABB.
     * @param p - The point index.
     * @return - The selection box point coordinates.
     */
    Point2Df getSelectionBoxPoint(const AABB2D& aabb, unsigned int p);

    /**
     * @brief showBoxItems - Sets the visibility of the items' bounding boxes.
     * @param show - True to show the boxes and false to hide them.
     */
    void showBoxItems(bool show = true);

    /**
     * @brief setupGroupBoxLayout - Configures the appearence properties of the items'group
     * bounding boxes.
     * The color of brush and pen of the lines is: (61,127,186)
     * The point size is 10;
     * The brush ratio of the points is 0.9;
     * The points of the external box are squared and white with the border in blue.
     */
    void setupGroupBoxLayout();

    /**
     * @brief setupBoxLayout - Configures the appearence properties of the items
     * bounding boxes.
     * The color of brush and pen of the lines is: (61,127,186)
     * The point size is 10;
     * The brush ratio of the points is 0.9;
     * The points of the external box are squared and white with the border in blue.
     */
    void setupBoxLayout(Graphics2DItem* item);

    /**
     * @brief applyTransformation - Applies the desired transformation m to the _groupItem
     * and its items, to its bounding box, and to the bounding boxes of its items
     * @param m - The matrix that defines the transformation
     * @param save - Defines whether the group items and boxes are going to push the states
     * of their matrix stacks.
     */
    void applyTransformation(const OpenGLMatrix& m, bool save = true);
    /**
     * @brief transformPoint - Transforms a point p by a matrix m.
     * @param p - Point to be transformed.
     * @param m - Matrix to transform the point p.
     * @return - The transformed point.
     */
    Point2Df transformPoint(const Point2Df& p, const QMatrix4x4& m) const;

    /**
     * @brief applyScale - Define a new scale.
     * @param p - Current mouse point on world coordinates.
     * @param uniformScale - define if the scale is uniform or not.
     */
    void applyScale(const Point2Df& p, bool uniformScale, bool save = true);

private:

    /**
     * @brief _scene - Pointer to scene used to add a GraphicsItem to scene items.
     */
    GraphicsScene* _scene = {nullptr};

    /**
     * @brief _groupItem - Group of selected items of the scene
     */
    SelectionGroup2DItem* _groupItem = {nullptr};

    /**
     * @brief _boxSelectedPoint - Current box selected point.
     */
    unsigned int _boxSelectedPoint = {Graphics2DItem::NO_INTERSECTS};

    /**
     * @brief _lastPoint - Store the last mouse point in world coordinates.
     */
    Point2Df _lastPoint;
    
    /**
     * @brief _interation - Define the iteration mode.
     */
    InterationMode _interation = {InterationMode::NONE};
};
}

