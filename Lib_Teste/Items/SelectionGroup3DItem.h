#pragma once
#include "Group3DItem.h"
#include <set>
namespace rm
{
class Graphics3DItem;

/**
 * @brief The SelectionGroup3DItem class - manager a group of items adding and removing elements. This class is a
 * facility to selection tool.
 */
class SelectionGroup3DItem : public Group3DItem
{
public:
    /**
     * @brief SelectionGroup3DItem - Default constructor.
     */
	SelectionGroup3DItem();

    /**
     * @brief SelectionGroup3DItem - Constructor that receives a set o itens to form the group.
     * @param items - the initial set of items to form the group.
     */
    SelectionGroup3DItem(const std::set<Graphics3DItem*>& items);

    /**
     * @brief addItem - Add a new item to the current group.
     * @param item - new item to be added.
     * @return - true if the item is added to the group and false otherwise. In case of false, the item is already in
     * in the group.
     */
	bool addItem(Graphics3DItem* item);

    /**
     * @brief removeItem - Remove the item from the current group.
     * @param item - item to be removed from the group.
     * @return  - true if the item was removed and false in case the item do not belongs to the group.
     */
	bool removeItem(Graphics3DItem* item);

};
}
