#include "PointSet2DItem.h"
#include "Polyline2DItem.h"
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <algorithm>
namespace rm
{

PointSet2DItem::PointSet2DItem() : Graphics2DItem()
{

}



PointSet2DItem::~PointSet2DItem()
{
    glDeleteBuffers(1, &_vertexBuffer);

    for (auto it = _vao.begin(); it != _vao.end(); it++)
    {
        //Destroy vao.
        it->second->destroy();

        //Delete object.
        delete it->second;
    }

    _vao.clear();
    _squareProgram->release();
    _squareProgram->removeAllShaders();
    delete _squareProgram;

    _circleProgram->release();
    _circleProgram->removeAllShaders();
    delete _circleProgram;

    _triangleProgram->release();
    _triangleProgram->removeAllShaders();
    delete _triangleProgram;
}



PointSet2DItem::PointSet2DItem(const std::vector<Point2Df>& pointSet)
    :Graphics2DItem()
    ,_pointSet(pointSet)
{
    computeAABB();
}



const std::vector<Point2Df>& PointSet2DItem::getPointSet() const
{
    return _pointSet;
}



unsigned int PointSet2DItem::getVerticesVBOId() const
{
    return  _vertexBuffer;
}



float PointSet2DItem::getWorldRadius() const
{
    Point2Df correctedPixelSize = updatePixelSize(_pixelSize);

    //Get the max pixel size direction value.
    float pixelSize = std::max(correctedPixelSize.x(), correctedPixelSize.y());

    //r = (d / 2) * _pixelSize
    return 0.5f * pixelSize * _size;
}



QOpenGLShaderProgram *PointSet2DItem::createProgram(const char *vertexSourceFile, const char *geometrySourceFile,
                                                    const char *fragmentSourceFile, LocationVariables &locations)
{
    //Create a new program.
    QOpenGLShaderProgram* program = new QOpenGLShaderProgram();

    //Add vertex and fragment shaders to program.
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vertexSourceFile);
    program->addShaderFromSourceFile(QOpenGLShader::Geometry, geometrySourceFile);
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fragmentSourceFile);


    //Try to link the program.
    program->link();

    //Save location variables.
    locations.m = program->uniformLocation("m");
    locations.vp = program->uniformLocation("vp");
    locations.radius = program->uniformLocation("r");
    locations.brushColor = program->uniformLocation("brushColor");
    locations.penColor = program->uniformLocation("penColor");
    locations.borderSize = program->uniformLocation("borderSize");
    locations.brushRatio = program->uniformLocation("brushRatio"); //TODO: remover


    return program;
}



void PointSet2DItem::createPrograms()
{
    _squareProgram = createProgram(":/shaders/no-transformation-vert",
                                   ":/shaders/quad-generator-geom",
                                   ":/shaders/bordered-square-frag",
                                   _squareLocations);
    _circleProgram = createProgram(":/shaders/no-transformation-vert",
                                   ":/shaders/quad-generator-geom",
                                   ":/shaders/bordered-circle-frag",
                                   _circleLocations);
    _triangleProgram = createProgram(":/shaders/no-transformation-vert",
                                     ":/shaders/triangle-generator-geom",
                                     ":/shaders/bordered-triangle-frag",
                                     _triangleLocations);
}



void PointSet2DItem::createBuffers()
{
    //Compute te number of bytes.
    int size = static_cast<int>(_pointSet.size());
    int numberOfBytes = size * static_cast<int>(sizeof(Point2Df));

    //Create a buffer.
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _pointSet.data(), GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
}



void PointSet2DItem::updateVertexBuffer()
{
    if (isInitialized())
    {
        //Transfer new data to buffer.
        int size = static_cast<int>(_pointSet.size());
        int numberOfBytes = size * static_cast<int>(sizeof(Point2Df));

        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,  numberOfBytes, _pointSet.data(), GL_DYNAMIC_DRAW);
    }
}



float PointSet2DItem::getPointSize() const
{
    return _size;
}



float PointSet2DItem::getBorderSize() const
{
    return _borderSize;
}



void PointSet2DItem::setPointSize(float size)
{
    _size = size;
}



void PointSet2DItem::createVao(int id)
{
    //Create and configure a new vao.
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    //Add vertex.
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //Add to vao.
    std::pair<int, QOpenGLVertexArrayObject*> newVao(id, vao);
    _vao.insert(std::move(newVao));
}



void PointSet2DItem::checkVao(int id)
{
    if (!hasVao(id))
    {
       createVao(id);
    }
}



void PointSet2DItem::computeAABB()
{
    if (_pointSet.size() == 0)
        return;

    Point2Df minCorner = getAABB().getMinCornerPoint();
    Point2Df maxCorner = getAABB().getMaxCornerPoint();

    minCorner = _pointSet[0];
    maxCorner = _pointSet[0];
    for (unsigned int i = 1; i < _pointSet.size(); i++)
    {
        minCorner[0] = std::min(minCorner.x(), _pointSet[i].x());
        minCorner[1] = std::min(minCorner.y(), _pointSet[i].y());

        maxCorner[0] = std::max(maxCorner.x(), _pointSet[i].x());
        maxCorner[1] = std::max(maxCorner.y(), _pointSet[i].y());
    }

    //Set the new AABB.
    setAABB({minCorner, maxCorner});
}



void PointSet2DItem::updateAABB(const Point2Df& p)
{
    Point2Df minCorner = getAABB().getMinCornerPoint();
    Point2Df maxCorner = getAABB().getMaxCornerPoint();

    if (_pointSet.size() == 1)
    {
        minCorner = p;
        maxCorner = p;
    }

    minCorner[0] = std::min(minCorner.x(), p.x());
    minCorner[1] = std::min(minCorner.y(), p.y());

    maxCorner[0] = std::max(maxCorner.x(), p.x());
    maxCorner[1] = std::max(maxCorner.y(), p.y());

    //Set the new AABB.
    setAABB({minCorner, maxCorner});
}



void PointSet2DItem::render(int viewId)
{
    //Verify if there is a vao. If necessary create a new one.
    checkVao(viewId);

    //Save the right variable locations.
    LocationVariables *currentLocation = nullptr;

    switch (_layoutType)
    {
        case LayoutType::Circle:
        {
            currentLocation = &_circleLocations;
            _circleProgram->bind();
            break;
        }
        case LayoutType::Square:
        {
            currentLocation = &_squareLocations;
            _squareProgram->bind();
            break;
        }

        case LayoutType::Triangle:
        {
            currentLocation = &_triangleLocations;
            _triangleProgram->bind();
            break;
        }
    }

    //Define the correct vao as current.
    _vao[viewId]->bind();

    glUniform4f(currentLocation->brushColor, _brushColor.x(), _brushColor.y(), _brushColor.z(), _brushColor.w());
    if (!_onFocus)
    {
        glUniform4f(currentLocation->penColor, _penColor.x(), _penColor.y(), _penColor.z(), _penColor.w());
    }
    else
    {
        glUniform4f(currentLocation->penColor, 0.5f, 0.5f, 0.5f, 1);
    }

    //Compute real radius value
    float rx = 0.5f * _pixelSize.x() * _size;
    float ry = 0.5f * _pixelSize.y() * _size;

    //Compute the brush ratio
    float bx = 1.0f - (_pixelSize.x() * _borderSize) / rx;
    float by = 1.0f - (_pixelSize.y() * _borderSize) / ry;

    glUniform2f(currentLocation->radius, rx, ry);
    glUniform2f(currentLocation->brushRatio, bx, by);

    //Set transformations
    glUniformMatrix4fv(currentLocation->vp, 1, false, _proj.topMatrix().data());
    glUniformMatrix4fv(currentLocation->m, 1, false, _modelMatrix.topMatrix().data());

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(_pointSet.size()));

    glDisable( GL_BLEND );

    _vao[viewId]->release();
    _squareProgram->release();
}



bool PointSet2DItem::isInitialized() const
{
    return _squareProgram != nullptr;
}



unsigned int PointSet2DItem::add(const Point2Df& p)
{
    //Compute the inverse matrix.
    QMatrix4x4 inverse = _modelMatrix.topMatrix().inverted();

    //Compute the new points.
    QVector4D worldPoint = inverse * QVector4D(p.x(), p.y(), 0.0f, 1.0f);

    //Get the transformed point.
    Point2Df point(worldPoint.x(), worldPoint.y());

    //Insert a new point at the end of the vector.
    _pointSet.push_back(point);

    //Update VBO of vertices.
    updateVertexBuffer();

    //Update AABB.
    updateAABB(p);

    return static_cast<unsigned int>(_pointSet.size());
}



int PointSet2DItem::insert(unsigned int pos, const Point2Df& p)
{
    //Compute the inverse matrix.
    QMatrix4x4 inverse = _modelMatrix.topMatrix().inverted();

    //Compute the new points.
    QVector4D worldPoint = inverse * QVector4D(p.x(), p.y(), 0.0f, 1.0f);

    //Get the transformed point.
    Point2Df point(worldPoint.x(), worldPoint.y());

    //Insert a new point on vector.
    _pointSet.insert(_pointSet.begin() + pos, point);
    int size = static_cast<int>(_pointSet.size());

    //Update VBO of vertices.
    updateVertexBuffer();

    //Update AABB.
    updateAABB(p);

    return size;
}



void PointSet2DItem::pointMove(unsigned int pointId, const Point2Df& delta)
{
    if (pointId < _pointSet.size())
    {
        _pointSet[pointId] += delta;

        //Update VBO of vertices.
        updateVertexBuffer();

        //Recompute AABB.
        computeAABB();
    }
}



void PointSet2DItem::setNewPointSet(const std::vector<Point2Df>& pointSet)
{
    //Save the new point set.
    _pointSet = pointSet;

    //Update VBO of vertices.
    updateVertexBuffer();

    //Recompute AABB.
    computeAABB();
}



void PointSet2DItem::setPoint(unsigned int pointId, const Point2Df &p)
{
    if (pointId < _pointSet.size())
    {
        //Compute the inverse matrix.
        QMatrix4x4 inverse = _modelMatrix.topMatrix().inverted();

        //Compute the new points.
        QVector4D worldPoint = inverse * QVector4D(p.x(), p.y(), 0.0f, 1.0f);

        //Get the transformed point.
        Point2Df point(worldPoint.x(), worldPoint.y());

        _pointSet[pointId] = point;

        //Update VBO of vertices.
        updateVertexBuffer();

        //Update AABB.
        computeAABB();
    }
}



void PointSet2DItem::setBorderSize(float size)
{
    if(size >= 0)
    {
        _borderSize = size;
    }
}



void PointSet2DItem::hardTranslate(float dx, float dy)
{
    Point2Df delta(dx, dy);
    for (Point2Df& p : _pointSet)
    {
        p += delta;
    }

    //Update VBO of vertices.
    updateVertexBuffer();

    //Translate AABB.
    Point2Df minCorner = getAABB().getMinCornerPoint();
    Point2Df maxCorner = getAABB().getMaxCornerPoint();

    minCorner += delta;
    maxCorner += delta;

    //Set the new AABB.
    setAABB({minCorner, maxCorner});
}



void PointSet2DItem::remove(unsigned int pointID)
{
    if (pointID < _pointSet.size())
    {
        _pointSet.erase(_pointSet.begin() + pointID);

        //Update VBO of vertices.
        updateVertexBuffer();

        //Recompute AABB.
        computeAABB();
    }
}



unsigned int PointSet2DItem::intersects(const Point2Df& inputPoint, unsigned int begin, unsigned int end) const
{
    if(end == static_cast<unsigned int>(-1) || begin > end)
    {
        end = size();
    }

    float minDistance = 1e+10;

    //Compute the inverse matrix.
    QMatrix4x4 inverse = _modelMatrix.topMatrix().inverted();

    //Compute the new points.
    QVector4D worldPoint = inverse * QVector4D(inputPoint.x(), inputPoint.y(), 0, 1);

    //Get the transformed point.
    Point2Df p(worldPoint.x(), worldPoint.y());

    //Intersected point index.
    unsigned int idx = NO_INTERSECTS;
    for (unsigned int i = begin; i < end; i++)
    {
        //Compute the displacement vector.
        Point2Df v = _pointSet[i] - p;

        //Get the square vector length.
        float dist2 =  v.sqrNorm();
        if ( dist2 < minDistance )
        {
            minDistance = dist2;
            idx = i;
        }
    }

    //Compute the tolerance.
    float r = getWorldRadius();

    if (minDistance <= r * r)
    {
        return idx;
    }
    return NO_INTERSECTS;
}



void PointSet2DItem::initialize()
{
    if (!isInitialized())
    {
        initializeOpenGLFunctions();

        //Create an OpenGL program to render this item.
        createPrograms();

        //Define the program as corrente. glUseProgram().
        _squareProgram->bind();

        createBuffers();
    }
}



bool PointSet2DItem::isIntersecting(const Point2Df& inputPoint) const
{
    float minDistance = 1e+10;

    //Compute the inverse matrix.
    QMatrix4x4 inverse = _modelMatrix.topMatrix().inverted();

    //Compute the new points.
    QVector4D worldPoint = inverse * QVector4D(inputPoint.x(), inputPoint.y(), 0, 1);

    //Get the transformed point.
    Point2Df p(worldPoint.x(), worldPoint.y());

    for (const Point2Df& point : _pointSet)
    {
        Point2Df v = point - p;
        float dist2 =  v * v;
        if ( dist2 < minDistance )
        {
            minDistance = dist2;
        }
    }

    //Compute the tolerance.
    float r = getWorldRadius();
    return minDistance <= r * r;
}



void PointSet2DItem::setLayout(LayoutType type)
{
    _layoutType = type;
}



PointSet2DItem::LayoutType PointSet2DItem::getLayout()
{
    return _layoutType;
}



unsigned int PointSet2DItem::size() const
{
    return static_cast<unsigned int>(_pointSet.size());
}



AABB2D PointSet2DItem::getAABBRender(const Point2Df &pixelSize) const
{
    Point2Df fixedPixelSize = updatePixelSize(pixelSize);
    Point2Df diff = fixedPixelSize * (_size * 0.5f);

    return AABB2D(getAABB().getMinCornerPoint() - diff,
                  getAABB().getMaxCornerPoint() + diff);
}
}
