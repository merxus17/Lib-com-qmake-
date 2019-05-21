#include "SelectionGroup2DItem.h"
#include "../Core/Graphics2DItem.h"
namespace rm
{

SelectionGroup2DItem::SelectionGroup2DItem()
    : Group2DItem(std::set<Graphics2DItem*>())
{
}



SelectionGroup2DItem::SelectionGroup2DItem(const std::set<Graphics2DItem*>& items)
    : Group2DItem(items)
{
}



bool SelectionGroup2DItem::addItem(Graphics2DItem* item)
{
    //Try to insert element and test if the element was inserted.
    if (_items.insert(item).second)
    {
        //Recompute AABB to take in account any object transformations.
        computeAABB();

        return true;
    }
    return false;
}



bool SelectionGroup2DItem::removeItem(Graphics2DItem* item)
{
    if (_items.erase(item) != 0)
    {
        computeAABB();
        return true;
    }
    return false;
}



void SelectionGroup2DItem::render(int)
{
    //It is important to notice that this method was intentionally left in black (not anymore because of this comment),
    //because the selection group do not control the items. At this moment, the selected objects are rendered by the
    //scene through the GraphicsView2D. This way, all items are rendered at the same way as their selection boxes. As
    //the selection box is an item on scene, its selection box is rendered too, but nothing more is rendered.

}
}
