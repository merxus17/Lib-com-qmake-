#include "RectangleZoomItem.h"
#include "../Events/GraphicsSceneMoveEvent.h"

#include <algorithm>
#include <QOpenGLShaderProgram>

namespace rm
{
RectangleZoomItem::~RectangleZoomItem()
{
    glDeleteBuffers(1, &_vertexbuffer);

    for (auto it = _vao.begin(); it != _vao.end(); it++)
    {
        //Destroy vao.
        it->second->destroy();

        //Delete object.
        delete it->second;
    }

    _vao.clear();
    _program->release();
    _program->removeAllShaders();
    delete _program;
}



void RectangleZoomItem::setFirstPoint(const Point2Df& p)
{
    _firstPoint = p;
    fixPointOrder();
    updateVertexBuffer();
}



void RectangleZoomItem::setSecondPoint(const Point2Df& p)
{
    _secondPoint = p;
    fixPointOrder();
    updateVertexBuffer();
}



void RectangleZoomItem::fixPointOrder()
{
    float minX = std::min(_firstPoint.x(), _secondPoint.x());
    float minY = std::min(_firstPoint.y(), _secondPoint.y());
    float maxX = std::max(_firstPoint.x(), _secondPoint.x());
    float maxY = std::max(_firstPoint.y(), _secondPoint.y());

    _points[0] = Point2Df(minX, maxY);
    _points[1] = Point2Df(minX, minY);
    _points[2] = Point2Df(maxX, maxY);
    _points[3] = Point2Df(maxX, minY);
}



void RectangleZoomItem::render(int viewId)
{
    //Verify if there is a vao. If necessary create a new one.
    checkVao(viewId);
    _program->bind();

    //Define the correct vao as current.
    _vao[viewId]->bind();

    _proj.loadIdentity();
    _proj.ortho(0, _width - 1, _height - 1, 0, -1, 1);

    glUniform4f(_locations.brushColor, _brushColor.x(), _brushColor.y(), _brushColor.z(), 0.15f);
    glUniformMatrix4fv(_locations.mvp, 1, false, _proj.topMatrix().data());

    //Enable culling.
    glEnable(GL_CULL_FACE);

    //Enable the back-face culling.
    glCullFace(GL_BACK);

    //Set the front-face orientation.
    glFrontFace(GL_CW);

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisable( GL_BLEND );

    //Disable culling.
    glDisable(GL_CULL_FACE);

    _vao[viewId]->release();
    _program->release();
}



bool RectangleZoomItem::isInitialized() const
{
    return _program != nullptr;
}



void RectangleZoomItem::initialize()
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



bool RectangleZoomItem::isIntersecting(const Point2Df &) const
{
    return false;
}



const Point2Df& RectangleZoomItem::getFirstPoint() const
{
    return _firstPoint;
}



const Point2Df& RectangleZoomItem::getSecondPoint() const
{
    return _secondPoint;
}



void RectangleZoomItem::setCanvasWidth(int w)
{
    _width = w;
}



void RectangleZoomItem::setCanvasHeight(int h)
{
    _height = h;
}



void RectangleZoomItem::createProgram()
{
    //Create shader program
    _program = new QOpenGLShaderProgram();

    //Add vertex and fragment shaders to program.
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/mvp-transformation-vert");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/solid-color-frag");

    //Try to link the program.
    _program->link();

    //Save location variables.
    _locations.mvp = _program->uniformLocation("mvp");
    _locations.brushColor = _program->uniformLocation("brushColor");
}



void RectangleZoomItem::createBuffers()
{
    //Compute te number of bytes.
    int numberOfBytes = 4 * static_cast<int>(sizeof(Point2Df));

    //Create a buffer.
    glGenBuffers(1, &_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _points, GL_DYNAMIC_DRAW);
}



void RectangleZoomItem::updateVertexBuffer()
{
    if (isInitialized())
    {
        //Transfer new data to buffer.
        int numberOfBytes = 4 * static_cast<int>(sizeof(Point2Df));

        glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, numberOfBytes, &_points[0], GL_DYNAMIC_DRAW);
    }
}



void RectangleZoomItem::createVao(int id)
{
    //Create and configure a new vao.
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    //Add vertex.
    glBindBuffer(GL_ARRAY_BUFFER, _vertexbuffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //Add to vao.
    std::pair<int, QOpenGLVertexArrayObject*> newVao(id, vao);
    _vao.insert(std::move(newVao));
}



void RectangleZoomItem::checkVao(int id)
{
    if (!hasVao(id))
    {
       createVao(id);
    }
}



};
