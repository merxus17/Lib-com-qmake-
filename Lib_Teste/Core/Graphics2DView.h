#pragma once
#include <QMatrix4x4>
#include <vector>

#include "GraphicsView.h"
#include "../Events/EventConstants.h"

namespace rm
{
class Graphics2DView : public GraphicsView
{

     Q_OBJECT
public:
    /**
     * It allows GraphicsScene to access private information. This is important to allow GraphicsSceneto create and
     * destroy views.
     */
    friend class GraphicsScene;

    /**
     * It allows QObject to access private information. This is important to allow the QObject constructor to call
     * Graphics2DView destructor.
     */
    friend class QObject;
public:
    /**
     * @brief paintGL - This function should be called whenever you need to repain the widget.
     */
    void paintGL() override;

    /**
     * @brief resizeGL - The new size is passed in w and h. It changes the size of the widget
     * W and H in pixels.
     */
    void resizeGL(int w, int h) override;

    /**
     * @brief initializeGL - This function should set up any required OpenGL resources and state.
     */
    void initializeGL() override;

    /**
     * @brief fit - Fit all visiable objects at view.
     */
    void fit() override;

    /**
     * @brief pan - Make a pan action on canvas.
     * @param oldPoint - the old point.
     * @param newPoint - the new point.
     */
    void pan(const Point2Df& oldPoint, const Point2Df& newPoint) override;

    /**
     * @brief zoomInOut - Function that implements the Zoom in and Zoom out, changing proj matrix.
     * @param zoomfactor - the factor used to implement zoom
     */
    void zoomInOut(float zoomFactor) override;

    /**
     * @brief zoomArea - Draw a rectagle around an area that should be displayed in the view.
     * @param iniPoint - zoom area initial point
     * @param finalPoint - zoom area final point
     */
    void zoomArea(const Point2Df& iniPoint, const Point2Df& finalPoint) override;

    /**
     * @brief convertFromScreenToWorld - Convert from the canvas coordinate system (screen) to world coordinate system.
     * @param screen - point on canvas coordinate system.
     * @return - point on world coordinate system.
     */
    Point2Df convertFromScreenToWorld(const Point2Df& screen) override;

    /**
     * @brief getPixelSize - Get the current pixel size.
     * @return - the pixel size information.
     */
    const Point2Df& getPixelSize() const;

    /**
     * @brief synchronizeWith - Synchronize the current view with that view passed by parameter.
     * @param view - view to be synchronized with the current view.
     */
    void synchronizeWith(const Graphics2DView* view);

    /**
     * @brief desynchronizeWith - Desynchronize the current view with that view passed by parameter.
     * @param view - view to be synchronized with the current view.
     */
    void desynchronizeWith(const Graphics2DView* view);

private slots:
    /**
     * @brief newCameraParameters - Slot to set new camera parameters.
     * @param center - world center position.
     * @param maxCorner - pixel size.
     */
    void newCameraParameters(const Point2Df& center, const Point2Df& pixelSize);

signals:
    /**
     * @brief cameraChanged - Signal to be emitted every time that visualization space is modified.
     * @param center - world center position.
     * @param maxCorner - pixel size.
     */
    void cameraChanged(const Point2Df& center, const Point2Df& pixelSize);
private:
    /**
     * @brief Graphics2DView - constructor for a View 2D that will render all 2D items from scene.
     * @param scene - scene that must be rendered.
     * @param parent - parent widget.
     */
    Graphics2DView(GraphicsScene* scene, QWidget *parent);

    /**
     * @brief computeWorldLimits - Base on c1 and c2 compute new world limits _min and _max respecting the scene aspect
     * ratio .
     * @param c1 - First corner.
     * @param c2 - Second corner.
     */
    void computeWorldLimits(const Point2Df &c1, const Point2Df &c2);

    /**
     * @brief computePixelSize - Compute the current pixel size on screen.
     */
    void computePixelSize();

    /**
     * @brief setWoldLimits - Define new limites to world to be visualized. This visualization keep the aspect ratio.
     * c1 and c2 are two extreme point on rectangle world diagonal.
     * @param c1 - firt world corner.
     * @param c2 - second world corner.
     */
    void setWorldLimits(const Point2Df& c1, const Point2Df& c2);

    /**
     * @brief Default destructor
     */
    ~Graphics2DView() override = default;

    
private:    
    /**
     * @brief _min - The defaults left, right, top and bottom of the orthographic projection
     */
    Point2Df _min {Point2Df(-1,-1)};

    /**
     * @brief _max - The defaults left, right, top and bottom of the orthographic projection
     */
    Point2Df _max {Point2Df(1,1)};

    /**
     * @brief _minInitial - Initial min corner for computing _min and _max projection points
     */
    const Point2Df _minInitial {Point2Df(-1,-1)};

    /**
     * @brief _maxInitial - Initial max corner for computing _min and _max projection points
     */
    const Point2Df _maxInitial {Point2Df(1,1)};

    /**
     * @brief _proj - The projection matrix to be passed to the vertex shader
     */
    OpenGLMatrix _proj;

    /**
     * @brief _pixelSize - Current pixel size.
     */
    Point2Df _pixelSize;
};
}
