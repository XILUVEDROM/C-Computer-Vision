#include "figure.h" // Для создания прямоугольников выделения

Figure::Figure(QPointF point, QObject *parent)
    :QObject(parent),QGraphicsItem()

{

}

QPointF Figure::startPoint()
{
    return m_startPoint ;
}

QPointF Figure::endPoint()
{
    return m_endPoint ;
}

void Figure::setStartPoint(QPointF point)
{
    m_startPoint = mapFromScene( point ) ;
}

void Figure::setEndPoint(QPointF point)
{
    m_endPoint = mapFromScene( point ) ;
}

QRectF Figure::boundingRect() const
{
    return QRectF() ;
}
