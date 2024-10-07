#ifndef ITEMS_H
#define ITEMS_H

#include <QPointF>
#include <QGraphicsItem>
#include <QLineF>
#include <enums.h>

class Items
{
public:
    Items();
    ~Items();

    QPointF getFirstPoint();
    QPointF getSecondPoint();
    QGraphicsItem* getItem();
    ItemTypes getType();
    bool isOnScene();
    QVector<QGraphicsItem*> *getItemsPoints();

    void setFirstPoint(QPointF point);
    void setSecondPoint(QPointF point);
    void setItem(QGraphicsItem *item);
    void setType(ItemTypes _type);
    void clearItem();
    void addItemPoint(QGraphicsItem* itemPoint);
private:
    QPointF first_point;
    QPointF second_point;
    QGraphicsItem *item;
    ItemTypes type;
    QVector<QGraphicsItem*> points;
};

#endif // ITEMS_H
