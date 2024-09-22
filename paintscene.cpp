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
        removeItemSafely(item_1);
        item_1 = drawItem(event->scenePos(), QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
    }
    else if (event->button() == Qt::RightButton) {
        removeItemSafely(item_2);
        item_2 = drawItem(event->scenePos(), QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
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

QGraphicsItem *PaintScene::drawItem(QPointF point, QPen pen)
{
    QGraphicsItem *drewItem = nullptr;
    removeItemSafely(item_3);

    if (type == ItemTypes::Line) {
        drewItem = drawLine(&point, &pen);
    }
    else if (type == ItemTypes::Ray) {
        drewItem = drawRay(&point, &pen);
    }
    else if (type == ItemTypes::Section) {
        drewItem = drawSection(&point, &pen);
    }

    return drewItem;
}

QGraphicsItem *PaintScene::drawLine(QPointF *point, QPen *pen)
{
    // Получаем размер сцены (или GraphicsView)
    QRectF sceneRect = this->sceneRect();

    // Координаты границ сцены
    qreal left = sceneRect.left();
    qreal right = sceneRect.right();
    qreal top = sceneRect.top();
    qreal bottom = sceneRect.bottom();

    // Вычисляем коэффициенты уравнения прямой: y = mx + b
    qreal m = (point->y() - previousPoint.y()) / (point->x() - previousPoint.x());  // Угловой коэффициент (наклон)
    qreal b = point->y() - m * point->x();                    // Свободный член

    // Пересечения прямой с границами сцены
    // Левый край (x = left)
    QPointF leftPoint(left, m * left + b);

    // Правый край (x = right)
    QPointF rightPoint(right, m * right + b);

    // Верхний край (y = top), находим x для y = top
    QPointF topPoint((top - b) / m, top);

    // Нижний край (y = bottom), находим x для y = bottom
    QPointF bottomPoint((bottom - b) / m, bottom);

    // Список точек, которые находятся внутри видимой области сцены
    QList<QPointF> points;

    // Проверяем, пересекает ли точка левую или правую границу
    if (leftPoint.y() >= top && leftPoint.y() <= bottom) points.append(leftPoint);
    if (rightPoint.y() >= top && rightPoint.y() <= bottom) points.append(rightPoint);

    // Проверяем, пересекает ли точка верхнюю или нижнюю границу
    if (topPoint.x() >= left && topPoint.x() <= right) points.append(topPoint);
    if (bottomPoint.x() >= left && bottomPoint.x() <= right) points.append(bottomPoint);

    // Если нашлись две точки пересечения, добавляем линию
    if (points.size() == 2) {
        // Создаем линию от одной точки пересечения до другой
        return this->addLine(QLineF(points[0], points[1]), *pen);
    }

    return nullptr;
}

QGraphicsItem *PaintScene::drawRay(QPointF *pointStart, QPen *pen)
{
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

    return this->addLine(line, *pen);
}

QGraphicsItem *PaintScene::drawSection(QPointF *point, QPen *pen)
{
    return addLine(previousPoint.x(),
                     previousPoint.y(),
                     point->x(),
                     point->y(),
                     *pen);
}

void PaintScene::setTypeItem(ItemTypes selectedType)
{
    type = selectedType;

    return;
}
