#pragma once

#include "../Core/GraphicsItem.h"

namespace rm
{
class GraphicsItemModel
{
public:
    GraphicsItemModel() = default;
    GraphicsItemModel(GraphicsItem* item) : _item(item) { }
    GraphicsItemModel(const GraphicsItem &other) { *_item = other; }
    ~GraphicsItemModel() = default;
    GraphicsItem* getItem() { return _item;}

private:
    GraphicsItem* _item;
};
};
