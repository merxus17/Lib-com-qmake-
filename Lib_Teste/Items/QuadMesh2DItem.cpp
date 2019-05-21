#include "QuadMesh2DItem.h"
#include "Polyline2DItem.h"
#include "../Utility/WireframeTextureBuilder.h"

namespace rm
{

QuadMesh2DItem::QuadMesh2DItem(std::vector<unsigned int>& mesh, std::vector<Point2Df>& points)
    : Graphics2DItem()
    , _mesh(mesh)
    , _points(points)
{
    quadToTriangleMesh();
    computeAABB();
}



QuadMesh2DItem::~QuadMesh2DItem()
{
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);

    for (auto it = _vao.begin(); it != _vao.end(); it++)
    {
        //Destroy vao.
        it->second->destroy();

        //Delete object.
        delete it->second;
    }

    _vao.clear();

    delete _program;
    _program = nullptr;
}



void QuadMesh2DItem::createVao(int id)
{
    //Create and configure a new vao.
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    //Add VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //Add elements.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    //Add to vao.
    std::pair<int, QOpenGLVertexArrayObject*> newVao(id, vao);
    _vao.insert(std::move(newVao));
}



void QuadMesh2DItem::checkVao(int id)
{
    if (!hasVao(id))
    {
       createVao(id);
    }
}



void QuadMesh2DItem::createProgram()
{
    //Create shader program.
    _program = new QOpenGLShaderProgram();

    //Add vertex and fragment shaders to program.
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/mvp-transformation-vert");
    _program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/wired-solid-color-uv-geom");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/wired-solid-color-uv-frag");


    //Try to link the program.
    _program->link();

    //Get variable locations.
    _locations.brushColor = _program->uniformLocation("brushColor");
    _locations.penColor = _program->uniformLocation("penColor");
    _locations.mvp = _program->uniformLocation("mvp");
    _locations.wireframe = _program->uniformLocation("wireframe");
}



void QuadMesh2DItem::createBuffers()
{
    //Create vertex buffer.
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(_points.size()*sizeof(Point2Df)), _points.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //Create element buffer
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(_mesh.size()*sizeof(unsigned int)), _mesh.data(), GL_STATIC_DRAW);
}



void QuadMesh2DItem::createWireframeTexture()
{
    _wireframeTexture = buildWireframeTexture(_wireframeLineThickness);
}



void QuadMesh2DItem::initialize()
{
    if (!isInitialized())
    {
        initializeOpenGLFunctions();

        //Create an OpenGL program to render this item.
        createProgram();

        //Create resources.
        createBuffers();

        //Create wireframe texture.
        createWireframeTexture();
    }
}



bool QuadMesh2DItem::isInitialized()
{
    return _program != nullptr;
}



void QuadMesh2DItem::render(int viewId)
{
    //Verify if there is a vao. If necessary create a new one.
    checkVao(viewId);

    _program->bind();

    //Define the correct vao as current.
    _vao[viewId]->bind();

    glUniform4f(_locations.brushColor, _brushColor.x(), _brushColor.y(), _brushColor.z(), 1.0f);
    glUniform4f(_locations.penColor, _penColor.x(), _penColor.y(), _penColor.z(), 1.0f);
    glUniformMatrix4fv(_locations.mvp, 1, false, (_proj.topMatrix() * _modelMatrix.topMatrix()).data());
    glUniform1i( _locations.wireframe, 0 );

    //Enable culling.
    glEnable(GL_CULL_FACE);

    //Enable the back-face culling.
    glCullFace(GL_BACK);

    //Set the front-face orientation.
    glFrontFace(GL_CCW);

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnable( GL_TEXTURE_1D );
    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_1D, _wireframeTexture );

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_mesh.size()), GL_UNSIGNED_INT, nullptr);

    glDisable( GL_BLEND );

    //Disable culling.
    glDisable(GL_CULL_FACE);

    _vao[viewId]->release();
    _program->release();
}



void QuadMesh2DItem::computeAABB()
{
    if (_points.size() == 0)
        return;

    Point2Df minCorner = getAABB().getMinCornerPoint();
    Point2Df maxCorner = getAABB().getMaxCornerPoint();

    minCorner = _points[0];
    maxCorner = _points[0];
    for (unsigned int i = 1; i < _points.size(); i++)
    {
        minCorner[0] = std::min(minCorner.x(), _points[i].x());
        minCorner[1] = std::min(minCorner.y(), _points[i].y());

        maxCorner[0] = std::max(maxCorner.x(), _points[i].x());
        maxCorner[1] = std::max(maxCorner.y(), _points[i].y());
    }

    //Set the new AABB.
    setAABB({minCorner, maxCorner});
}



float QuadMesh2DItem::getWireframeLineThickness()
{
    return _wireframeLineThickness;
}



void QuadMesh2DItem::setWireframeLineThickNess(float thickness)
{
     _wireframeLineThickness = thickness;
}



bool QuadMesh2DItem::isIntersecting(const Point2Df&) const
{
    return false;
}



int QuadMesh2DItem::cellSelect(const Point2Df&)
{
    return 0;
}



void QuadMesh2DItem::updateVertexBuffer()
{
    if (isInitialized())
    {
        //Transfer new data to buffer.
        int size = static_cast<int>(_points.size());
        int numberOfBytes = size * static_cast<int>(sizeof(Point2Df));

        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER,  numberOfBytes, _points.data(), GL_STATIC_DRAW);
    }
}



void QuadMesh2DItem::quadToTriangleMesh()
{
    std::vector<unsigned int> triangleMesh;

    unsigned int numberofQuads = static_cast<unsigned int>(_mesh.size() / 4);

    //Every four elements it's a quadrilateral element
    for(unsigned int i = 0; i < numberofQuads; i++)
    {
        unsigned int v0 = _mesh[4 * i];
        unsigned int v1 = _mesh[4 * i + 1];
        unsigned int v2 = _mesh[4 * i + 2];
        unsigned int v3 = _mesh[4 * i + 3];

        //first triangle from quadrilateral element
        triangleMesh.push_back(v0);
        triangleMesh.push_back(v1);
        triangleMesh.push_back(v3);

        //second triangle from quadrilateral element
        triangleMesh.push_back(v2);
        triangleMesh.push_back(v3);
        triangleMesh.push_back(v1);
    }

    _mesh = triangleMesh;
}
};
