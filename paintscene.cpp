#include "paintscene.h"

PaintScene::PaintScene(QTextBrowser *_text, QObject *parent)
    : QGraphicsScene{parent}
{
    item_2 = new Items();
    item_3 = new Items();
    textResult = _text;

}

PaintScene::~PaintScene()
{
    if (item_2 != nullptr) delete item_2;
    if (item_3 != nullptr) delete item_3;
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    previousPoint = event->scenePos();
    correctCoords(&previousPoint);

    if (isPointInClass(item_2, previousPoint)
        || isPointInClass(item_3, previousPoint)) {

        editLinesMode = true;
    }
    else {
        editLinesMode = false;
    }

    return;
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->buttons() == Qt::MouseButton::LeftButton
        || event->buttons() == Qt::MouseButton::RightButton) {

        QPointF currentPos = event->scenePos();
        correctCoords(&currentPos);

        if (editLinesMode) {
            removeItemSafely(item_1);
            if (isPointInClass(item_2, previousPoint)) {
                item_1 = addLine(currentPos.x(),
                                 currentPos.y(),
                                 item_2->getSecondPoint().x(),
                                 item_2->getSecondPoint().y(),
                                 QPen(Qt::green, 10, Qt::SolidLine, Qt::RoundCap));
            }
            else if (isPointInClass(item_3, previousPoint)) {
                item_1 = addLine(currentPos.x(),
                                 currentPos.y(),
                                 item_3->getSecondPoint().x(),
                                 item_3->getSecondPoint().y(),
                                 QPen(Qt::green, 10, Qt::SolidLine, Qt::RoundCap));
            }
        }
        else {

            removeItemSafely(item_1);
            item_1 = addLine(previousPoint.x(),
                             previousPoint.y(),
                             currentPos.x(),
                             currentPos.y(),
                             QPen(Qt::green, 10, Qt::SolidLine, Qt::RoundCap));
        }

    }
    this->update();

    return;
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    QPointF currentPos = event->scenePos();
    QString text;

    correctCoords(&currentPos);

    if (editLinesMode) {

        if (isPointInClass(item_2, previousPoint)) {
            item_2->setFirstPoint(currentPos);
            removeItemSafely(item_2->getItem());
            item_2->setItem(drawItem(item_2, QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap)));
        }
        else if (isPointInClass(item_3, previousPoint)) {
            item_3->setFirstPoint(currentPos);
            removeItemSafely(item_3->getItem());
            item_3->setItem(drawItem(item_3, QPen(Qt::blue, 5, Qt::SolidLine, Qt::RoundCap)));
        }

    }
    else {

        if (event->button() == Qt::LeftButton) {

            item_2->setType(type);
            item_2->setFirstPoint(previousPoint);
            item_2->setSecondPoint(currentPos);

            removeItemSafely(item_2->getItem());
            item_2->setItem(drawItem(item_2, QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap)));

        }
        else if (event->button() == Qt::RightButton) {

            item_3->setType(type);
            item_3->setFirstPoint(previousPoint);
            item_3->setSecondPoint(currentPos);

            removeItemSafely(item_3->getItem());
            item_3->setItem(drawItem(item_3, QPen(Qt::blue, 5, Qt::SolidLine, Qt::RoundCap)));

        }
    }

    auto [isIntersect, pointIntersect] = linesIntersect();

    if (item_2->isOnScene()
        && item_3->isOnScene()
        && isIntersect) {

        text = "Пересекаются. Точка пересечения: x: %1, y: %2";
        text = text.arg(pointIntersect.x() / 50.).arg(pointIntersect.y() / 50.);
        this->addEllipse(pointIntersect.x()-2.5, pointIntersect.y()-2.5, 3, 3, QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap));
    }
    else {
        text = "Не пересекаются.";
    }

    textResult->setText(text);

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

void PaintScene::drawGrid(qreal gridSize)
{
    // Получаем размеры сцены
    QRectF sceneRect = this->sceneRect();

    // Очищаем текущую сцену перед рисованием сетки
    this->clear();

    // Рисуем вертикальные линии
    for (qreal x = sceneRect.left() - 50; x <= sceneRect.right() + 50; x += gridSize) {
        this->addLine(QLineF(x, sceneRect.top() - 50, x, sceneRect.bottom() + 50), QPen(Qt::gray));
    }

    // Рисуем горизонтальные линии
    for (qreal y = sceneRect.top() - 50; y <= sceneRect.bottom() + 50; y += gridSize) {
        this->addLine(QLineF(sceneRect.left() - 50, y, sceneRect.right() + 50, y), QPen(Qt::gray));
    }
}

QGraphicsItem *PaintScene::drawItem(Items *item, QPen pen)
{
    QGraphicsItem *drewItem = nullptr;
    removeItemSafely(item_1);

    if (type == ItemTypes::Line) {
        drewItem = drawLine(item->getFirstPoint(), item->getSecondPoint(), &pen);
    }
    else if (type == ItemTypes::Ray) {
        drewItem = drawRay(item->getFirstPoint(), item->getSecondPoint(), &pen);
    }
    else if (type == ItemTypes::Section) {
        drewItem = drawSection(item->getFirstPoint(), item->getSecondPoint(), &pen);
    }

    return drewItem;
}

QGraphicsItem *PaintScene::drawLine(QPointF pointFirst, QPointF pointSecond, QPen *pen)
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

    // Список точек пересечения
    QList<QPointF> points;

    // Проверяем, является ли линия вертикальной
    if (pointFirst.x() == pointSecond.x()) {
        // Линия вертикальная, пересекаем верхний и нижний край сцены
        points.append(QPointF(pointFirst.x(), top));     // Пересечение с верхом
        points.append(QPointF(pointFirst.x(), bottom));  // Пересечение с низом
    } else {
        // Линия не вертикальная, вычисляем угловой коэффициент m и свободный член b
        qreal m = (pointFirst.y() - pointSecond.y()) / (pointFirst.x() - pointSecond.x());
        qreal b = (pointFirst.y() - m * pointFirst.x());

        // Пересечения прямой с границами сцены
        // Левый край (x = left)
        QPointF leftPoint(left, m * left + b);

        // Правый край (x = right)
        QPointF rightPoint(right, m * right + b);

        // Верхний край (y = top), находим x для y = top
        QPointF topPoint((top - b) / m, top);

        // Нижний край (y = bottom), находим x для y = bottom
        QPointF bottomPoint((bottom - b) / m, bottom);

        // Проверяем, пересекает ли точка левую или правую границу
        if (leftPoint.y() >= top && leftPoint.y() <= bottom) points.append(leftPoint);
        if (rightPoint.y() >= top && rightPoint.y() <= bottom) points.append(rightPoint);

        // Проверяем, пересекает ли точка верхнюю или нижнюю границу
        if (topPoint.x() >= left && topPoint.x() <= right) points.append(topPoint);
        if (bottomPoint.x() >= left && bottomPoint.x() <= right) points.append(bottomPoint);
    }

    // Если нашлись две точки пересечения, добавляем линию
    if (points.size() == 2) {
        // Создаем линию от одной точки пересечения до другой
        drewLine = QLineF(points[0], points[1]);
        drewLine.setLength(maxLength / 2);
        return this->addLine(drewLine, *pen);
    }

    return nullptr;
}

QGraphicsItem *PaintScene::drawRay(QPointF pointFirst, QPointF pointSecond, QPen *pen)
{
    QLineF lineStart = QLine(pointFirst.x(),
                        pointFirst.y(),
                        pointSecond.x(),
                        pointSecond.y());

    qreal angle = lineStart.angle();
    qreal maxLength = qSqrt(qPow(this->width(), 2) * qPow(this->height(), 2));
    QLineF line1 = QLineF(pointFirst, pointFirst + QPointF(1, 0));
    QLineF line2 = QLineF(pointSecond, pointSecond + QPointF(1, 0));

    line1.setLength(maxLength / 2);
    line1.setAngle(angle+ 180);

    line2.setLength(maxLength / 2);
    line2.setAngle(angle);

    QLineF line = QLineF(line1.p1(), line2.p2());

    return this->addLine(line, *pen);
}

QGraphicsItem *PaintScene::drawSection(QPointF pointFirst, QPointF pointSecond, QPen *pen)
{
    return addLine(pointFirst.x(),
                     pointFirst.y(),
                     pointSecond.x(),
                     pointSecond.y(),
                     *pen);
}

bool PaintScene::isPointInClass(Items *line, const QPointF &checkPoint)
{
    // Получаем первую и вторую точки из объекта класса
    QPointF firstPoint = line->getFirstPoint();
    QPointF secondPoint = line->getSecondPoint();

    // Проверяем, совпадает ли точка проверки с первой или второй точками
    return (firstPoint == checkPoint) || (secondPoint == checkPoint);
}

void PaintScene::correctCoords(QPointF *point)
{
    if (point == nullptr) return;

    point->setX(25 * qRound(point->x() / 25.));
    point->setY(25 * qRound(point->y() / 25.));

    return;
}

std::pair<bool, QPointF> PaintScene::linesIntersect()
{
    bool u1 = false, u2 = false, isIntersect = false;
    QPointF pointIntersect(0, 0);
    QSMatrix a = pointToMatrix(item_2->getFirstPoint());
    QSMatrix b = pointToMatrix(item_2->getSecondPoint());
    QSMatrix c = pointToMatrix(item_3->getFirstPoint());
    QSMatrix d = pointToMatrix(item_3->getSecondPoint());

    QSMatrix V = b - a;
    QSMatrix M = V.mergeMatrix(c - d);

    qreal det = M.determinant();

    if (qAbs(det) <= qPow(10, -9)) {
        return std::make_pair(false, pointIntersect);
    }

    QSMatrix obrM = M.getReverse();
    QSMatrix T = obrM * (c - a);

    qreal t1 = T(0, 0);
    qreal t2 = T(1, 0);

    u1 =
        item_2->getType() == ItemTypes::Line
        || (item_2->getType() == ItemTypes::Ray && t1 >= 0)
              || (item_2->getType() == ItemTypes::Section && t1 >= 0 && t1 <= 1);

    u2 =
        item_3->getType() == ItemTypes::Line
        || (item_3->getType() == ItemTypes::Ray && t2 >= 0)
        || (item_3->getType() == ItemTypes::Section && t2 >= 0 && t2 <= 1);

    isIntersect = u1 && u2;

    QSMatrix matrixIntersect = a + V * t1;

    pointIntersect.setX(matrixIntersect(0, 0));
    pointIntersect.setY(matrixIntersect(1, 0));

    return std::make_pair(isIntersect, pointIntersect);
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
