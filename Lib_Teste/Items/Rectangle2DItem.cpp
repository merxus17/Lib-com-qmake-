#include "Rectangle2DItem.h"
#include "../Core/CoreItems/AABB2DItem.h"
#include <QOpenGLShaderProgram>


namespace rm
{

Rectangle2DItem::Rectangle2DItem()
    : _height(0.0f)
    , _width(0.0f)
{

}



Rectangle2DItem::Rectangle2DItem(const Point2Df& point, float width, float height)
    : _centerPoint(point)
    , _width(std::abs(width))
    , _height(std::abs(height))
{
    computeAABB();
}



Rectangle2DItem::~Rectangle2DItem()
{
    glDeleteBuffers(1, &_vertexBuffer);

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



void Rectangle2DItem::initialize()
{
    if (!isInitialized())
    {
        initializeOpenGLFunctions();

        //Create an OpenGL program to render this item.
        createProgram();

        //Define the program as corrente. glUseProgram().
        _program->bind();

        createBuffers();
    }
}



bool Rectangle2DItem::isInitialized() const
{
    return _program != nullptr;
}



bool Rectangle2DItem::isIntersecting(const Point2Df& inputPoint) const
{
    //Compute the inverse matrix.
    QMatrix4x4 inverse = _modelMatrix.topMatrix().inverted();

    //Compute the new points.
    QVector4D worldPoint = inverse * QVector4D(inputPoint.x(), inputPoint.y(), 0, 1);

    //Get the transformed point.
    Point2Df p(worldPoint.x(), worldPoint.y());

    return getAABB().isAABBInside(p);
}



void Rectangle2DItem::render(int viewId )
{
    //Verify if there is a vao. If necessary create a new one.
    checkVao(viewId);

    _program->bind();

    //Define the correct vao as current.
    _vao[viewId]->bind();

    glUniform4f(_locations.brushColor, _brushColor.x(), _brushColor.y(), _brushColor.z(), _brushColor.w());
    if (!_onFocus)
    {
        glUniform4f(_locations.penColor, _penColor.x(), _penColor.y(), _penColor.z(), _penColor.w());
    }
    else
    {
        glUniform4f(_locations.penColor, 0.5f, 0.5f, 0.5f, 1);
    }

    //Compute real radius value
    float rx = _width * 0.5f;
    float ry = _height * 0.5f;

    //Compute the brush ratio
    float bx = 1.0f - (_pixelSize.x() / _modelMatrix.sX() * _borderSize) / rx;
    float by = 1.0f - (_pixelSize.y() / _modelMatrix.sY() * _borderSize) / ry;

    glUniform2f(_locations.radius, rx, ry);
    glUniform2f(_locations.brushRatio, bx, by);
    glUniformMatrix4fv(_locations.vp, 1, false, _proj.topMatrix().data());
    glUniformMatrix4fv(_locations.m, 1, false, _modelMatrix.topMatrix().data());

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glDrawArrays(GL_POINTS, 0, 1);

    glDisable( GL_BLEND );

    _vao[viewId]->release();
    _program->release();
}



float Rectangle2DItem::getWidth()
{
    return _width;
}



void Rectangle2DItem::setWidth(float width)
{
    _width = std::abs(width);

    //Update AABB.
    computeAABB();
}



float Rectangle2DItem::getHeight()
{
    return _height;
}



void Rectangle2DItem::setHeight(float height)
{
    _height = std::abs(height);

    //Update AABB.
    computeAABB();
}



const Point2Df& Rectangle2DItem::getCenterPoint()
{
    return _centerPoint;
}



void Rectangle2DItem::setCenterPoint(const Point2Df& point)
{
    _centerPoint = point;

    //Update VBO of vertices.
    updateVertexBuffer();

    //Update AABB.
    computeAABB();
}



const float& Rectangle2DItem::getBorderSize() const
{
   return _borderSize;
}



void Rectangle2DItem::setBorderSize(float size)
{
    if(size >= 0)
    {
        _borderSize = size;
    }
}



void Rectangle2DItem::createVao(int id)
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



void Rectangle2DItem::checkVao(int id)
{
    if (!hasVao(id))
    {
        createVao(id);
    }
}



void Rectangle2DItem::createProgram()
{
    //Create shader program.
    _program = new QOpenGLShaderProgram();

    //Add shaders to program.
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/no-transformation-vert");
    _program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/transformable-quad-generator-geom");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/bordered-square-frag");

    //Try to link the program.
    _program->link();

    //Save location variables.
    _locations.m = _program->uniformLocation("m");
    _locations.vp = _program->uniformLocation("vp");
    _locations.radius = _program->uniformLocation("r");
    _locations.brushColor = _program->uniformLocation("brushColor");
    _locations.penColor = _program->uniformLocation("penColor");
    _locations.borderSize = _program->uniformLocation("borderSize");
    _locations.brushRatio = _program->uniformLocation("brushRatio");
}



void Rectangle2DItem::createBuffers()
{
    //Compute te number of bytes.
    int size = 1;
    int numberOfBytes = size * static_cast<int>(sizeof(Point2Df));

    //Create a buffer.
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, &_centerPoint, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
}



void Rectangle2DItem::updateVertexBuffer()
{
    if (isInitialized())
    {
        //Transfer new data to buffer.
        int size = 1;
        int numberOfBytes = size * static_cast<int>(sizeof(Point2Df));

        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,  numberOfBytes, &_centerPoint, GL_DYNAMIC_DRAW);
    }
}



void Rectangle2DItem::computeAABB()
{
    Point2Df offset(_width * 0.5f, _height * 0.5f);
    setAABB({_centerPoint - offset, _centerPoint + offset});
}



void Rectangle2DItem::updateRectangle(const Point2Df& centerPoint, float width, float height)
{
    _width = std::abs(width);
    _height = std::abs(height);
    _centerPoint = centerPoint;

    //Update VBO of vertices.
    updateVertexBuffer();

    //Update AABB.
    computeAABB();
}

}
