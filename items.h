#ifndef ITEMS_H
#define ITEMS_H

#include <QPointF>
#include <QGraphicsItem>

class Items
{
public:
    Items();
    ~Items();

    QPointF* getFirstPoint();
    QPointF* getSecondPoint();
    QGraphicsItem* getItem();

    void setFirstPoint(QPointF *point);
    void setSecondPoint(QPointF *point);
    void setItem(QGraphicsItem *item);

private:
    QPointF *first_point;
    QPointF *second_point;
    QGraphicsItem *item;

};

#endif // ITEMS_H
