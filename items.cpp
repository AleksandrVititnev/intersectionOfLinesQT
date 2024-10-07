#include "items.h"

Items::Items() {
    item = nullptr;
}

Items::~Items()
{

}

QPointF Items::getFirstPoint()
{
    return first_point;
}

QPointF Items::getSecondPoint()
{
    return second_point;
}

QGraphicsItem *Items::getItem()
{
    return item;
}

ItemTypes Items::getType()
{
    return type;
}

bool Items::isOnScene()
{
    return (item != nullptr && item->scene());
}

QVector<QGraphicsItem *> *Items::getItemsPoints()
{
    return &points;
}

void Items::setFirstPoint(QPointF point)
{
    first_point = point;
}

void Items::setSecondPoint(QPointF point)
{
    second_point = point;
}

void Items::setItem(QGraphicsItem *_item)
{
    item = _item;
}

void Items::setType(ItemTypes _type)
{
    type = _type;
}

void Items::clearItem()
{
    item = nullptr;
}

void Items::addItemPoint(QGraphicsItem *itemPoint)
{
    points.append(itemPoint);
}

