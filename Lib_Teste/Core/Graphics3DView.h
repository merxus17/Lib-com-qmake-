#pragma once

#include <vector>
#include <QVector3D>
#include <QVector2D>
#include <QQuaternion>
#include "GraphicsView.h"
#include "../Geometry/AxisAligmentBoundingBox.h"


namespace rm
{
class Graphics3DView : public GraphicsView
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

    /**
     * @brief arcballInit - Initialize arcball parameters.
     */
    void arcballInit();

    /**
     * @brief arcballRotationQuat - Apply arcball rotation.
     * @param p0 - first screen point.
     * @param p1 - last screen point
     */
    void arcballRotationQuat(const Point2Df& p1, const Point2Df& p2);

    /**
     * @brief paintGL - Method responsible for repaint the 3D view widget.
     */
    void paintGL() override;

    /**
     * @brief resizeGL - The new size is passed in w and h. It changes the size of the widget
     * W and H in pixels.
     * @param w - Width size in pixels.
     * @param h - Height size in pixels.
     */
    void resizeGL(int w, int h) override;

    /**
     * @brief initializeGL - This function should set up any required OpenGL resources and state.
     */
    void initializeGL() override;

    /**
     * @brief lookAt This function will change the camera of the class. This function must do the operation as is in this site
     * https://www.khronos.org/opengl/wiki/GluLookAt_code but the matrix operations should be implemented by us.
     * @param eye - 3D point indicating eye position
     * @param center - 3D point indicating center position
     * @param up - 3D vector indicating upVector
     */
    void lookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up);

    /**
     * @brief fit - Fit all visiable objects at view.
     */
    void fit() override;

    /**
     * @brief ortho - This should make the view have an orthogonal projection
     * @param proj Projection matrix
     */
    void ortho(const OpenGLMatrix &proj);

    /**
     * @brief perspective - This should make the view have an perspective projection
     * @param proj Projection matrix
     */
    void perspective(const OpenGLMatrix &proj);

    /**
     * @brief perspective - Changes the projection matrix
     * @param fovy
     * @param ratio
     * @param zNear
     * @param zFar
     * @return
     */
    OpenGLMatrix perspective(float fovy, float ratio, float zNear, float zFar);

    /**
     * @brief setShadingModel -  Set a new shading model attatched to view 3D.
     * @param model - a new shading model.
     */
    void setShadingModel(ShadingModel model);

    /**
     * @brief getShadingModel - Return the current Graphics3DView shading model.
     * @return - the current Graphics3DView shading model
     */
    ShadingModel getShadingModel();

    /**
     * @brief zoomInOut - Function that implements the Zoom in and Zoom out, changing camera position.
     * @param zoomfactor - the factor used to implement zoom
     * @param aabb - Bounding box given as the center. Direction of zoom in/out
     */
    void zoomInOut(float zoomfactor, const AABB3D &aabb);

    /**
     * @brief zoomArea - Draw a rectagle around an area that should be displayed in the view.
     * @param p1 - Zoom area initial point.
     * @param p2 - Zoom area final point.
     */
    void zoomArea(const Point2Df& p1, const Point2Df& p2) override;

    /**
     * @brief pan - Make a pan action on canvas.
     * @param oldPoint - the old point.
     * @param newPoint - the new point.
     */
    void pan(const Point2Df& oldPoint, const Point2Df& newPoint) override;

    /**
     * @brief convertFromScreenToWorld - Convert from the canvas coordinate system (screen) to world coordinate system.
     * @param screen - point on canvas coordinate system.
     * @return - point on world coordinate system.
     */
    Point2Df convertFromScreenToWorld(const Point2Df& screen) override;

    /**
     * @brief viewFrom - This is an equivalent of lookAt at view3D, we are keeping this in case view2D has an equal or similar lookAt
     * function
     */
    void viewFrom(QVector3D point);

    /**
     * @brief setSceneModelToIdentity - Set the scene model to the idendity matrix
     */
    void setSceneModelToIdentity();

    /**
     * @brief synchronizeWith - Synchronize the current view with that view passed by parameter.
     * @param view - view to be synchronized with the current view.
     */
    void synchronizeWith(const Graphics3DView* view);

    /**
     * @brief desynchronizeWith - Desynchronize the current view with that view passed by parameter.
     * @param view - view to be synchronized with the current view.
     */
    void desynchronizeWith(const Graphics3DView* view);

    struct Camera
    {
           /**
             * @brief eye - Position of the eye.
             */
           QVector3D eye = QVector3D(0,0,0);

           /**
            * @brief center - Point that the camera points to.
            */
           QVector3D center = QVector3D(0,0,0);

           /**
            * @brief up - Orientation of the up direction of the camera.
            */
           QVector3D up = QVector3D(0,0,0);

           /**
            * @brief fovy - Field of vision of the camera.
            */
           float fovy = 60.0f;

           /**
            * @brief zNear - Frustrum's near plane.
            */
           float zNear = 0.01f;

           /**
            * @brief zFar - Frustrum's far plane.
            */
           float zFar = 100.0f;
     };

private

slots:
    /**
     * @brief newCameraParameters - Slot to set new camera parameters.
     * @param c - Camera object
     */
    void newCameraParameters(const Camera& c, const OpenGLMatrix &sceneTransformations);

signals:
    /**
     * @brief cameraChanged- Signal to be emitted every time that visualization space is modified.
     * @param c - Camera object.
     */
    void cameraChanged(const Camera& c, const OpenGLMatrix& sceneTransformations);

protected:


private:
    /**
     * @brief Graphics3DView
     * @param scene
     * @param parent
     */
    Graphics3DView(GraphicsScene* scene, QWidget *parent);

    /**
     *
     */
    virtual ~Graphics3DView() override = default;

    /**
     * @brief computeRayDirection
     * @param screenPosition
     * @return
     */
    QVector3D computeRayDirection(const Point2Df& screenPosition);

    /**
     * @brief screenToArcSphere - covert screen coordinates in arc sphere quaternion.
     * @param screen - screen coordinates.
     * @return - arch sphere quaternion.
     */
    QQuaternion screenToArcSphere(const Point2Df& screen);

private:
    struct Arcball
    {
        Point2Df _center;
        float _radius;

        QVector4D _xe;
        QVector4D _ye;
        QVector4D _ze;
    };


private:

    /**
     * @brief _arcball - Arcball parameters.
     */
    Arcball _arcball;

    /**
     * @brief _camera - Camera parameters.
     */
    Camera _camera;

    /**
     * @brief _view - Matrix that defines the camera coordinates.
     */
    OpenGLMatrix _view;

    /**
     * @brief _sceneModel - Matrix to store the scene transformations.
     */
    OpenGLMatrix _sceneModel;

    /**
     * @brief _proj - Matrix that defines the projection used by render.
     */
    OpenGLMatrix _proj;

    /**
     * @brief _shadingModel - Current shading model used by render.
     */
    ShadingModel _shadingModel;
};
};
