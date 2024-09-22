#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QDebug>

#include <enums.h>

class PaintScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PaintScene(QObject *parent = nullptr);
    ~PaintScene();

    void setTypeItem(ItemTypes selectedType);

private:
    QPointF previousPoint;
    QGraphicsItemGroup *object_1;
    QGraphicsItemGroup *object_2;
    QGraphicsItem *item_1 = nullptr;
    QGraphicsItem *item_2 = nullptr;
    QGraphicsItem *item_3 = nullptr;
    ItemTypes type;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

    void removeItemSafely(QGraphicsItem *_item);
    QGraphicsItem *drawItem(QPointF point, QPen pen);
    QGraphicsItem *drawLine(QPointF *point, QPen *pen);
    QGraphicsItem *drawRay(QPointF *pointStart, QPen *pen);
    QGraphicsItem *drawSection(QPointF *point, QPen *pen);
};

#endif // PAINTSCENE_H
