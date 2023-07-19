#include "line.h"

#include <QPainter>

Line::Line(QPointF point, QObject *parent) :
    Figure(point,parent)
{
    Q_UNUSED(point)
}

Line::~Line()
{

}

// Реализуем метод отрисовки
void Line::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(QPen(color, depth));

    QLineF line1(endPoint().x() > startPoint().x() ? startPoint().x() : endPoint().x(),
                endPoint().y() > startPoint().y() ? startPoint().y() : endPoint().y(),
                (endPoint().x() - startPoint().x()),
                (endPoint().y() - startPoint().y()));

    painter->drawLine(line1);

    Q_UNUSED(option)
    Q_UNUSED(widget)
}

