#include "Select2DItemTool.h"
#include "../Core/GraphicsScene.h"
#include "../Core/Graphics2DView.h"
#include "../Core/CoreItems/AABB2DItem.h"
#include "../Items/SelectionGroup2DItem.h"
#include "../Items/PointSet2DItem.h"

#include <string>
#include <QCursor>

namespace rm
{
Select2DItemTool::Select2DItemTool(GraphicsScene* scene)
    : _scene(scene)
    , _groupItem(new SelectionGroup2DItem())
{
}



Select2DItemTool::~Select2DItemTool()
{
    //Ungroup selected items and delete the group.
    _groupItem->ungroup();
    _scene->makeCurrent();
    delete _groupItem;
    _scene->doneCurrent();
}



void Select2DItemTool::showBoxItems(bool show)
{
    _groupItem->getAABB2DItem()->visible(show);
    for (auto gi : _groupItem->getItems())
    {
        gi->getAABB2DItem()->visible(show);
    }
}

void Select2DItemTool::setupGroupBoxLayout()
{
    QVector4D blue(61.f / 255, 127.f / 255, 186.f / 255, 1.0f);
    AABB2DItem* groupItemSelectionBox = _groupItem->getAABB2DItem();

    setupBoxLayout(_groupItem);
    PointSet2DItem& boxPointSetItem = groupItemSelectionBox->getPointSetItem();
    PointSet2DItem& previewPointSetItem = groupItemSelectionBox->getPreviewPointSetItem();
    groupItemSelectionBox->setBrushColor(blue);
    groupItemSelectionBox->setPenColor(blue);
    groupItemSelectionBox->showHandles(true);

    boxPointSetItem.setLayout(PointSet2DItem::LayoutType::Square);
    boxPointSetItem.setPenColor(blue);
    boxPointSetItem.setBrushColor(QVector4D(1, 1, 1, 1));
    boxPointSetItem.setBorderSize(1.0f);
    boxPointSetItem.setPointSize(10.0f);

    previewPointSetItem.setBrushColor(blue);
    previewPointSetItem.setPenColor(blue);
    previewPointSetItem.setPointSize(10);

}



void Select2DItemTool::setupBoxLayout(Graphics2DItem *item)
{
    AABB2DItem* aabbItem = item->getAABB2DItem();
    QVector4D color(61.f / 255, 127.f / 255, 186.f / 255, 1.0f);
    aabbItem->visible(false);
    aabbItem->setLineWidth(1);
    aabbItem->showHandles(false);
    aabbItem->setBrushColor(color);
    aabbItem->setPenColor(color);

    aabbItem->getPointSetItem().setBrushColor(QVector4D(1, 1, 1, 1));
    aabbItem->getPointSetItem().setPenColor(color);
    aabbItem->getPointSetItem().setBorderSize(1.0f);

    aabbItem->getPreviewPointSetItem().setBrushColor(color);
    aabbItem->getPreviewPointSetItem().setPenColor(color);
    aabbItem->getPreviewPointSetItem().setPointSize(10);
    aabbItem->getPreviewPointSetItem().visible(false);
    aabbItem->getPreviewPointSetItem().setLayout(PointSet2DItem::LayoutType::Square);
}



void Select2DItemTool::applyTransformation(const rm::OpenGLMatrix& m, bool save)
{
    _groupItem->popModelMatrix();
    if(save)
    {
        _groupItem->pushModelMatrix();
    }

    //Apply transformation on the group item and automatically on its children
    _groupItem->multLefModelMatrix(m);
}



Point2Df Select2DItemTool::transformPoint(const Point2Df& p, const QMatrix4x4& m) const
{
    //Transform the point by matrix.
    QVector4D t = m * QVector4D(p.x(), p.y(), 0.0f, 1.0f);
    return Point2Df(t.x(), t.y());
}



void Select2DItemTool::applyScale(const Point2Df& p, bool uniformScale, bool)
{
    //Points order pattern.
    //Opposite vertex to vertex i => o = (i + 4) % 8;
    //6-------7-------0
    //|               |
    //|               |
    //|               |
    //5               1
    //|               |
    //|               |
    //|               |
    //4-------3-------2

    //Get the opposite point index.
    unsigned int oppositePoint = (_boxSelectedPoint + 4) % 8;

    //Get the opposite point coordinates.
    Point2Df fixedPoint = getSelectionBoxPoint(_groupItem->getAABB(), oppositePoint);
    fixedPoint = transformPoint(fixedPoint, _groupItem->getModelMatrix().topMatrix());

    Point2Df deltaP = p - fixedPoint;
    Point2Df deltaLast = _lastPoint - fixedPoint;

    float sx = 1.0f;
    float sy = 1.0f;

    float eps = 1e-6f;
    float signX = std::signbit(deltaLast.x()) ? -1.f : 1.f;
    float signY = std::signbit(deltaLast.y()) ? -1.f : 1.f;

    if (std::fabs(deltaLast.x()) < eps)
    {
        deltaLast[0] = signX * eps;
    }

    if (std::fabs(deltaLast.y()) < eps)
    {
        deltaLast[1] = signY * eps;
    }

    //Compute the scale factors.
    sx = deltaP.x() / deltaLast.x();
    sy = deltaP.y() / deltaLast.y();

    //Active uniform scale just in case of corner vertex edition.
    if (_boxSelectedPoint % 2 == 0)
    {
        //Define equal scale factor in case of uniform scale.
        //Select the minimum scale factor for both directions.
        if (uniformScale)
        {
            if (sx < sy)
            {
                sy = sx;
            }
            else
            {
                sx = sy;
            }
        }
    }
    //If edit point in x direction, set the scale in y direction as constant.
    else if ((_boxSelectedPoint - 1) % 4 == 0)
    {
        sy = 1;
    }
    //If edit point in y direction, set th scale in x direction as constant.
    else if ((_boxSelectedPoint + 1) % 4 == 0)
    {
        sx = 1;
    }

    OpenGLMatrix m;
    m.translate(fixedPoint.x(), fixedPoint.y(), 0);
    m.scale(sx, sy, 1);
    m.translate(-fixedPoint.x(), -fixedPoint.y(), 0);

    //Apply transformation on the group item and automatically on its children
    _groupItem->multLefModelMatrix(m);
}



Graphics2DItem *Select2DItemTool::checkItemIntersection(const Point2Df& p, const Point2Df& pixelSize)
{
    //Render 2d items
    const std::list<GraphicsItem *>& itemsList = _scene->items();
    for(auto item : itemsList)
    {
        Graphics2DItem* item2d = dynamic_cast<Graphics2DItem*>(item);

        if (item2d != nullptr && item2d != _groupItem)
        {
            //Define the pixel size information.
            item2d->setPixelSize(pixelSize);

            //Check of intersect with the item.
            if (item2d->isInsideToItemAABB(p, pixelSize) && item2d->isIntersecting(p))
            {
                return item2d;
            }
        }
    }

    return nullptr;
}



Point2Df Select2DItemTool::getSelectionBoxPoint(const AABB2D &aabb, unsigned int p)
{
    //Get aabb information.
    const Point2Df& minCorner = aabb.getMinCornerPoint();
    const Point2Df& maxCorner = aabb.getMaxCornerPoint();

    //We assume that the point is the max and fix the coordinates if it is necessary.
    Point2Df point = maxCorner;

    //6-------7-------0
    //|               |
    //|               |
    //|               |
    //5               1
    //|               |
    //|               |
    //|               |
    //4-------3-------2

    //Determin x.
    switch (p)
    {
    case 3:
    case 7:
        point[0] = (maxCorner.x() + minCorner.x()) / 2.0f;
        break;
    case 4:
    case 5:
    case 6:
        point[0] = minCorner.x();
        break;
    }

    //Determin y.
    switch (p)
    {
    case 1:
    case 5:
        point[1] = (maxCorner.y() + minCorner.y()) / 2.0f;
        break;
    case 2:
    case 3:
    case 4:
        point[1] = minCorner.y();
        break;
    }

    return point;
}



void Select2DItemTool::mousePressEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view)
{
    if(event->isLeftButtonPressed())
    {
        //Get the 2D view.
        Graphics2DView* view2D = dynamic_cast<Graphics2DView*>(view);

        //Current pixel size.
        const Point2Df& pixelSize = view2D->getPixelSize();

        //Test the intersection between the point and the box selection points.
        const Point2Df& p = event->getWorldPosition();

        //For multiple selection
        bool ctrlKey = event->isControlKeyPressed();

        //Multiple selection has precedence over selection box.
        if(ctrlKey)
        {
            _interation = InterationMode::NONE;
            showBoxItems(false);
        }

        if (_groupItem->getAABB2DItem()->isVisible())
        {
            //Define the current pixel size.
            _groupItem->setPixelSize(pixelSize);

            //Verify if intersects with the selection box.
            _boxSelectedPoint = _groupItem->getAABB2DItem()->getPointSetItem().intersects(p);

            //Check if intersects with a selection box point -> scale
            if (_boxSelectedPoint != GraphicsItem::NO_INTERSECTS)
            {
                 _interation = InterationMode::SCALE;
            }
            //Verify if intersects with the interior box -> translate.
            else if (_groupItem->isInsideToItemAABB(p, view2D->getPixelSize()))
            {
                _interation = InterationMode::TRANSLATE;
                view->setCursor(Qt::ClosedHandCursor);
            }
            //Do not intersects the box.
            else
            {
                _interation = InterationMode::NONE;
                showBoxItems(false);
                _groupItem->setModelToIdentity();
            }

            _lastPoint = p;
        }

        if (_interation == InterationMode::NONE)
        {
            Graphics2DItem* item = nullptr;
            if((item = checkItemIntersection(p, pixelSize)) != nullptr)
            {
                if(ctrlKey)
                {
                    if(_groupItem->containsItem(item))
                    {
                        _groupItem->removeItem(item);
                    }
                    else
                    {
                        _groupItem->addItem(item);
                        setupBoxLayout(item);
                    }
                }
                else
                {
                    _groupItem->ungroup();
                    _groupItem->addItem(item);
                    setupBoxLayout(item);
                }

                if(_groupItem->size() > 0)
                {

                    showBoxItems(true);

                    _interation = InterationMode::SELECTION;
                    view->setCursor(Qt::OpenHandCursor);
                }
            }
            else
            {
                _groupItem->ungroup();
            }
        }
    }
}



void Select2DItemTool::mouseMoveEvent2D(const GraphicsSceneMoveEvent* event, GraphicsView*)
{
    //Get the current point in world coordinates.
    const Point2Df& p = event->getWorldPosition();

    if (_interation == InterationMode::TRANSLATE)
    {
        //Define the new translation.
        //Compute the translation.
        Point2Df t = p - _lastPoint;

         //Set the new model on object.
        OpenGLMatrix m;
        m.translate(t.x(), t.y(), 0);

        applyTransformation(m, true);
    }
    else if (_interation == InterationMode::SCALE)
    {
        _groupItem->popModelMatrix();
        _groupItem->pushModelMatrix();
        applyScale(p, event->isShiftKeyPressed());
    }
}


void Select2DItemTool::mouseReleaseEvent2D(const GraphicsScenePressEvent* event, GraphicsView* view )
{
    //Get the current point in world coordinates.
    const Point2Df& p = event->getWorldPosition();

    if (_interation == InterationMode::TRANSLATE)
    {
        //Define the final translation.
        //Compute the translation.
        Point2Df t = p - _lastPoint;
        OpenGLMatrix m;
        m.translate(t.x(), t.y(), 0);

        applyTransformation(m, false);

        //Reset the interation mode.
        _interation = InterationMode::SELECTION;

        view->setCursor(Qt::OpenHandCursor);
    }
    else if (_interation == InterationMode::SCALE)
    {
        _groupItem->popModelMatrix();

        //Define the final scale
        applyScale(p, event->isShiftKeyPressed(), false);

        //Reset the interation mode.
        _interation = InterationMode::SELECTION;
    }
}



void Select2DItemTool::keyPressEvent2D(const GraphicsSceneKeyEvent *event, GraphicsView*)
{
    if(event->isControlKeyPressed() && _groupItem->size() != 0)
    {
        //Group selected items
        if(event->getKey() == Qt::Key_G)
        {
            //Get and remove the selected items from this tool
            std::set<Graphics2DItem*> items = _groupItem->ungroup();

            const std::list<GraphicsItem*>& sceneItems = _scene->items();
            std::list<GraphicsItem*>::const_iterator iteratorPosition = _scene->items().end();

            //Search for an item from the group being disolved in the furtherest position of scene list
            for(auto sceneItem = sceneItems.begin(); sceneItem != sceneItems.end();)
            {
                Graphics2DItem* item2d = dynamic_cast<Graphics2DItem*>(*sceneItem);
                if(item2d != nullptr && items.find(item2d) != items.end())
                {
                    sceneItem = _scene->removeItem(item2d);
                    iteratorPosition = sceneItem;
                }
                else
                {
                    sceneItem++;
                }
            }

            //Add the new item as selected.
            for (auto gi : items)
            {
                gi->getAABB2DItem()->visible(false);
            }

            Group2DItem* newGroup = new Group2DItem(items);
            _groupItem->addItem(newGroup);
            setupBoxLayout(newGroup);

            //Add a new group item to the scene containing the selected items
            _scene->addItem(newGroup, iteratorPosition);
        }

        //Ungroup selected items that are a group
        else if (event->getKey() == Qt::Key_U)
        {
           //Get items from _groupItem
           std::set<Graphics2DItem*> items = _groupItem->ungroup();

            for(auto item : items)
            {
                item->getAABB2DItem()->visible(true);

                //If item is group, it has to be treated.
                Group2DItem* group = dynamic_cast<Group2DItem*> (item);
                if(group != nullptr)
                {
                    //Removing group from the scene
                    auto iteratorPosition = _scene->removeItem(group);

                    //The selected item is a group, so you have to ungroup again to capture the itens
                    std::set<Graphics2DItem*> subItems = group->ungroup();
                    delete group;

                    //Adding itens separately
                    for(auto sub : subItems)
                    {
                        _scene->addItem(sub, iteratorPosition);

                        //Necessary to keep the previously selected items selected
                        _groupItem->addItem(sub);
                        setupBoxLayout(sub);

                        sub->getAABB2DItem()->visible(true);
                    }
                }
                else
                {
                    //Necessary to keep the previously selected items selected
                    _groupItem->addItem(item);
                    setupBoxLayout(item);
                }
            }
        }
        else if (!event->isShiftKeyPressed() && event->getKey() == Qt::Key_M)
        {
            //Workaround to deal with the groupItem order problem
            _scene->removeItem(_groupItem);
            _scene->bringForward(_groupItem);
            _scene->addItem(_groupItem);
        }
        else if (!event->isShiftKeyPressed() && event->getKey() == Qt::Key_N)
        {
            _scene->sendBackward(_groupItem);
        }
        else if (event->isShiftKeyPressed() && event->getKey() == Qt::Key_N)
        {
            _scene->sendToBack(_groupItem);
        }
        else if (event->isShiftKeyPressed() && event->getKey() == Qt::Key_M)
        {
            //Workaround to deal with the groupItem order problem
            _scene->removeItem(_groupItem);
            _scene->bringToFront(_groupItem);
            _scene->addItem(_groupItem);
        }
    }
}



void Select2DItemTool::mouseHoverEvent2D(const GraphicsSceneHoverEvent *event, GraphicsView *view)
{
    AABB2DItem* groupItemSelectionBox = _groupItem->getAABB2DItem();
    if(groupItemSelectionBox->isVisible())
    {
        //Get the 2D view.
        Graphics2DView* view2D = dynamic_cast<Graphics2DView*>(view);

        //Test the intersection between the point and the box selection points.
        const Point2Df& p = event->getWorldPosition();

        //Verify if intersects with the selection box.
        unsigned int pointIndex = groupItemSelectionBox->getPointSetItem().intersects(p);

        if (pointIndex != GraphicsItem::NO_INTERSECTS)
        {
            //Get the box point
            const Point2Df& previewPosition = groupItemSelectionBox->getPointSetItem().getPointSet()[pointIndex];

            //Set position to preview.
            groupItemSelectionBox->getPreviewPointSetItem().setNewPointSet({previewPosition});
            groupItemSelectionBox->getPreviewPointSetItem().visible(false);

            //Points order pattern.
            //Opposite vertex to vertex i => o = (i + 4) % 8;
            //6-------7-------0
            //|               |
            //|               |
            //|               |
            //5               1
            //|               |
            //|               |
            //|               |
            //4-------3-------2

            switch (pointIndex % 4)
            {
                case 0:
                {
                    view->setCursor(Qt::SizeBDiagCursor);
                    break;
                }
                case 1:
                {
                    view->setCursor(Qt::SizeHorCursor);
                    break;
                }
                case 2:
                {
                    view->setCursor(Qt::SizeFDiagCursor);
                    break;
                }
                case 3:
                {
                    view->setCursor(Qt::SizeVerCursor);
                    break;
                }
            }
        }
        else if(groupItemSelectionBox->isInsideToItemAABB(p,view2D->getPixelSize()))
        {
            view->setCursor(Qt::OpenHandCursor);
            groupItemSelectionBox->getPreviewPointSetItem().visible(false);
        }
        else
        {
            view->setCursor(Qt::CrossCursor);
            groupItemSelectionBox->getPreviewPointSetItem().visible(false);
        }
    }

}



void Select2DItemTool::initialize()
{
    setupGroupBoxLayout();
    _groupItem->setModelToIdentity();
    _scene->addItem(_groupItem);
}



void Select2DItemTool::finalize()
{
    _groupItem->setModelToIdentity();
    _groupItem->getAABB2DItem()->visible(false);

    //set groupItems with their selection box visible
    for (auto i : _groupItem->getItems())
    {
        i->getAABB2DItem()->visible(false);
    }
    _groupItem->getAABB2DItem()->getPreviewPointSetItem().visible(false);
    _scene->removeItem(_groupItem);
    _interation = InterationMode::NONE;
}



std::string Select2DItemTool::name()
{
    return "Select 2D Item";
}



bool Select2DItemTool::hasSupport2D() const
{
    return true;
}



bool Select2DItemTool::hasSupport3D() const
{
    return false;
}
}
