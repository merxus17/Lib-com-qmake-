#include "Polyline2DItem.h"
#include <QOpenGLShaderProgram>


namespace rm
{
Polyline2DItem::Polyline2DItem():Graphics2DItem()
{
    _previewPoint.add(Point2Df(0, 0));
    _previewPoint.visible(false);
    _previewPoint.setBrushColor(QVector3D(0.5, 0, 0.5));
    _pointSetItem.setBrushColor(QVector3D(0, 0.5, 0));
}



Polyline2DItem::Polyline2DItem(const std::vector<Point2Df>& p, bool isClosed)
    :_pointSetItem(p),
    _isClosed(isClosed)
{
    _previewPoint.add(Point2Df(0, 0));
    _previewPoint.visible(false);
    _previewPoint.setBrushColor(QVector3D(0.5, 0, 0.5));
    _pointSetItem.setBrushColor(QVector3D(0, 0.5, 0));
}



Polyline2DItem::~Polyline2DItem()
{
    for (auto it = _vao.begin(); it != _vao.end(); it++)
    {
        //Destroi vao.
        it->second->destroy();

        //Delete object.
        delete it->second;
    }

    _vao.clear();
    _program->release();
    _program->removeAllShaders();
    delete _program;
}



void Polyline2DItem::createProgram()
{
    //Create shader program.
    _program = new QOpenGLShaderProgram();

    //Add shaders to program.
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/no-transformation-vert");
    _program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/rectangle-generator-geom");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/bordered-line-frag");

    //Try to link the program.
    _program->link();

    //Get variable locations.
    _locations.brushColor = _program->uniformLocation("brushColor");
    _locations.penColor = _program->uniformLocation("penColor");
    _locations.brushRatio = _program->uniformLocation("brushRatio"); //TODO: remover
    _locations.radius = _program->uniformLocation("r");
    _locations.capStyle = _program->uniformLocation("capStyle");
    _locations.vp = _program->uniformLocation("vp");
    _locations.m = _program->uniformLocation("m");
}



void Polyline2DItem::createBuffers()
{
    glBindBuffer(GL_ARRAY_BUFFER, _pointSetItem.getVerticesVBOId());
    glEnableVertexAttribArray(0);
}



void Polyline2DItem::createVao(int id)
{
    //Create and configure a new vao.
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    //Add vertex.
    glBindBuffer(GL_ARRAY_BUFFER, _pointSetItem.getVerticesVBOId());
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //Add to vao.
    std::pair<int, QOpenGLVertexArrayObject*> newVao(id, vao);
    _vao.insert(std::move(newVao));
}



void Polyline2DItem::checkVao(int id)
{
    if (!hasVao(id))
    {
        createVao(id);
    }
}



void Polyline2DItem::render(int viewId)
{
    //Verify if there is a vao. If necessary create a new one.
    checkVao(viewId);

    //Define the program as current.
    _program->bind();

    //Define the correct vao as current.
    _vao[viewId]->bind();

    glUniform4f(_locations.brushColor, _brushColor.x(), _brushColor.y(), _brushColor.z(), 1);

    if (!_onFocus)
    {
        glUniform4f(_locations.penColor, _penColor.x(), _penColor.y(), _penColor.z(), 1);
    }
    else
    {
        glUniform4f(_locations.penColor, 0.5f, 0.5f, 0.5f, 1);
    }

    //Compute the vp matriz.
    OpenGLMatrix& vp = _proj;
    glUniformMatrix4fv(_locations.vp, 1, false, vp.topMatrix().data());
    glUniformMatrix4fv(_locations.m, 1, false, _modelMatrix.topMatrix().data());

    //Compute real radius value
    float r = 0.5f * getWorldLineWidth();

    //Compute the brush ratio
    float b = 1.0f - (std::max(_pixelSize.x(),_pixelSize.y()) * _borderSize / r);

    glUniform1f(_locations.radius, r);
    glUniform1f(_locations.brushRatio, b);
    glUniform1i(_locations.capStyle, static_cast<int>(_capStyle));

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    if (_isClosed)
        glDrawArrays(GL_LINE_LOOP, 0, static_cast<GLsizei>(_pointSetItem.getPointSet().size()));
    else
        glDrawArrays(GL_LINE_STRIP, 0, static_cast<GLsizei>(_pointSetItem.getPointSet().size()));

     glDisable(GL_BLEND);

    _vao[viewId]->release();
    _program->release();

    //Render points using PointSetItem render method.
    _pointSetItem.onFocus(_onFocus);
    if(_pointSetItem.isVisible())
    {
        _pointSetItem.setPixelSize(_pixelSize);
        _pointSetItem.render(viewId);
    }

    if(_previewPoint.isVisible())
    {
        _previewPoint.setPixelSize(_pixelSize);
        _previewPoint.render(viewId);
    }
}



const AABB2D& Polyline2DItem::getAABB() const
{
    return _pointSetItem.getAABB();
}



AABB2D Polyline2DItem::getAABBRender(const Point2Df& pixelSize) const
{
    Point2Df fixedPixelSize = updatePixelSize(pixelSize);
    //Size of the point
    Point2Df diff = _pointSetItem.getPointSize() * fixedPixelSize * 0.5f;

    constexpr float SQRT2 = 1.41421356237309504880f;

    if(_capStyle == PenCapStyle::Square)
    {
        //Max between the size of the line (square cap) and the point
        diff.setX(std::max(_lineWidth * fixedPixelSize.x() * 0.5f * SQRT2, diff.x()));
        diff.setY(std::max(_lineWidth * fixedPixelSize.y() * 0.5f * SQRT2, diff.y()));
    }
    else
    {
        //Max between the size of the line (not square cap) and the point
        diff.setX(std::max(_lineWidth * fixedPixelSize.x() * 0.5f, diff.x()));
        diff.setY(std::max(_lineWidth * fixedPixelSize.y() * 0.5f, diff.y()));
    }

    return AABB2D(getAABB().getMinCornerPoint() - diff,
                  getAABB().getMaxCornerPoint() + diff);
}



AABB2DItem* Polyline2DItem::getAABB2DItem()
{
    return _pointSetItem.getAABB2DItem();
}



 const float& Polyline2DItem::getBorderSize() const
{
    return _borderSize;
}



void Polyline2DItem::setBorderSize(float size)
{
    if(size >= 0)
    {
        _borderSize = size;
    }
}



bool Polyline2DItem::isInitialized() const
{
    return _program != nullptr;
}



unsigned int Polyline2DItem::add(const Point2Df& p)
{
    return _pointSetItem.add(p);
}



int Polyline2DItem::addInPlace(const Point2Df& p, unsigned int index)
{
    //Get the segment points.
    const std::vector<Point2Df>& pointSet = _pointSetItem.getPointSet();
    const Point2Df& p1 = pointSet[index];
    const Point2Df& p2 = pointSet[(index + 1) % pointSet.size()];

    //Compute the inverse matrix.
    QMatrix4x4 inverse = _modelMatrix.topMatrix().inverted();

    //Compute the new points.
    QVector4D worldPoint = inverse * QVector4D(p.x(), p.y(), 0.0f, 1.0f);

    //Get the transformed point.
    Point2Df point(worldPoint.x(), worldPoint.y());

    //Project point over segment.
    Point2Df projectedPoint = Point2Df::projectPointOnSegment(point, p1, p2);

    //Compute the new points.
    QVector4D modelPoint = _modelMatrix.topMatrix() * QVector4D(projectedPoint.x(), projectedPoint.y(), 0.0f, 1.0f);

    //Get the transformed point.
    Point2Df projected(modelPoint.x(), modelPoint.y());


    return _pointSetItem.insert(index + 1, projected);
}



unsigned int Polyline2DItem::computeSegmentIntersection(const Point2Df& p) const
{
    //Get the set of point from point set item.
    const std::vector<Point2Df>& pointSet = _pointSetItem.getPointSet();

    //Get the number of point on point set.
    unsigned int n = _pointSetItem.size();

    if (n == 0)
    {
        return NO_INTERSECTS;
    }

    //Compute the corret number of segments.
    unsigned int numberSegments = n;
    if(!isClosed())
    {
        numberSegments--;
    }

    //Compute the point in world coordinates.
    QMatrix4x4 inverse = _modelMatrix.topMatrix().inverted();
    QVector4D worldPoint = inverse * QVector4D(p.x(), p.y(), 0.0f, 1.0f);
    Point2Df point(worldPoint.x(), worldPoint.y());

    //Compute the tolerance to be used.
    float tol = getLineTol(_pixelSize);

    for (unsigned int i = 0; i < numberSegments; i++)
    {
        //Compute the distance from to point to the current segment,
        float distance = Point2Df::pointSegmentDist(point, pointSet[i], pointSet[(i+1) % pointSet.size()]);

        if ( distance * distance <= tol * tol )
        {
            return i;
        }
    }

    return NO_INTERSECTS;
}



void Polyline2DItem::initialize()
{
    if (!isInitialized())
    {
        initializeOpenGLFunctions();

        //Initialize point sets.
        _pointSetItem.initialize();
        _previewPoint.initialize();

        //Create an OpenGL program to render this item.
        createProgram();

        //Define the program as corrente. glUseProgram().
        _program->bind();

        createBuffers();
    }
}



bool Polyline2DItem::isIntersecting(const Point2Df& inputPoint) const
{
    if (_pointSetItem.size() == 0)
    {
        return false;
    }

    //Test if intersects to some point.
    if (_pointSetItem.isIntersecting(inputPoint))
    {
        return true;
    }

    //Compute the inverse matrix.
    QMatrix4x4 inverse = _modelMatrix.topMatrix().inverted();

    //Compute the new points.
    QVector4D worldPoint = inverse * QVector4D(inputPoint.x(), inputPoint.y(), 0.0f, 1.0f);

    //Get the transformed point.
    Point2Df p(worldPoint.x(), worldPoint.y());

    //Get the set of points.
    const std::vector<Point2Df>& pointSet = _pointSetItem.getPointSet();

    //Compute the number of segments on polyline.
    unsigned numberSegments = _pointSetItem.size();
    if(!isClosed())
    {
        numberSegments--;
    }

    //Compute the tolerance
    float tol = getLineTol(_pixelSize);

    for (unsigned int i = 0; i < numberSegments; i++)
    {
        //Compute the distance from point to segment.
        float d = Point2Df::pointSegmentDist(p, pointSet[i], pointSet[(i+1) % pointSet.size()]);
        if ( d * d < tol * tol)
        {
            return true;
        }
    }

    return false;
}



Polyline2DItem::PenCapStyle Polyline2DItem::getPenCapStyle() const
{
    return _capStyle;
}



void Polyline2DItem::closed(bool c)
{
    _isClosed = c;
}



bool Polyline2DItem::isClosed() const
{
    return _isClosed;
}



void Polyline2DItem::setPenCapStyle(PenCapStyle c)
{
    _capStyle = c;
}



void Polyline2DItem::setModelMatrix(const OpenGLMatrix& m)
{
    _modelMatrix = m;
    _pointSetItem.setModelMatrix(m);
    _previewPoint.setModelMatrix(m);
}



void Polyline2DItem::pushModelMatrix()
{
    _modelMatrix.push();
    _pointSetItem.pushModelMatrix();
    _previewPoint.pushModelMatrix();
}



void Polyline2DItem::popModelMatrix()
{
    _modelMatrix.pop();
    _pointSetItem.popModelMatrix();
    _previewPoint.popModelMatrix();
}



void Polyline2DItem::setModelToIdentity()
{
    _modelMatrix.loadIdentity();
    _pointSetItem.setModelToIdentity();
    _previewPoint.setModelToIdentity();
}



void Polyline2DItem::multModelMatrix(const OpenGLMatrix &m)
{
    _modelMatrix.multMatrix(m);
    _pointSetItem.multModelMatrix(m);
    _previewPoint.multModelMatrix(m);
}



void Polyline2DItem::multLefModelMatrix(const OpenGLMatrix &m)
{
    _modelMatrix.multLeftMatrix(m);
    _pointSetItem.multLefModelMatrix(m);
    _previewPoint.multLefModelMatrix(m);
}



void Polyline2DItem::translate(const QVector3D& t)
{
    _modelMatrix.translate(t.x(), t.y(), t.z());
    _pointSetItem.translate(t);
    _previewPoint.translate(t);
}



void Polyline2DItem::scale(const QVector3D& s)
{
    _modelMatrix.scale(s.x(), s.y(), s.z());
    _pointSetItem.scale(s);
    _previewPoint.scale(s);
}



void Polyline2DItem::segmentMove()
{

}



void Polyline2DItem::setProjectionMatrix(const OpenGLMatrix& proj)
{
    _proj = proj;
    _pointSetItem.setProjectionMatrix(_proj);
    _previewPoint.setProjectionMatrix(_proj);
}



PointSet2DItem& Polyline2DItem::getPointSetItem()
{
    return _pointSetItem;
}



const PointSet2DItem &Polyline2DItem::getPointSetItem() const
{
    return _pointSetItem;
}



PointSet2DItem &Polyline2DItem::getPreviewPointSetItem()
{
    return _previewPoint;
}



const PointSet2DItem &Polyline2DItem::getPreviewPointSetItem() const
{
    return _previewPoint;
}



float Polyline2DItem::getWorldLineWidth() const
{
    //Consider the max pixel direction.
    return std::max(_pixelSize.x(), _pixelSize.y()) * _lineWidth;
}



float Polyline2DItem::getLineTol(const Point2Df& pixelSize) const
{
    Point2Df fixedPixelSize = updatePixelSize(pixelSize);
    return std::max(fixedPixelSize.x(), fixedPixelSize.y()) * _lineWidth;
}



void Polyline2DItem::setPreviewPoint(const Point2Df& p)
{
    _previewPoint.setPoint(0, p);
}



void Polyline2DItem::setPreviewVisibility(bool visibility)
{
    _previewPoint.visible(visibility);
}
}
