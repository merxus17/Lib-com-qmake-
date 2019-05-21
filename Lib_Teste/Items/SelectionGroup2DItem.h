#pragma once
#include "Group2DItem.h"
#include <set>

namespace rm
{
class Graphics2DItem;
/**
 * @brief The SelectionGroup2DItem class - manager a group of items adding and removing elements. This class is a
 * facility to selection tool.
 */
class SelectionGroup2DItem : public Group2DItem
{
public:
    /**
     * @brief SelectionGroup2DItem - Default constructor.
     */
	SelectionGroup2DItem();

    /**
     * @brief SelectionGroup2DItem - Constructor that receives a set o itens to form the group.
     * @param items - the initial set of items to form the group.
     */
    SelectionGroup2DItem(const std::set<Graphics2DItem*>& items);

    /**
     * @brief addItem - Add a new item to the current group.
     * @param item - new item to be added.
     * @return - true if the item is added to the group and false otherwise. In case of false, the item is already in
     * in the group.
     */
    bool addItem(Graphics2DItem* item);

    /**
     * @brief removeItem - Remove the item from the current group.
     * @param item - item to be removed from the group.
     * @return  - true if the item was removed and false in case the item do not belongs to the group.
     */
    bool removeItem(Graphics2DItem* item);

    /**
     * @brief render - Virtual function of render. It should contain whatever is necessary to draw on the correct
     * context (empty implementation).
     * @param viewId - View identifier.
     */
    void render(int viewId) override;

};
}
