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
    if (type == ItemTypes::None) return;

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
    if (type == ItemTypes::None) return;

    if (event->buttons() == Qt::MouseButton::LeftButton
        || event->buttons() == Qt::MouseButton::RightButton) {

        QPointF currentPos = event->scenePos();
        correctCoords(&currentPos);

        if (editLinesMode) {
            removeItemSafely(item_1);
            if (isPointInClass(item_2, previousPoint)) {
                if (previousPoint == item_2->getFirstPoint())
                    item_1 = drawItem(currentPos, item_2->getSecondPoint(), item_2->getType(), QPen(Qt::green, 5, Qt::SolidLine, Qt::RoundCap));
                else if (previousPoint == item_2->getSecondPoint())
                    item_1 = drawItem(item_2->getFirstPoint(), currentPos, item_2->getType(), QPen(Qt::green, 5, Qt::SolidLine, Qt::RoundCap));
            }
            else if (isPointInClass(item_3, previousPoint)) {
                if (previousPoint == item_3->getFirstPoint())
                    item_1 = drawItem(currentPos, item_3->getSecondPoint(), item_3->getType(), QPen(Qt::green, 5, Qt::SolidLine, Qt::RoundCap));
                else if (previousPoint == item_3->getSecondPoint())
                    item_1 = drawItem(item_3->getFirstPoint(), currentPos, item_3->getType(), QPen(Qt::green, 5, Qt::SolidLine, Qt::RoundCap));
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
    if (type == ItemTypes::None) return;

    QPointF currentPos = event->scenePos();
    QPointF pointIntersectCenter;
    QString text;

    correctCoords(&currentPos);

    if (editLinesMode) {
        if (isPointInClass(item_2, previousPoint)) {
            if (previousPoint == item_2->getFirstPoint())
                item_2->setFirstPoint(currentPos);
            else if (previousPoint == item_2->getSecondPoint())
                item_2->setSecondPoint(currentPos);

            removeItemSafely(item_2->getItem());
            removeItemPoints(item_2->getItemsPoints());
            item_2->setItem(drawItem(item_2->getFirstPoint(), item_2->getSecondPoint(), item_2->getType(), QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap)));
            addItemPoints(item_2);
        }
        else if (isPointInClass(item_3, previousPoint)) {
            if (previousPoint == item_3->getFirstPoint())
                item_3->setFirstPoint(currentPos);
            else if (previousPoint == item_3->getSecondPoint())
                item_3->setSecondPoint(currentPos);

            removeItemSafely(item_3->getItem());
            removeItemPoints(item_3->getItemsPoints());
            item_3->setItem(drawItem(item_3->getFirstPoint(), item_3->getSecondPoint(), item_3->getType(), QPen(Qt::blue, 5, Qt::SolidLine, Qt::RoundCap)));
            addItemPoints(item_3);
        }

    }
    else {

        if (event->button() == Qt::LeftButton) {

            item_2->setType(type);
            item_2->setFirstPoint(previousPoint);
            item_2->setSecondPoint(currentPos);

            removeItemSafely(item_2->getItem());
            removeItemPoints(item_2->getItemsPoints());
            item_2->setItem(drawItem(item_2->getFirstPoint(), item_2->getSecondPoint(), item_2->getType(), QPen(Qt::red, 5, Qt::SolidLine, Qt::RoundCap)));
            addItemPoints(item_2);
        }
        else if (event->button() == Qt::RightButton) {

            item_3->setType(type);
            item_3->setFirstPoint(previousPoint);
            item_3->setSecondPoint(currentPos);

            removeItemSafely(item_3->getItem());
            removeItemPoints(item_3->getItemsPoints());
            item_3->setItem(drawItem(item_3->getFirstPoint(), item_3->getSecondPoint(), item_3->getType(), QPen(Qt::blue, 5, Qt::SolidLine, Qt::RoundCap)));
            addItemPoints(item_3);
        }
    }

    auto [isIntersect, pointIntersect] = linesIntersect();

    if (item_2->isOnScene()
        && item_3->isOnScene()
        && isIntersect) {

        removeItemSafely(itemPointIntersect);

        pointIntersectCenter = pointFromCenter(pointIntersect);

        text = "Пересекаются. Точка пересечения: \nx: %1, y: %2";
        text = text.arg(QString::number(pointIntersectCenter.x(), 'f', 1))
                   .arg(QString::number(pointIntersectCenter.y(), 'f', 1));

        itemPointIntersect = this->addEllipse(
                qRound(pointIntersect.x()) - 10,
                qRound(pointIntersect.y()) - 10, 20, 20,
                QPen(Qt::green, 1), QBrush(Qt::green));
    }
    else {
        removeItemSafely(itemPointIntersect);
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

    _item = nullptr;

    return;
}

void PaintScene::drawGrid(qreal gridSize)
{
    // Получаем размеры сцены
    QRectF sceneRect = this->sceneRect();

    // Получаем середину сцены
    qreal centerX = sceneRect.left() + sceneRect.width() / 2;
    qreal centerY = sceneRect.top() + sceneRect.height() / 2;

    centerX = qRound(centerX / 50.) * 50;
    centerY = qRound(centerY / 50.) * 50;

    // Очищаем текущую сцену перед рисованием сетки
    this->clear();
    item_2->clearItem();
    item_3->clearItem();

    // Рисуем вертикальные линии
    for (qreal x = sceneRect.left() - 50; x <= sceneRect.right() + 50; x += gridSize) {
        this->addLine(QLineF(x, sceneRect.top() - 50, x, sceneRect.bottom() + 50), QPen(Qt::gray));
    }

    this->addLine(QLineF(centerX, sceneRect.top() - 50, centerX, sceneRect.bottom() + 50), QPen(Qt::black));

    // Рисуем горизонтальные линии
    for (qreal y = sceneRect.top() - 50; y <= sceneRect.bottom() + 50; y += gridSize) {
        this->addLine(QLineF(sceneRect.left() - 50, y, sceneRect.right() + 50, y), QPen(Qt::gray));
    }

    this->addLine(QLineF(sceneRect.left() - 50, centerY, sceneRect.right() + 50, centerY), QPen(Qt::black));
}

void PaintScene::reCalcMiddleScreen()
{
    middleScreen = getSceneCenter();
    return;
}

QGraphicsItem *PaintScene::drawItem(QPointF pointFirst, QPointF pointSecond, ItemTypes _type, QPen pen)
{
    QGraphicsItem *drewItem = nullptr;
    removeItemSafely(item_1);

    if (_type == ItemTypes::Line) {
        drewItem = drawLine(pointFirst, pointSecond, &pen);
    }
    else if (_type == ItemTypes::Ray) {
        drewItem = drawRay(pointFirst, pointSecond, &pen);
    }
    else if (_type == ItemTypes::Section) {
        drewItem = drawSection(pointFirst, pointSecond, &pen);
    }

    return drewItem;
}

QGraphicsItem *PaintScene::drawLine(QPointF pointFirst, QPointF pointSecond, QPen *pen)
{
    QLineF drewLine;

    // Получаем размер сцены
    QRectF sceneRect = this->sceneRect();

    // Координаты границ сцены
    qreal left = sceneRect.left() - 50;
    qreal right = sceneRect.right() + 50;
    qreal top = sceneRect.top() - 50;
    qreal bottom = sceneRect.bottom() + 50;

    // Список точек пересечения
    QList<QPointF> points;

    // Проверяем, является ли линия вертикальной (x координаты равны)
    if (qFuzzyCompare(pointFirst.x(), pointSecond.x())) {
        // Линия вертикальная, пересекаем верхний и нижний край сцены
        points.append(QPointF(pointFirst.x(), top));     // Пересечение с верхом
        points.append(QPointF(pointFirst.x(), bottom));  // Пересечение с низом
    } else {
        // Линия не вертикальная, вычисляем наклон (m) и свободный член (b)
        qreal m = (pointFirst.y() - pointSecond.y()) / (pointFirst.x() - pointSecond.x());
        qreal b = pointFirst.y() - m * pointFirst.x();

        // Пересечение с левым краем (x = left)
        QPointF leftPoint(left, m * left + b);
        if (leftPoint.y() >= top && leftPoint.y() <= bottom) {
            points.append(leftPoint);
        }

        // Пересечение с правым краем (x = right)
        QPointF rightPoint(right, m * right + b);
        if (rightPoint.y() >= top && rightPoint.y() <= bottom) {
            points.append(rightPoint);
        }

        // Пересечение с верхним краем (y = top)
        QPointF topPoint((top - b) / m, top);
        if (topPoint.x() >= left && topPoint.x() <= right) {
            points.append(topPoint);
        }

        // Пересечение с нижним краем (y = bottom)
        QPointF bottomPoint((bottom - b) / m, bottom);
        if (bottomPoint.x() >= left && bottomPoint.x() <= right) {
            points.append(bottomPoint);
        }
    }

    // Если нашлись две точки пересечения, добавляем линию
    if (points.size() >= 2) {
        // Выбираем первые две точки
        drewLine = QLineF(points[0], points[1]);
        // Добавляем линию на сцену
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

QPointF PaintScene::getSceneCenter()
{
    // Получаем размеры сцены
    QRectF sceneRect = this->sceneRect();

    // Вычисляем координаты центра
    qreal centerX = sceneRect.left() + sceneRect.width() / 2 / 50;
    qreal centerY = sceneRect.top() + sceneRect.height() / 2 / 50;

    // Возвращаем точку с координатами середины
    return QPointF(qRound(centerX), qRound(centerY));
}

QPointF PaintScene::pointFromCenter(QPointF point)
{
    qreal x = middleScreen.x() - point.x() / 50.;
    qreal y = middleScreen.y() - point.y() / 50.;

    return QPointF(-x, y);
}

void PaintScene::correctCoords(QPointF *point)
{
    if (point == nullptr) return;

    point->setX(25 * qRound(point->x() / 25.));
    point->setY(25 * qRound(point->y() / 25.));

    return;
}

void PaintScene::removeItemPoints(QVector<QGraphicsItem *> *points)
{
    if (points == nullptr) return;

    for (auto point : *points) {
        this->removeItemSafely(point);
    }

    points->clear();
}

void PaintScene::addItemPoints(Items *item)
{
    if (item == nullptr) return;

    qreal radius = 5;
    QVector<QGraphicsItem*> *points = item->getItemsPoints();

    points->append(
        this->addEllipse(
            item->getFirstPoint().x() - radius,
            item->getFirstPoint().y() - radius, radius * 2, radius * 2,
            QPen(Qt::green, 1), QBrush(Qt::black)));

    points->append(
        this->addEllipse(
            item->getSecondPoint().x() - radius,
            item->getSecondPoint().y() - radius, radius * 2, radius * 2,
            QPen(Qt::green, 1), QBrush(Qt::black)));
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
