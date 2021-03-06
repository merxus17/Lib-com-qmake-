#include <QOpenGLShaderProgram>
#include <vector>
#include <iostream>

#include "QuadMesh3DItem.h"
#include "../Utility/WireframeTextureBuilder.h"

namespace rm
{
QuadMesh3DItem::QuadMesh3DItem(std::vector<unsigned int>& mesh, std::vector<Point3Df>& points)
    :_mesh(mesh)
    , _points(points)
{
    //Must to be executed in this order.
    computeNormals();
    quadToTriangleMesh();
    computeAABB();
}



QuadMesh3DItem::~QuadMesh3DItem()
{
    if (isInitialized())
    {
        glDeleteBuffers(1, &_vertexBuffer);
        glDeleteBuffers(1, &_normalsBuffer);
        glDeleteBuffers(1, &_elementBuffer);

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
}



void QuadMesh3DItem::createVao(int id)
{
    //Create and configure a new vao.
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    //Add vertex.
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //Add normals.
    glBindBuffer(GL_ARRAY_BUFFER, _normalsBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    //Add elements.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);

    //Add to vao.
    std::pair<int, QOpenGLVertexArrayObject*> newVao(id, vao);
    _vao.insert(std::move(newVao));
}



void QuadMesh3DItem::checkVao(int id)
{
    if (!hasVao(id))
    {
       createVao(id);
    }
}



void QuadMesh3DItem::computeAABB()
{
    if (_points.size() == 0)
        return;

    QVector3D &minCorner = _aabb.getMinCornerPoint();
    QVector3D &maxCorner = _aabb.getMaxCornerPoint();

    minCorner = _points[0];
    maxCorner = _points[0];
    for (unsigned int i = 1; i < _points.size(); i++)
    {
        minCorner[0] = std::min(minCorner[0], _points[i][0]);
        minCorner[1] = std::min(minCorner[1], _points[i][1]);
        minCorner[2] = std::min(minCorner[2], _points[i][2]);

        maxCorner[0] = std::max(maxCorner[0], _points[i][0]);
        maxCorner[1] = std::max(maxCorner[1], _points[i][1]);
        maxCorner[2] = std::max(maxCorner[2], _points[i][2]);
    }
}



void QuadMesh3DItem::createPrograms()
{
}



void QuadMesh3DItem::createBuffers()
{
    //Compute the number of bytes.
    int size = static_cast<int>(_points.size());
    int numberOfBytes = size * static_cast<int>(sizeof(Point3Df));

    //Create vertex buffer.
    glGenBuffers(1, &_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _points.data(), GL_STATIC_DRAW);

    //Create normal buffer.
    glGenBuffers(1, &_normalsBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _normalsBuffer);
    glBufferData(GL_ARRAY_BUFFER, numberOfBytes, _normals.data(), GL_STATIC_DRAW);

    //Create element buffer.
    size = static_cast<int>(_mesh.size());
    numberOfBytes = size * static_cast<int>(sizeof(unsigned int));

    glGenBuffers(1, &_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberOfBytes, _mesh.data(), GL_STATIC_DRAW);
}



void QuadMesh3DItem::updateVertexBuffer()
{
    if (isInitialized())
    {
        //Recompute normals
        computeNormals();

        //Transfer new data to buffer.
        int size = static_cast<int>(_points.size());
        int numberOfBytes = size * static_cast<int>(sizeof(Point3Df));

        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER,  numberOfBytes, _points.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, _normalsBuffer);
        glBufferData(GL_ARRAY_BUFFER,  numberOfBytes, _normals.data(), GL_STATIC_DRAW);
    }
}



void QuadMesh3DItem::createProgram()
{
	//Create shader program.
	_program = new QOpenGLShaderProgram();

    //Add vertex and fragment shaders to program.
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/phong-vert");
    _program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/wired-phong-uv-geom");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/wired-phong-uv-frag");

	//Try to link the program.
	_program->link();

	//Get variable locations.
    _locations.ambientLight = _program->uniformLocation("ambientLight");
    _locations.diffuseLight = _program->uniformLocation("diffuseLight");
    _locations.ambientMaterial = _program->uniformLocation("ambientMaterial");
    _locations.diffuseMaterial = _program->uniformLocation("diffuseMaterial");
    _locations.penColor = _program->uniformLocation("penColor");
    _locations.mv = _program->uniformLocation("mv");
    _locations.lpos = _program->uniformLocation("lpos");
	_locations.normalMatrix = _program->uniformLocation("nm");
	_locations.mvp = _program->uniformLocation("mvp");
    _locations.wireframe = _program->uniformLocation("wireframe");
}



void QuadMesh3DItem::createWireframeTexture()
{
    _wireframeTexture = buildWireframeTexture(_wireframeLineThickness);
}



void QuadMesh3DItem::computeNormals()
{
    _normals.resize(_points.size(), QVector3D(0, 0, 0));
    for (unsigned int t = 0; t < _mesh.size() / 4; t++)
    {
       //Get the triangle vertices
       unsigned int v0 = _mesh[4 * t + 0];
       unsigned int v1 = _mesh[4 * t + 1];
       unsigned int v2 = _mesh[4 * t + 2];
       unsigned int v3 = _mesh[4 * t + 3];
       const Point3Df& p0 = _points[v0];
       const Point3Df& p1 = _points[v1];
       const Point3Df& p2 = _points[v2];
       const Point3Df& p3 = _points[v3];
       Point3Df n1 = Point3Df::crossProduct(p1 - p0, p3 - p0);
       Point3Df n2 = Point3Df::crossProduct(p3 - p1, p1 - p2);
       Point3Df n = (n1 + n2)/2;
       _normals[v0] += n;
       _normals[v1] += n;
       _normals[v2] += n;
       _normals[v3] += n;
    }
    //Normalize each normal
    for (auto& normal : _normals)
    {
       normal = normal.normalized();
    }
}



void QuadMesh3DItem::initialize()
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



bool QuadMesh3DItem::isInitialized()
{
    return _program != nullptr;
}



void QuadMesh3DItem::render(int viewId)
{
    //Verify if there is a vao. If necessary create a new one.
    checkVao(viewId);

    _program->bind();

    //Define the correct vao as current.
    _vao[viewId]->bind();

    _viewMatrix.push();
    OpenGLMatrix mv = _viewMatrix.multMatrix(_modelMatrix);
    _viewMatrix.pop();

    _proj.push();
    OpenGLMatrix mvp = _proj .multMatrix(mv);
    _proj.pop();

    //Get the light source.
    LightSource s = _shadingModel.getLightSource(0);
    const Color& ambient = s.getAmbientComponent();
    const Color& diffuse = s.getDiffuseComponent();

    glUniform3f(_locations.ambientMaterial, 0.1f * _brushColor.x(), 0.1f * _brushColor.y(), 0.1f * _brushColor.z());
    glUniform3f(_locations.diffuseMaterial, 0.9f * _brushColor.x(), 0.9f * _brushColor.y(), 0.9f * _brushColor.z());
    glUniform3f(_locations.ambientLight, ambient.red() / 255.0f, ambient.green() / 255.0f, ambient.blue() / 255.0f);
    glUniform3f(_locations.diffuseLight, diffuse.red() / 255.0f, diffuse.green() / 255.0f, diffuse.blue() / 255.0f);
    glUniform4f(_locations.lpos, s.getLightPosition().x(), s.getLightPosition().y(), s.getLightPosition().z(),
                s.getLightPosition().w());

    glUniform4f(_locations.penColor, _penColor.x(), _penColor.y(), _penColor.z(), 1.0f);

    glUniformMatrix4fv(_locations.mv, 1, false, mv.topMatrix().data());
    glUniformMatrix4fv(_locations.mvp, 1, false, mvp.topMatrix().data());
    glUniformMatrix3fv(_locations.normalMatrix, 1, false, mv.topMatrix().normalMatrix().data());
    glUniform1i(_locations.wireframe, 0);

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_1D);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_1D, _wireframeTexture);

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_mesh.size()), GL_UNSIGNED_INT, nullptr);

    glDisable(GL_TEXTURE_1D);
    glDisable(GL_BLEND);


    _vao[viewId]->release();
    _program->release();
}



bool QuadMesh3DItem::isIntersecting(const Point2Df& ) const
{
    return false;
}



int QuadMesh3DItem::cellSelect(const Point2Df& )
{
    return 0;
}



float QuadMesh3DItem::getWireframeLineThickness()
{
    return _wireframeLineThickness;
}



void QuadMesh3DItem::setWireframeLineThickNess(float thickness)
{
    _wireframeLineThickness = thickness;
}



void QuadMesh3DItem::quadToTriangleMesh()
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

        //First triangle from quadrilateral element
        triangleMesh.push_back(v0);
        triangleMesh.push_back(v1);
        triangleMesh.push_back(v3);

        //Second triangle from quadrilateral element
        triangleMesh.push_back(v2);
        triangleMesh.push_back(v3);
        triangleMesh.push_back(v1);
    }

    _mesh = triangleMesh;
}
};
