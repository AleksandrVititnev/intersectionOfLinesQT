#include "paintscene.h"

PaintScene::PaintScene(QObject *parent)
    : QGraphicsScene{parent}
{

}

PaintScene::~PaintScene()
{

}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    previousPoint = event->scenePos();

    return;
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::MouseButton::LeftButton
        || event->buttons() == Qt::MouseButton::RightButton) {

        removeItemSafely(item_3);
        item_3 = addLine(previousPoint.x(),
                         previousPoint.y(),
                         event->scenePos().x(),
                         event->scenePos().y(),
                         QPen(Qt::green, 10, Qt::SolidLine, Qt::RoundCap));

    }
    this->update();

    return;
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    if (event->button() == Qt::LeftButton) {

        drawItem(event->scenePos());
    }
    else if (event->button() == Qt::RightButton) {

        removeItemSafely(item_2);
        item_2 = addLine(previousPoint.x(),
                         previousPoint.y(),
                         event->scenePos().x(),
                         event->scenePos().y(),
                         QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
    }

    this->update();

    return;
}

void PaintScene::removeItemSafely(QGraphicsItem *_item)
{
    if (_item && _item->scene() == this) {
        this->removeItem(_item);
    }

    return;
}

void PaintScene::drawItem(QPointF point)
{
    removeItemSafely(item_3);

    if (type == ItemTypes::Line) {
        drawLine(&point);
    }
    else if (type == ItemTypes::Ray) {
        drawRay(&point);
    }
    else if (type == ItemTypes::Section) {
        drawSection(&point);
    }
}

void PaintScene::drawLine(QPointF *point)
{

}

void PaintScene::drawRay(QPointF *pointStart)
{
    removeItemSafely(item_1);
    QLineF lineStart = QLine(previousPoint.x(),
                        previousPoint.y(),
                        pointStart->x(),
                        pointStart->y());

    qreal angle = lineStart.angle();
    qreal maxLength = qSqrt(qPow(this->width(), 2) * qPow(this->height(), 2));
    QLineF line1 = QLineF(previousPoint, previousPoint + QPointF(1, 0));
    QLineF line2 = QLineF(previousPoint, previousPoint + QPointF(1, 0));

    line1.setLength(maxLength / 2);
    line1.setAngle(angle+ 180);

    line2.setLength(maxLength / 2);
    line2.setAngle(angle);

    QLineF line = QLineF(line1.p1(), line2.p2());

    item_1 = this->addLine(line);

}

void PaintScene::drawSection(QPointF *point)
{
    removeItemSafely(item_3);
    removeItemSafely(item_1);
    item_1 = addLine(previousPoint.x(),
                     previousPoint.y(),
                     point->x(),
                     point->y(),
                     QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
}

void PaintScene::setTypeItem(ItemTypes selectedType)
{
    type = selectedType;

    return;
}

