#include "square.h" // Для создания прямоугольников выделения

#include <QPainter>

Square::Square(QPointF point, QObject *parent)
    : Figure( point, parent )
{

}

void Square::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    painter->setPen( QPen( Qt::black, 3) ) ;

    painter->drawRect( QRectF(  startPoint(), endPoint() ) );

    Q_UNUSED(widget)
    Q_UNUSED(option)
}
