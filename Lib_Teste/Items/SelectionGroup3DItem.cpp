#include "SelectionGroup3DItem.h"

namespace rm
{
SelectionGroup3DItem::SelectionGroup3DItem()
    :Group3DItem(std::set<Graphics3DItem*>())
{
}



SelectionGroup3DItem::SelectionGroup3DItem(const std::set<Graphics3DItem*>& items)
    : Group3DItem (items)
{

}



bool SelectionGroup3DItem::addItem(Graphics3DItem* item)
{
    //Try to insert element and test if the element was inserted.
    if (_items.insert(item).second)
    {
        //Update AABB.
        _aabb += item->getAABB();
    }
    return false;
}



bool SelectionGroup3DItem::removeItem(Graphics3DItem* item)
{
    //Remove item if in selection group
    if(_items.erase(item) != 0)
    {
        computeAABB();
        return true;
    }
    return false;
}
}
