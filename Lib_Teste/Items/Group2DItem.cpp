#include "Group2DItem.h"
#include "../Core/CoreItems/AABB2DItem.h"

namespace rm
{
Group2DItem::Group2DItem(const std::set<Graphics2DItem*>& itens)
    : Graphics2DItem()
    , _items(itens)
{
    computeAABB();
}



Group2DItem::~Group2DItem()
{
   for ( auto item : _items )
    {
        delete item;
    }
    _items.clear();
}



void Group2DItem::computeAABB()
{
    AABB2D aabb;

    if (_items.size() == 0)
    {
        aabb = AABB2D(Point2Df(0, 0),  Point2Df(0, 0));
    }
    else
    {
        //Get the first AABB.
        auto it = _items.begin();
        aabb = (*it)->getModelMatrix().topMatrix() * (*it)->getAABB();

        //Compute the group's AABB;
        for(++it; it != _items.end(); ++it)
        {
            aabb += (*it)->getModelMatrix().topMatrix() * (*it)->getAABB();
        }
    }

    //As the AABB will be recomputed using the model, the group model is not necessary anymore.
    _modelMatrix.loadIdentity();

    //Set the new AABB.
    setAABB(aabb);

    _recomputeAABB = false;
}



void Group2DItem::initialize()
{
    for(auto item : _items)
    {
        item->initialize();
    }
}



bool Group2DItem::isIntersecting(const Point2Df &point) const
{
    if (isInsideToItemAABB(point, getPixelSize()))
    {
        for(auto item : _items)
        {
            if (item->isInsideToItemAABB(point, getPixelSize()) && item->isIntersecting(point))
            {
                return true;
            }
        }
    }
    return false;
}



void Group2DItem::render(int viewId)
{
    //As the group item has the items control, the group renders all objetcs and your selection
    //boxes.
    for(auto item : _items)
    {
        item->setPixelSize(_pixelSize);
        item->setProjectionMatrix(_proj);
        item->render(viewId);

        //Render all aabb items
        if (item->getAABB2DItem()->isVisible())
        {
            item->getAABB2DItem()->render(viewId, _proj, item->getModelMatrix(), _pixelSize);
        }
    }
}



std::set<Graphics2DItem*> Group2DItem::ungroup()
{
    setAABB(AABB2D());

    //As the group has no items, the model matrix is reset.
    _modelMatrix.loadIdentity();

    return  std::move(_items);;
}



void Group2DItem::scale(const QVector3D& s)
{
    _modelMatrix.scale(s.x(), s.y(), s.z());
    for(auto item : _items)
    {
        item->scale(s);
    }
}



void Group2DItem::translate(const QVector3D& t)
{
    _modelMatrix.translate(t.x(), t.y(), t.z());
    for(auto item : _items)
    {
        item->translate(t);
    }
}



void Group2DItem::setBrushColor(const QVector3D& color)
{
    _brushColor = color;
    for(auto item : _items)
    {
        item->setBrushColor(color);
    }
}



void Group2DItem::setPenColor(const QVector3D& color)
{
    _penColor = color;
    for(auto item : _items)
    {
        item->setPenColor(color);
    }
}



void Group2DItem::visible(bool v)
{
    _visible = v;
    for(auto item : _items)
    {
        item->visible(v);
    }
}



void Group2DItem::setLineWidth(float width)
{
    _lineWidth =  width;
    for(auto item : _items)
    {
        item->setLineWidth(width);
    }
}



void Group2DItem::rotate(const QVector3D& r, const float angle)
{
    _modelMatrix.rotate(angle, r.x(), r.y(), r.z());
    for(auto item : _items)
    {
        item->rotate(r, angle);
    }
}



void Group2DItem::setModelMatrix(const OpenGLMatrix &m)
{
    _modelMatrix = m;
    for(auto item : _items)
    {
        item->setModelMatrix(m);
    }
}



void Group2DItem::setProjectionMatrix(const OpenGLMatrix &proj)
{
    _proj = proj;
    for(auto item : _items)
    {
        item->setProjectionMatrix(proj);
    }
}



void Group2DItem::pushModelMatrix()
{
    _modelMatrix.push();
    for(auto item : _items)
    {
        item->pushModelMatrix();
    }
}



void Group2DItem::popModelMatrix()
{
    _modelMatrix.pop();
    for(auto item : _items)
    {
        item->popModelMatrix();
    }
}



void Group2DItem::multModelMatrix(const OpenGLMatrix &m)
{
    _modelMatrix.multMatrix(m);
    for(auto item : _items)
    {
        item->multModelMatrix(m);
    }
}



void Group2DItem::multLefModelMatrix(const OpenGLMatrix &m)
{
    _modelMatrix.multLeftMatrix(m);
    for(auto item : _items)
    {
        item->multLefModelMatrix(m);
    }
}



void Group2DItem::onFocus(bool onFocus)
{
    _onFocus = onFocus;
    for(auto item : _items)
    {
        item->onFocus(onFocus);
    }
}



size_t Group2DItem::size() const
{
    return _items.size();
}



bool Group2DItem::containsItem(Graphics2DItem *item) const
{
    return _items.find(item) != _items.end();
}



const std::set<Graphics2DItem *> &Group2DItem::getItems() const
{
    return _items;
}



AABB2D Group2DItem::getAABBRender(const Point2Df &pixelSize) const
{
    if (_items.size() == 0)
    {
        return AABB2D(Point2Df(0, 0),  Point2Df(0, 0));
    }

    AABB2D aabb;

    //@REVIEW - This piece of code is necessary to ensure that the group has the corrected matrix transformation and
    //aabb values. As AABB is computed using model matrix, its space is the world space. As this function is a const
    //function, the const_cast is necessary to turn around this constraint.
    {
        OpenGLMatrix* m = const_cast<OpenGLMatrix*>(&_modelMatrix);
        m->loadIdentity();
        _recomputeAABB = true;
    }

    //Get the first AABB.
    auto it = _items.begin();
    aabb = (*it)->getModelMatrix().topMatrix() * (*it)->getAABBRender(pixelSize);

    //Compute the group's AABB;
    for(++it; it != _items.end(); ++it)
    {
        aabb += (*it)->getModelMatrix().topMatrix() * (*it)->getAABBRender(pixelSize);
    }

    return aabb;
}



const AABB2D &Group2DItem::getAABB() const
{
    //Test if the curret object aabb is outdated. In case of true recompute it.
    if (_recomputeAABB)
    {
        Group2DItem* group = const_cast<Group2DItem*>(this);
        group->computeAABB();
    }
    return Graphics2DItem::getAABB();
}
}
