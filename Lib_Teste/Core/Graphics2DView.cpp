#include <QOpenGLFunctions>
#include "Graphics2DItem.h"
#include "../Items/SelectionGroup2DItem.h"
#include "Graphics2DView.h"
#include "../Core/CoreItems/AABB2DItem.h"

namespace rm
{
Graphics2DView::Graphics2DView(GraphicsScene* scene, QWidget* parent)
    :GraphicsView(scene, parent, ViewType::VIEW_2D)
{

}



void Graphics2DView::computeWorldLimits(const Point2Df &c1, const Point2Df &c2)
{
    //Get the min and max corner.
    Point2Df minCorner(std::min(c1.x(), c2.x()), std::min(c1.y(), c2.y()));
    Point2Df maxCorner(std::max(c1.x(), c2.x()), std::max(c1.y(), c2.y()));


    //Compute the world dimensions.
    Point2Df worldDimensions = maxCorner - minCorner;

    //Ajust aspect ratio.
    if (worldDimensions.x() / worldDimensions.y() > static_cast<float>(width()) / height())
    {
        worldDimensions[1] = worldDimensions.x() * height() / width();
    }
    else
    {
        worldDimensions[0] = worldDimensions.y() * width() / height();
    }

    //Compute the center of the canvas.
    Point2Df center = (minCorner + maxCorner) / 2;

    //Compute new min and max points e define new projection
    _min = center - 0.5f * worldDimensions;
    _max = center + 0.5f * worldDimensions;
}



const Point2Df& Graphics2DView::getPixelSize() const
{
    return _pixelSize;
}



void Graphics2DView::computePixelSize()
{
    _pixelSize[0] = (_max.x() - _min.x()) / width();
    _pixelSize[1] = (_max.y() - _min.y()) / height();
}



void Graphics2DView::setWorldLimits(const Point2Df& c1, const Point2Df& c2)
{
    //Compute new world limits.
    computeWorldLimits(c1, c2);

    //Define new projection matrix.
    _proj.loadIdentity();
    _proj.ortho( _min.x(), _max.x(), _min.y(), _max.y(), -1, 1 );

    //Recompute the pixel size.
    computePixelSize();

    //Emit signal to synchronize views.
    emit cameraChanged((_min + _max) * 0.5f, _pixelSize);
}



void Graphics2DView::newCameraParameters(const Point2Df& center, const Point2Df& pixelSize)
{
    //Compute new min and max points e define new projection
    _min[0] = center.x() - 0.5f * pixelSize.x() * width();
    _min[1] = center.y() - 0.5f * pixelSize.y() * height();

    _max[0] = center.x() + 0.5f * pixelSize.x() * width();
    _max[1] = center.y() + 0.5f * pixelSize.y() * height();

    //Define new projection matrix.
    _proj.loadIdentity();
    _proj.ortho( _min.x(), _max.x(), _min.y(), _max.y(), -1, 1 );

    //Recompute the pixel size.
    computePixelSize();
}



void Graphics2DView::synchronizeWith(const Graphics2DView* view)
{
    connect(view, &Graphics2DView::cameraChanged, this, &Graphics2DView::newCameraParameters);
}



void Graphics2DView::desynchronizeWith(const Graphics2DView *view)
{
    disconnect(view,  &Graphics2DView::cameraChanged, this, &Graphics2DView::newCameraParameters);
}



Point2Df Graphics2DView::convertFromScreenToWorld(const Point2Df& screen)
{    
    //Get parameters.
    float tx = screen.x() / (width() - 1.0f);
    float ty = 1.0f - screen.y() / (height() - 1.0f);

    Point2Df result;
    result[0] = (1.0f - tx) * _min.x() + tx * _max.x();
    result[1] = (1.0f - ty) * _min.y() + ty * _max.y();

    return result;
}



void Graphics2DView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    //Render 2d items
    const std::list<GraphicsItem *>& itemsList = _scene->items();
    for(auto item : itemsList)
    {
        Graphics2DItem* item2d = dynamic_cast<Graphics2DItem*>(item);

        if (item2d && item2d->isVisible())
        {
            item2d->setProjectionMatrix(_proj);
            item2d->setPixelSize(_pixelSize);
            item2d->render(id());
        }

    }
    //Render item's bounding boxes
    for(auto item : itemsList)
    {
        Graphics2DItem* item2d = dynamic_cast<Graphics2DItem*>(item);
        if(item2d)
        {
            //@TODO: quando mudar o render dos objetos, mudar o itemBox para const.
            AABB2DItem* itemBox = item2d->getAABB2DItem();
            if(itemBox->isVisible())
            {
                //Render item box
                itemBox->render(id(), _proj, item2d->getModelMatrix(), _pixelSize);
            }
        }
    }

    if (_rectangleZoom.isVisible())
    {
         glDisable(GL_DEPTH_TEST);
        _rectangleZoom.render(id());
    }
}



void Graphics2DView::resizeGL(int w, int h)
{
    if (h == 0 || w == 0)
    {
        return;
    }

    //Get the current viewport.
    GLint viewPort[4];
    glGetIntegerv(GL_VIEWPORT, viewPort);

    //Set a new viewport.
    glViewport(0, 0, w, h);

    float widthFactor = static_cast<float>(w) / viewPort[2];
    float heightFactor = static_cast<float>(h) / viewPort[3];

    // Computa a coordenada do centro da área visível
    Point2Df center = (_min + _max) / 2.0f;
    Point2Df increment = (_max - _min) / 2.0f;

    increment[0] *= widthFactor;
    increment[1] *= heightFactor;

    _min = center - increment;
    _max = center + increment;

    //Recompute the pixel size.
    computePixelSize();

    //Define new projection matrix.
    _proj.loadIdentity();
    _proj.ortho( _min.x(), _max.x(), _min.y(), _max.y(), -1, 1 );
}



void Graphics2DView::initializeGL()
{
    //TODO: Include background color as a GraphicsView member
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glViewport(0, 0, 100, 100);
    _rectangleZoom.initialize();
}



void Graphics2DView::fit()
{
    AABB2D aabb = _scene->computeAABB2D();
    setWorldLimits(aabb.getMinCornerPoint(), aabb.getMaxCornerPoint());
}



void Graphics2DView::zoomInOut(float zoomFactor)
{
    //Computes the center.
    Point2Df center = (_min + _max) / 2;

    //Compute the world dimensions.
    Point2Df worldDimensions = _max - _min;

    //Aplies zoom factor.
    Point2Df minCorner = center - (0.5f * zoomFactor) * worldDimensions;
    Point2Df maxCorner = center + (0.5f * zoomFactor) * worldDimensions;

    setWorldLimits(minCorner, maxCorner);
}



void Graphics2DView::zoomArea(const Point2Df &iniPoint, const Point2Df &finalPoint)
{
    setWorldLimits(iniPoint, finalPoint);
}



void Graphics2DView::pan(const Point2Df& oldPoint, const Point2Df& newPoint)
{
    //Compute the wold displacement.
    Point2Df delta = oldPoint - newPoint;
    Point2Df minCorner = _min + delta;
    Point2Df maxCorner = _max + delta;

    //Define new world limits.
    setWorldLimits(minCorner, maxCorner);
}
}
