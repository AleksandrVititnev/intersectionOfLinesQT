#include "paintscene.h"

PaintScene::PaintScene(QObject *parent)
    : QGraphicsScene{parent}
{
    object_1 = new QGraphicsItemGroup();
    object_2 = new QGraphicsItemGroup();

    this->addItem(object_1);
    this->addItem(object_2);
}

PaintScene::~PaintScene()
{
    delete object_1;
    delete object_2;
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    previousPoint = event->scenePos();
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    if (item_3 != nullptr)
        this->removeItem(item_3);
    item_3 = addLine(previousPoint.x(),
            previousPoint.y(),
            event->scenePos().x(),
            event->scenePos().y(),
            QPen(Qt::green, 10, Qt::SolidLine, Qt::RoundCap));
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (item_3 != nullptr)
        this->removeItem(item_3);
    if (event->button() == Qt::LeftButton) {
        if (item_1 != nullptr)
            this->removeItem(item_1);
        item_1 = addLine(previousPoint.x(),
                         previousPoint.y(),
                         event->scenePos().x(),
                         event->scenePos().y(),
                         QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
    }

    if (event->button() == Qt::RightButton) {
        if (item_2 != nullptr)
            this->removeItem(item_2);
        item_2 = addLine(previousPoint.x(),
                         previousPoint.y(),
                         event->scenePos().x(),
                         event->scenePos().y(),
                         QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
    }
}
