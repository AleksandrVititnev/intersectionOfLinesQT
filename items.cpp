#include "items.h"

Items::Items() {
    first_point = nullptr;
    second_point = nullptr;
    item = nullptr;
}

Items::~Items()
{
    if (first_point != nullptr)
        delete first_point;
    if (second_point != nullptr)
        delete second_point;
    if (item != nullptr)
        delete item;
}

QPointF *Items::getFirstPoint()
{
    return first_point;
}

QPointF *Items::getSecondPoint()
{
    return second_point;
}

QGraphicsItem *Items::getItem()
{
    return item;
}

void Items::setFirstPoint(QPointF *point)
{
    first_point = point;
}

void Items::setSecondPoint(QPointF *point)
{
    second_point = point;
}

void Items::setItem(QGraphicsItem *_item)
{
    item = _item;
}
