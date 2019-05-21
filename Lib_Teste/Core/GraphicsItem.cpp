#include "GraphicsItem.h"
#include <iostream>
#include <QOpenGLFunctions>

#include<QOpenGLShaderProgram>

namespace rm
{
bool GraphicsItem::hasVao(int id) const
{
    return _vao.find(id) != _vao.end();
}



const QVector4D &GraphicsItem::getBrushColor() const
{
    return _brushColor;
}



void GraphicsItem::setBrushColor(const QVector4D& color)
{
    _brushColor = color;
}



void GraphicsItem::setBrushColor(const QVector3D& color)
{
    _brushColor = QVector4D(color, 1.0f);
}



const QVector4D &GraphicsItem::getPenColor() const
{
    return _penColor;
}



void GraphicsItem::setPenColor(const QVector4D& color)
{
   _penColor = color;
}



void GraphicsItem::setPenColor(const QVector3D& color)
{
   _penColor = QVector4D(color, 1.0f);
}



bool GraphicsItem::isVisible() const
{
    return _visible;
}



void GraphicsItem::visible(bool visible)
{
    _visible = visible;
}



void GraphicsItem::setTexture(Texture* texture)
{
    _texture = texture;
}



float GraphicsItem::getLineWidth() const
{
    return _lineWidth;
}



void GraphicsItem::setLineWidth(float width)
{
    _lineWidth = width;
}



void GraphicsItem::setStyle(ItemStyle style)
{
    _style = style;
}



void GraphicsItem::setFont(Font* font)
{
    _font = font;
}



void GraphicsItem::setProjectionMatrix(const OpenGLMatrix& proj)
{
    _proj = proj;
}



bool GraphicsItem::isOnFocus() const
{
    return _onFocus;
}



void GraphicsItem::onFocus(bool onFocus)
{
    _onFocus = onFocus;
}



void GraphicsItem::setName(std::string name)
{
    _name = name;
}



const std::string& GraphicsItem::getName() const
{
    return _name;
}



void GraphicsItem::setModelToIdentity()
{
    _modelMatrix.loadIdentity();
}



void GraphicsItem::translate(const QVector3D& t)
{
    //_modelMatrix.translate(t);
    _modelMatrix.translate(t.x(), t.y(), t.z());
}



void GraphicsItem::scale(const QVector3D& s)
{
    //_modelMatrix.scale(s);
    _modelMatrix.scale(s.x(), s.y(), s.z());
}



void GraphicsItem::rotate(const QVector3D& r, const float angle)
{
    //_modelMatrix.rotate(angle, r);
    _modelMatrix.rotate(angle, r.x(), r.y(), r.z());
}



void GraphicsItem::setModelMatrix(const OpenGLMatrix& m)
{
    _modelMatrix = m;
}



const OpenGLMatrix& GraphicsItem::getModelMatrix() const
{
    return _modelMatrix;
}



void GraphicsItem::pushModelMatrix()
{
    _modelMatrix.push();
}



void GraphicsItem::popModelMatrix()
{
    _modelMatrix.pop();
}


void GraphicsItem::multModelMatrix(const OpenGLMatrix &m)
{
    _modelMatrix.multMatrix(m);
}



void GraphicsItem::multLefModelMatrix(const OpenGLMatrix &m)
{
    _modelMatrix.multLeftMatrix(m);
}
}
