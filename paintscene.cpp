#include "paintscene.h"

PaintScene::PaintScene(QObject *parent)
    : QGraphicsScene{parent}
{
    item_2 = new Items();
    item_3 = new Items();
}

PaintScene::~PaintScene()
{
    if (item_2 != nullptr) delete item_2;
    if (item_3 != nullptr) delete item_3;
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

        removeItemSafely(item_1);
        item_1 = addLine(previousPoint.x(),
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

    QPointF currentPos = event->scenePos();

    if (event->button() == Qt::LeftButton) {

        item_2->setType(type);
        item_2->setFirstPoint(previousPoint);
        item_2->setSecondPoint(currentPos);

        removeItemSafely(item_2->getItem());
        item_2->setItem(drawItem(currentPos, QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap)));

    }
    else if (event->button() == Qt::RightButton) {

        item_3->setType(type);
        item_3->setFirstPoint(previousPoint);
        item_3->setSecondPoint(currentPos);

        removeItemSafely(item_3->getItem());
        item_3->setItem(drawItem(currentPos, QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap)));

    }

    if (item_2->isOnScene()
        && item_3->isOnScene()
        && isLinesIntersect()) {

        QString text = "Pereseklis!";
        QGraphicsTextItem *textItem = this->addText(text);
        textItem->setPos(currentPos);
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
    removeItemSafely(item_1);

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
    QLineF drewLine;
    qreal maxLength = qSqrt(qPow(this->width(), 2) * qPow(this->height(), 2));

    // Получаем размер сцены (или GraphicsView)
    QRectF sceneRect = this->sceneRect();

    // Координаты границ сцены
    qreal left = sceneRect.left();
    qreal right = sceneRect.right();
    qreal top = sceneRect.top();
    qreal bottom = sceneRect.bottom();

    // Вычисляем коэффициенты уравнения прямой: y = mx + b
    qreal m = (point->y() - previousPoint.y()) / (point->x() - previousPoint.x());  // Угловой коэффициент (наклон)
    qreal b = (point->y() - m * point->x());                    // Свободный член

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
        drewLine = QLineF(points[0], points[1]);
        drewLine.setLength(maxLength / 2);
        return this->addLine(drewLine, *pen);
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

bool PaintScene::isLinesIntersect()
{
    QSMatrix a = pointToMatrix(item_2->getFirstPoint());
    QSMatrix b = pointToMatrix(item_2->getSecondPoint());
    QSMatrix c = pointToMatrix(item_3->getFirstPoint());
    QSMatrix d = pointToMatrix(item_3->getSecondPoint());

    QSMatrix V = b - a;
    QSMatrix M = V.mergeMatrix(c - d);

    qreal det = M.determinant();

    if (qAbs(det) <= qPow(10, -9)) {
        return false;
    }

    QSMatrix obrM = M.getReverse();
    QSMatrix T = obrM * (c - a);

    qreal t1 = T(0, 0);
    qreal t2 = T(1, 0);

    bool u1 =
        item_2->getType() == ItemTypes::Line
        || (item_2->getType() == ItemTypes::Ray && t1 >= 0)
              || (item_2->getType() == ItemTypes::Section && t1 >= 0 && t1 <= 1);

    bool u2 =
        item_3->getType() == ItemTypes::Line
        || (item_3->getType() == ItemTypes::Ray && t2 >= 0)
        || (item_3->getType() == ItemTypes::Section && t2 >= 0 && t2 <= 1);

    return u1 && u2;
}

QSMatrix PaintScene::pointToMatrix(QPointF point)
{
    QSMatrix matrix(2, 1, 0);
    matrix(0, 0) = point.x();
    matrix(1, 0) = point.y();

    return matrix;
}

void PaintScene::setTypeItem(ItemTypes selectedType)
{
    type = selectedType;

    return;
}
