#include "Graphics3DView.h"
#include "Graphics3DItem.h"
#include <cmath>

namespace rm
{
Graphics3DView::Graphics3DView(GraphicsScene* scene, QWidget* parent)
    : GraphicsView(scene, parent, ViewType::VIEW_3D)
{
}



void Graphics3DView::arcballInit()
{
    //Get the eye system coordinates.
    QVector3D ze = (_camera.eye - _camera.center).normalized();
    QVector3D xe = (QVector3D::crossProduct(_camera.up, ze)).normalized();
    QVector3D ye = (QVector3D::crossProduct(ze, xe)).normalized();

    int w = width();
    int h = height();
    int radius = std::max(w, h);

    //Compute initialization parameters.
    Point2Df center(w / 2.0f, h / 2.0f);

   _arcball._center = center;
   _arcball._radius = radius;
   _arcball._xe = QVector4D(xe, 1.0f);
   _arcball._ye = QVector4D(ye, 1.0f);
   _arcball._ze = QVector4D(ze, 1.0f);
}



void Graphics3DView::arcballRotationQuat(const Point2Df& p0, const Point2Df& p1)
{
    //Get references for eye coordinates.
    QVector4D& xe = _arcball._xe;
    QVector4D& ye = _arcball._ye;
    QVector4D& ze = _arcball._ze;

    QQuaternion q0 = screenToArcSphere( p0 ).conjugated();
    QQuaternion q1 = screenToArcSphere( p1 );
    QQuaternion q = q1 * q0;

    QVector3D axis = q.vector().normalized();
    float halfAngle = std::acos(q.scalar());
    axis = axis.x() * xe.toVector3D() + axis.y() * ye.toVector3D() + axis.z() * ze.toVector3D();

    float s = std::sin(halfAngle);
    axis *=  (s / axis.length());

    q = QQuaternion(std::cos(halfAngle), axis);
    q.normalize();

    //Trasform update camera parameters.
    OpenGLMatrix rotMatrix, t2, t1;
    rotMatrix.rotate(q);

    //Compute the AABB center.
    AABB3D sceneAABB = _scene->computeAABB3D();
    QVector3D center = (sceneAABB.getMinCornerPoint() + sceneAABB.getMaxCornerPoint()) / 2;

    xe = rotMatrix.topMatrix() * xe;
    ye = rotMatrix.topMatrix() * ye;
    ze = rotMatrix.topMatrix() * ze;

    t1.translate(-center.x(), -center.y(), -center.z());
    t2.translate(+center.x(), +center.y(), +center.z());

    _sceneModel.multLeftMatrix(t1);
    _sceneModel.multLeftMatrix(rotMatrix);
    _sceneModel.multLeftMatrix(t2);

    emit cameraChanged(_camera, _sceneModel);
}



QQuaternion Graphics3DView::screenToArcSphere(const Point2Df& screen)
{
    if (_arcball._radius > 0)
    {
        Point2Df v = (screen - _arcball._center) / _arcball._radius;
        float r = v * v;

        QVector3D q(v.x(), v.y(), 0.0f);

        if(r > 1.0f)
        {
            q /= std::sqrt(r);
        }
        else
        {
            q[2] = std::sqrt(1.0f - r);
        }

        return QQuaternion(0.0f, q);
    }
    return QQuaternion();
}



void Graphics3DView::paintGL()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _view.push();
    OpenGLMatrix modelview = _view.multMatrix(_sceneModel);
    _view.pop();

    //Render 3d items
    const std::list<GraphicsItem *>& itemsList = _scene->items();
    for(auto item : itemsList)
    {
        Graphics3DItem* item3d = dynamic_cast<Graphics3DItem*>(item);

        if (item3d && item3d->isVisible())
        {
            item3d->setProjectionMatrix(_proj);
            item3d->setViewMatrix(modelview);
            item3d->setShadingModel(_shadingModel);
            item3d->render(id());
        }

    }

    if (_rectangleZoom.isVisible())
    {
         glDisable(GL_DEPTH_TEST);
        _rectangleZoom.render(id());
    }
}



Point2Df Graphics3DView::convertFromScreenToWorld(const Point2Df& screen)
{
    return Point2Df(screen);
}



void Graphics3DView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    _proj.loadIdentity();
    _proj.perspective(60, static_cast<float>(w) / h, 0.1f, 10000.0f);
    emit cameraChanged(_camera, _sceneModel);
}



void Graphics3DView::initializeGL()
{
    _rectangleZoom.initialize();
}



void Graphics3DView::fit()
{
    viewFrom(_camera.eye);
    emit cameraChanged(_camera, _sceneModel);
}



void Graphics3DView::lookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up)
{
    _camera.center = center;
    _camera.eye =  eye;
    _camera.up = up;
    _view.loadIdentity();
    _view.lookAt(eye.x(), eye.y(), eye.z(),
                 center.x(), center.y(), center.z(),
                 up.x(), up.y(), up.z());
}



void Graphics3DView::ortho(const OpenGLMatrix& proj)
{
   _proj = proj;
}



void Graphics3DView::perspective(const OpenGLMatrix& proj)
{
    _proj = proj;
}



OpenGLMatrix Graphics3DView::perspective(float fovy, float ratio, float zNear, float zFar)
{
    //Just to avoid warnings.
    float x = fovy + ratio + zNear + zFar;
    x++;
    return OpenGLMatrix();
}




void Graphics3DView::viewFrom(QVector3D point)
{
    if(_scene->items().size() != 0)
    {
        constexpr double PI = 3.14159265358979323846;
        const AABB3D sceneAABB = _scene->computeAABB3D();

        const QVector3D& maxAABBCorner = sceneAABB.getMaxCornerPoint();
        const QVector3D& minAABBCorner = sceneAABB.getMinCornerPoint();

        QVector3D AABBCenter = (minAABBCorner + maxAABBCorner) / 2;

        /**
         * diagonalLenght - A diagonal da bounding box foi escolhida por ser a maior distancia possivel dentro da bounding box
         * assim garantindo que todos os objetos estão na tela .
         */
        const float diagonalLenght = maxAABBCorner.distanceToPoint(minAABBCorner);
        const float angle = _camera.fovy / 2;
        const double radAngle = static_cast<double>(angle) * PI / 180;

        /**
         *  distanceEye - a distancia que o olho deve estar do centro da bounding box é calculado através do angulo de visão da camera .
         */
        const float distanceEye = 0.5f * diagonalLenght / static_cast<float>(tan(radAngle));

        /**
         *  centerToEye - vetor apontando do centro pro olho da camera.
         */
        const QVector3D centerToEye = point - AABBCenter;

        /**
         * newEyePosition - a nova posição onde o olho deve estar para ver todos itens da scena
         */
        const QVector3D newEyePosition = AABBCenter + centerToEye.normalized() * distanceEye;

        const float dotProduct = QVector3D::dotProduct(centerToEye.normalized(), QVector3D(0.f,1.f,0.f));
        if(fabs(fabs(static_cast<double>(dotProduct)) - 1) < 10e-6)
            lookAt(newEyePosition, AABBCenter, QVector3D(1.f, 0.f, 0.f));
        else
            lookAt(newEyePosition, AABBCenter, QVector3D(0.f,1.f,0.f));     
    }
}



void Graphics3DView::pan(const Point2Df& , const Point2Df& )
{

}



void Graphics3DView::zoomInOut(float zoomFactor, const AABB3D& aabb)
{

    const QVector3D& maxAABBCorner = aabb.getMaxCornerPoint();
    const QVector3D& minAABBCorner = aabb.getMinCornerPoint();

    QVector3D newCenter = (maxAABBCorner + minAABBCorner) / 2.f;
    QVector3D newEyePosition = (newCenter - _camera.eye) * zoomFactor + _camera.eye;

    lookAt(newEyePosition, newCenter, _camera.up);

    emit cameraChanged(_camera, _sceneModel);

}



QVector3D Graphics3DView::computeRayDirection(const Point2Df& )
{
//    //Compute angle.
//    constexpr double PI = 3.14159265358979323846;
//    const float angle = _camera.fovy / 2;
//    const double radAngle = static_cast<double>(angle) * PI / 180.0;

//    //Compute auxiliar variables.
//    float df = _camera.zNear;
//    float h = 2 * df * static_cast<float>(tan(radAngle));
//    float w = h * width() / height();

//    //Compute the ray direction.
//    QVector4D direction = -_camera.zNear * _view.row(2) + h * (screenPosition.y() / height() - 0.5f) * _view.row(1)
//            + w * (screenPosition.x() / width() - 0.5f) * _view.row(0);

    return QVector3D();
}



void Graphics3DView::zoomArea(const Point2Df &c1, const Point2Df &c2)
{
    //Get the min and max corner.
    Point2Df minCorner(std::min(c1.x(), c2.x()), std::min(c1.y(), c2.y()));
    Point2Df maxCorner(std::max(c1.x(), c2.x()), std::max(c1.y(), c2.y()));

    //Compute canvas and rectangle center.
    Point2Df canvasCenter(static_cast<float>(width() - 1) / 2.0f, static_cast<float>(height() - 1) / 2.0f);
    Point2Df rectagleCenter = (minCorner + maxCorner) / 2;

    //Compute the direction to both centers.
    QVector3D canvasCenterDirection = computeRayDirection(canvasCenter);
    QVector3D rectagleCenterDirection = computeRayDirection(rectagleCenter);

    //Compute the cossine between directions.
    float cosDirections = QVector3D::dotProduct(canvasCenterDirection , rectagleCenterDirection) /
                (canvasCenterDirection.length() * rectagleCenterDirection.length());

    QVector3D newEyePosition = _camera.eye + (1 - cosDirections) * rectagleCenterDirection;
    QVector3D newCenter = _camera.eye + 2 * rectagleCenterDirection;
    lookAt(newEyePosition, newCenter, _camera.eye);

    //Get the extreme rectangle points.
//    Point2Df topMiddle(rectagleCenter.x(), maxCorner.y());
//    QVector3D topMiddleDirection = computeRayDirection(topMiddle);
//    float rectAngle = static_cast<float>(QVector3D::dotProduct(rectagleCenterDirection, topMiddleDirection));

//    float halfDistance = distanceToNewCenter / static_cast<float>(tan(rectAngle));



    //QVector4D direction = -_camera.zNear * _view.column(2) + h * ;
//    float h = 2 * tan(_camera.fovy/2) * _camera.zNear;
//    float w = (_width/_height) * h;
//    Point2Df rcenter ( (iniPoint.x()+finalPoint.x())/2,
//                       (iniPoint.y()+finalPoint.y())/2 );
//    Point2Df center( (rcenter.x() / _width  - 0.5f) * w,
//                      (0.5f - rcenter.y() / _height) * h);
//    Point2Df centerWS = convertFromScreenToWorld(center);
//    QVector3D cWS (centerWS.x(), centerWS.y(), 0);

//    QVector3D dir1 = _camera.eye * _camera.zNear;
//    QVector3D dir2 = dir1 + cWS;
//    dir2.normalize();

//    float dist = _camera.zNear * _height / (finalPoint.y()-iniPoint.y());
//    QVector3D pos2 = _camera.eye + dir2 * (dist + _camera.zNear);
//    lookAt(pos2, cWS, _camera.up);
}



void Graphics3DView::setShadingModel(ShadingModel model)
{
    _shadingModel = model;
}



ShadingModel Graphics3DView::getShadingModel()
{
    return _shadingModel;
}



void Graphics3DView::setSceneModelToIdentity()
{
    _sceneModel.loadIdentity();
}



void Graphics3DView::synchronizeWith(const Graphics3DView* view)
{
    connect(view, &Graphics3DView::cameraChanged, this, &Graphics3DView::newCameraParameters);
}



void Graphics3DView::desynchronizeWith(const Graphics3DView* view)
{
    disconnect(view, &Graphics3DView::cameraChanged, this, &Graphics3DView::newCameraParameters);
}



void Graphics3DView::newCameraParameters(const Camera& c, const OpenGLMatrix& sceneTransformations)
{
    lookAt(c.eye,c.center,c.up);
    _sceneModel = sceneTransformations;
}
}
