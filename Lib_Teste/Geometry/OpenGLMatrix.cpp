#include "OpenGLMatrix.h"
#include <cstring>
#include <cmath>
namespace rm
{

OpenGLMatrix::OpenGLMatrix( )
{
    _currentMatrix.setToIdentity();
}



float OpenGLMatrix::sX() const
{
    return _currentMatrix.column(0).length();
}



float OpenGLMatrix::sY() const
{
    return _currentMatrix.column(1).length();
}



float OpenGLMatrix::sZ() const
{
    return _currentMatrix.column(2).length();
}



QVector3D OpenGLMatrix::getScaleFactors() const
{
    return {sX(), sY(), sZ()};
}



void OpenGLMatrix::push( )
{
    _matrixStack.push( _currentMatrix );

}



void OpenGLMatrix::pop( )
{
    if (!_matrixStack.empty( ))
    {
        _currentMatrix = _matrixStack.top( );
        _matrixStack.pop( );
    }
}



void OpenGLMatrix::loadIdentity( )
{
    _currentMatrix.setToIdentity();
}



void OpenGLMatrix::loadMatrix(const QMatrix4x4 &m )
{
    _currentMatrix = m;
}



void OpenGLMatrix::translate(float dx, float dy, float dz )
{
   _currentMatrix.translate(dx, dy, dz);
}



void OpenGLMatrix::scale(float sx, float sy, float sz )
{
   _currentMatrix.scale(sx, sy, sz);
}



void OpenGLMatrix::rotate( float a, float x, float y, float z )
{
    _currentMatrix.rotate(a, x, y, z);
}



void OpenGLMatrix::rotate(const QQuaternion& q)
{
    _currentMatrix.rotate(q);
}



void OpenGLMatrix::lookAt( float eyeX, float eyeY, float eyeZ, float centerX, float centerY, float centerZ,
                                  float upX, float upY, float upZ )
{
    _currentMatrix.lookAt({eyeX, eyeY, eyeZ}, {centerX, centerY, centerZ}, {upX, upY, upZ});
}



void OpenGLMatrix::frustum( float l, float r, float b, float t, float near, float far )
{
    _currentMatrix.frustum(l, r, b, t, near, far);
}



void OpenGLMatrix::ortho( float l, float r, float b, float t, float near, float far )
{

    _currentMatrix.ortho(l, r, b, t, near, far);
}



void OpenGLMatrix::perspective( float fovY, float aspect, float zNear, float zFar )
{
    _currentMatrix.perspective(fovY, aspect, zNear, zFar);
}



const QMatrix4x4 &OpenGLMatrix::topMatrix() const
{
    return _currentMatrix;
}



const OpenGLMatrix& OpenGLMatrix::multMatrix(const OpenGLMatrix &rhs)
{
    _currentMatrix *= rhs.topMatrix();
    return *this;
}



const OpenGLMatrix& OpenGLMatrix::multLeftMatrix(const OpenGLMatrix& rhs)
{
    _currentMatrix = rhs.topMatrix() * _currentMatrix;
    return *this;
}
}
