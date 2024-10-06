#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemGroup>
#include <QTimer>
#include <QDebug>
#include <QTextBrowser>

#include <enums.h>
#include <items.h>
#include <matrix.h>

class PaintScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit PaintScene(QTextBrowser *_text, QObject *parent = nullptr);
    ~PaintScene();

    void setTypeItem(ItemTypes selectedType);
    void drawGrid(qreal gridSize);

private:
    QPointF previousPoint;
    QGraphicsItemGroup *object_1;
    QGraphicsItemGroup *object_2;
    QGraphicsItem *item_1 = nullptr;
    QTextBrowser *textResult;
    Items *item_2 = nullptr;
    Items *item_3 = nullptr;
    ItemTypes type;
    bool editLinesMode = false;
    int nowEditLine = 0;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void removeItemSafely(QGraphicsItem *_item);
    void correctCoords(QPointF *point);

    QGraphicsItem *drawItem(Items *item, QPen pen);
    QGraphicsItem *drawLine(QPointF pointFirst, QPointF pointSecond, QPen *pen);
    QGraphicsItem *drawRay(QPointF pointFirst, QPointF pointSecond, QPen *pen);
    QGraphicsItem *drawSection(QPointF pointFirst, QPointF pointSecond, QPen *pen);
    bool isPointInClass(Items *line, const QPointF& checkPoint);

    std::pair<bool, QPointF> linesIntersect();
    QSMatrix pointToMatrix(QPointF point);
};

#endif // PAINTSCENE_H
