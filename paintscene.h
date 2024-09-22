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
    void drawItem(QPointF point);
    void drawLine(QPointF *point);
    void drawRay(QPointF *pointStart);
    void drawSection(QPointF *point);
};

#endif // PAINTSCENE_H
