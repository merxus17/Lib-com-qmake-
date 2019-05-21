#include "TriangleMesh2DItem.h"
#include "../Utility/WireframeTextureBuilder.h"
#include "Polyline2DItem.h"

namespace rm
{

TriangleMesh2DItem::TriangleMesh2DItem(std::vector<unsigned int>& mesh, std::vector<Point2Df>& points, TriangleType t)
    : Graphics2DItem()
    , _mesh(mesh)
    , _points(points)
    , _type(t)
{
    computeAABB();
}



TriangleMesh2DItem::~TriangleMesh2DItem()
{
    if (isInitialized())
    {
        glDeleteBuffers(1, &_vboPoints);
        glDeleteBuffers(1, &_ebo);
        if (_type == TriangleType::TRI6)
        {
            glDeleteBuffers(1, &_vboColor);
        }
    }

    delete _program;

    for (auto it = _vao.begin(); it != _vao.end(); it++)
    {
        //Destroy vao.
        it->second->destroy();

        //Delete object.
        delete it->second;
    }

    _vao.clear();
}



void TriangleMesh2DItem::createVao(int id)
{
    //Create and configure a new vao.
    QOpenGLVertexArrayObject *vao = new QOpenGLVertexArrayObject();
    vao->create();
    vao->bind();

    //Add VBO
    glBindBuffer(GL_ARRAY_BUFFER, _vboPoints);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    //TODO REMOVE: just for research tests.
    if (_type == TriangleType::TRI6)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vboColor);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(1);
    }

    //Add elements.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);

    //Add to vao.
    std::pair<int, QOpenGLVertexArrayObject*> newVao(id, vao);
    _vao.insert(std::move(newVao));
}



void TriangleMesh2DItem::checkVao(int id)
{
    if (!hasVao(id))
    {
       createVao(id);
    }
}



void TriangleMesh2DItem::createTri3Program()
{
    //Create shader program.
    _program = new QOpenGLShaderProgram();

    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/mvp-transformation-vert");
    _program->addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/wired-solid-color-uvw-geom");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/wired-solid-color-uvw-frag");

    //Try to link the program.
    _program->link();

    //Get variable locations.
    _locations.brushColor = _program->uniformLocation("brushColor");
    _locations.penColor = _program->uniformLocation("penColor");
    _locations.mvp = _program->uniformLocation("mvp");
    _locations.wireframe = _program->uniformLocation("wireframe");
}


void TriangleMesh2DItem::createTri6Program()
{
    //Create shader program.
    _program = new QOpenGLShaderProgram();

    //Add vertex and fragment shaders to program.
    _program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/tri6-400-vert");
    _program->addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tri6-400-tesc");
    _program->addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tri6-400-tese");
    _program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/wired-solid-color-400-frag");

    //Try to link the program.
    _program->link();

    //Get variable locations.
    _locations.brushColor = _program->uniformLocation("brushColor");
    _locations.penColor = _program->uniformLocation("penColor");
    _locations.mvp = _program->uniformLocation("mvp");
    _locations.wireframe = _program->uniformLocation("wireframe");
}



void TriangleMesh2DItem::createTri6ProgramForDebug()
{
    std::string vertexShaderSource = R"(
            #version 400 core
            layout(location = 0) in vec4 pos;
            layout(location = 1) in vec4 color;

            out vec3 posV;
            out vec4 colorV;

            void main()
            {
                posV = pos.xyz;
                colorV = color;
            }
            )";


    std::string tesselationControl = R"(
            #version 400 core
            //define o numero de vertices de saida
            layout(vertices = 1) out;

            //vetor com pontos de controle dos vertices que vieram
            //do vertexShader
            in vec3 posV[];
            in vec4 colorV[];

            struct OutputPatch
            {
                vec3 p[6];
                vec4 c[6];
                vec3 t[6];
            };

            const vec3 texCoor[6] = vec3[](vec3(1.0, 1.0, 0.0), vec3(1.0, 0.0, 1.0), vec3(0.0, 1.0, 1.0),
                                     vec3(1.0, 0.5, 0.5), vec3(0.5, 0.5, 1.0), vec3(0.5, 1.0, 0.5));

            // atributos de saida dos pontos de controle
            out patch OutputPatch outPatch;

            void main()
            {
                if (gl_InvocationID == 0)
                {
                    //copia as informacoes de saida
                    for (int i = 0; i < 6; i++)
                    {
                        outPatch.p[i] = posV[i];
                        outPatch.c[i] = colorV[i];
                        outPatch.t[i] = texCoor[i];
                    }

                    //define os niveis de subdivisao
                    gl_TessLevelInner[0] = 20;

                    gl_TessLevelOuter[0] = 10;
                    gl_TessLevelOuter[1] = 10;
                    gl_TessLevelOuter[2] = 10;
                }
            }
          )";

    std::string tesselationEvaluation = R"(
           #version 400 core

           layout(triangles, equal_spacing, ccw) in;

           uniform mat4 mvp;

           struct OutputPatch
           {
               vec3 p[6];
               vec4 c[6];
               vec3 t[6];
           };

           in patch OutputPatch outPatch;
           out vec3 texTE;
           out vec4 colorTE;
           void main()
           {
               //salva as coordenadas baricentricas do ponto
               //tePatchDistance = gl_TessCoord;
               float l0 = gl_TessCoord.x;
               float l1 = gl_TessCoord.y;
               float l2 = gl_TessCoord.z;

               float n0 = l0 * (2 * l0 - 1);
               float n1 = l1 * (2 * l1 - 1);
               float n2 = l2 * (2 * l2 - 1);
               float n3 = 4 * l0 * l1;
               float n4 = 4 * l1 * l2;
               float n5 = 4 * l2 * l0;

               //computa a posicao do novo vertice
               vec3 position = n0 * outPatch.p[0] + n1 * outPatch.p[1] + n2 * outPatch.p[2] +
                               n3 * outPatch.p[3] + n4 * outPatch.p[4] + n5 * outPatch.p[5];

               colorTE = n0 * outPatch.c[0] + n1 * outPatch.c[1] + n2 * outPatch.c[2] +
                         n3 * outPatch.c[3] + n4 * outPatch.c[4] + n5 * outPatch.c[5];

               texTE = n0 * outPatch.t[0] + n1 * outPatch.t[1] + n2 * outPatch.t[2] +
                       n3 * outPatch.t[3] + n4 * outPatch.t[4] + n5 * outPatch.t[5];

               //projeca as coordendas do vertice calculado
               gl_Position = mvp * vec4(position, 1.0f);
           }
           )";

    std::string geometryShaderSource = R"(
            #version 400 core
            layout(triangles) in;
            layout(triangle_strip, max_vertices = 3) out;

            in vec4 colorTE[];
            in vec3 texTE[];

            out vec3 uvw;
             out vec3 uvw2;
            out vec4 colorG;

            void main()
            {
                uvw = texTE[0];
                uvw2 = vec3(1, 1, 0);
                colorG = colorTE[0];
                gl_Position = gl_in[0].gl_Position;
                EmitVertex();

                uvw = texTE[1];
                uvw2 = vec3(1, 0, 1);
                colorG = colorTE[1];
                gl_Position = gl_in[1].gl_Position;
                EmitVertex();

                uvw = texTE[2];
                uvw2 = vec3(0, 1, 1);
                colorG = colorTE[2];
                gl_Position = gl_in[2].gl_Position;
                EmitVertex();

                EndPrimitive();
            }
            )";

    std::string fragmentShaderSource = R"(
            #version 400 core
            uniform vec4 brushColor;
            uniform vec4 penColor;
            uniform sampler1D wireframe;

            in vec4 colorG;
            in vec3 uvw;
            in vec3 uvw2;
            out vec4 fragmentColor;

            void main()
            {
                //Obtain the alpha from texture using mipmap.
                float alpha1 = texture( wireframe, uvw.x ).r;
                float alpha2 = texture( wireframe, uvw.y ).r;
                float alpha3 = texture( wireframe, uvw.z ).r;

                fragmentColor = colorG;

                //Apply DECAL
                fragmentColor = mix( fragmentColor, penColor, alpha1 );
                fragmentColor = mix( fragmentColor, penColor, alpha2 );
                fragmentColor = mix( fragmentColor, penColor, alpha3 );

                fragmentColor = vec4(fragmentColor.xyz, 1.0f);
            }
            )";

    //TODO REMOVE: check OpenGL version.
    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    //Create shader program.
    _program = new QOpenGLShaderProgram();

    //Add vertex and fragment shaders to program.
    if (!_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource.c_str()))
        qCritical() << QObject::tr( "Could not compile vertex shader. Log:" ) << _program->log();

    if (!_program->addShaderFromSourceCode(QOpenGLShader::TessellationControl, tesselationControl.c_str()))
        qCritical() << QObject::tr( "Could not compile TessellationControl shader. Log:" ) << _program->log();

    if (!_program->addShaderFromSourceCode(QOpenGLShader::TessellationEvaluation, tesselationEvaluation.c_str()))
        qCritical() << QObject::tr( "Could not compile TessellationEvaluation shader. Log:" ) << _program->log();

    if (!_program->addShaderFromSourceCode(QOpenGLShader::Geometry, geometryShaderSource.c_str()))
        qCritical() << QObject::tr( "Could not compile Geometry shader. Log:" ) << _program->log();

    if (!_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource.c_str()))
        qCritical() << QObject::tr( "Could not compile Fragment shader. Log:" ) << _program->log();

    //Try to link the program.
    _program->link();

    //Get variable locations.
    _locations.brushColor = _program->uniformLocation("brushColor");
    _locations.penColor = _program->uniformLocation("penColor");
    _locations.mvp = _program->uniformLocation("mvp");
    _locations.wireframe = _program->uniformLocation("wireframe");
}



void TriangleMesh2DItem::createBuffers()
{
    //Create vertex buffer.
    glGenBuffers(1, &_vboPoints);
    glBindBuffer(GL_ARRAY_BUFFER, _vboPoints);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(_points.size() * sizeof(Point2Df)), _points.data(), GL_STATIC_DRAW);

    //TODO REMOVE: just for research tests.
    if (_type == TriangleType::TRI6)
    {
        std::vector<float> color{1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
                                 1.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f};

        int numBytes = static_cast<int>(color.size() * sizeof(float));
        printf("%d\n", numBytes);
        glGenBuffers(1, &_vboColor);
        glBindBuffer(GL_ARRAY_BUFFER, _vboColor);
        glBufferData(GL_ARRAY_BUFFER, numBytes, color.data(), GL_STATIC_DRAW);
    }

    //Create element buffer
    glGenBuffers(1, &_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(_mesh.size()*sizeof(unsigned int)), _mesh.data(), GL_STATIC_DRAW);
}



void TriangleMesh2DItem::createWireframeTexture()
{
    _wireframeTexture = buildWireframeTexture(_wireframeLineThickness);
}



void TriangleMesh2DItem::initialize()
{
    if (!isInitialized())
    {
        initializeOpenGLFunctions();

        if (_type == TriangleType::TRI3)
        {
            //Create an OpenGL program to render this item.
            createTri3Program();
        }
        else if (_type == TriangleType::TRI6)
        {
            //Create an OpenGL program to render this item.
            createTri6Program();

            //createTri6ProgramForDebug();
        }

        //Create resources.
        createBuffers();

        //Create wireframe texture.
        createWireframeTexture();
    }
}



bool TriangleMesh2DItem::isInitialized()
{
    return _program != nullptr;
}



void TriangleMesh2DItem::render(int viewId)
{
    //Verify if there is a vao. If necessary create a new one.
    checkVao(viewId);
    _program->bind();

    //Define the correct vao as current.
    _vao[viewId]->bind();

    _proj.push();
    OpenGLMatrix mvp = _proj.multMatrix(_modelMatrix);
    _proj.pop();

    glUniform4f(_locations.brushColor, _brushColor.x(), _brushColor.y(), _brushColor.z(), 1.0f);
    glUniform4f(_locations.penColor, _penColor.x(), _penColor.y(), _penColor.z(), 1.0f);
    glUniformMatrix4fv(_locations.mvp, 1, false, mvp.topMatrix().data());
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

    if (_type  == TriangleType::TRI3)
    {
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(_mesh.size()), GL_UNSIGNED_INT, nullptr);
    }
    else
    {
        _program->setPatchVertexCount(6);
        glDrawElements(GL_PATCHES, static_cast<GLsizei>(_mesh.size()), GL_UNSIGNED_INT, nullptr);
    }
    glDisable( GL_BLEND );

    //Disable culling.
    glDisable(GL_CULL_FACE);

    _vao[viewId]->release();
    _program->release();
}





void TriangleMesh2DItem::computeAABB()
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



float TriangleMesh2DItem::getWireframeLineThickness()
{
    return _wireframeLineThickness;
}



void TriangleMesh2DItem::setWireframeLineThickNess(float thickness)
{
     _wireframeLineThickness = thickness;
}



bool TriangleMesh2DItem::isIntersecting(const Point2Df& ) const
{
	return false;
}



int TriangleMesh2DItem::cellSelect(const Point2Df& )
{
	return 0;
}



void TriangleMesh2DItem::updateVertexBuffer()
{
    if (isInitialized())
    {
        //Transfer new data to buffer.
        int size = static_cast<int>(_points.size());
        int numberOfBytes = size * static_cast<int>(sizeof(Point2Df));

        glBindBuffer(GL_ARRAY_BUFFER, _vboPoints);
        glBufferData(GL_ARRAY_BUFFER,  numberOfBytes, _points.data(), GL_STATIC_DRAW);
    }
}
};
