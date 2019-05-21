#include "Group3DItem.h"
#include <QVector3D>

namespace rm
{
Group3DItem::Group3DItem(const std::set<Graphics3DItem*>& items)
    : Graphics3DItem()
    , _items(items)
{
    computeAABB();
}



Group3DItem::~Group3DItem()
{
    for ( auto item : _items )
    {
        delete item;
    }
    _items.clear();
}



void Group3DItem::computeAABB()
{
    if (_items.size() == 0)
    {
        _aabb = AABB3D(QVector3D(0, 0, 0), QVector3D(0, 0, 0));
        return;
    }

    //Get the first AABB.
    _aabb = (*_items.begin())->getAABB();

    //COmpute the group's AABB;
    for(auto item : _items)
    {
        _aabb += item->getAABB();
    }
}

void Group3DItem::initialize()
{
    for(auto item : _items)
    {
        item->initialize();
    }
}



void Group3DItem::render(int viewId)
{
    for(auto item : _items)
    {
        item->render(viewId);
    }
}



bool Group3DItem::isIntersecting(const Point2Df& ) const
{
    return false;
}



std::set<Graphics3DItem*> Group3DItem::ungroup()
{
    return std::move(_items);
}



void Group3DItem::scale(const QVector3D& s)
{
    _modelMatrix.scale(s.x(), s.y(), s.z());
    for(auto item : _items)
    {
        item->scale(s);
    }
}



void Group3DItem::translate(const QVector3D& t)
{
    _modelMatrix.translate(t.x(), t.y(), t.z());
    for(auto item : _items)
    {
        item->translate(t);
    }
}



void Group3DItem::setBrushColor(const QVector3D& color)
{
    _brushColor = color;
    for(auto item : _items)
    {
        item->setBrushColor(color);
    }
}



void Group3DItem::setPenColor(const QVector3D& color)
{
    _penColor = color;
    for(auto item : _items)
    {
        item->setPenColor(color);
    }
}



void Group3DItem::visible(bool v)
{
    _visible = v;
    for(auto item : _items)
    {
        item->visible(v);
    }
}



void Group3DItem::setLineWidth(float width)
{
    _lineWidth = width;
    for(auto item : _items)
    {
        item->setLineWidth(width);
    }
}



void Group3DItem::rotate(const QVector3D& r, const float angle)
{
    _modelMatrix.rotate(angle, r.x(), r.y(), r.z());
    for(auto item : _items)
    {
        item->rotate(r, angle);
    }
}


void Group3DItem::setModelMatrix(const OpenGLMatrix &m)
{
    _modelMatrix = m;
    for(auto item : _items)
    {
        item->setModelMatrix(m);
    }
}



void Group3DItem::onFocus(bool onFocus)
{
    _onFocus = onFocus;
    for(auto item : _items)
    {
        item->onFocus(onFocus);
    }
}



size_t Group3DItem::getSize()
{
    return _items.size();
}
}
