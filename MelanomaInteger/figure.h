#ifndef FIGURE_H
#define FIGURE_H

// Для создания фигуры выделения

#include <QGraphicsObject>

class Figure : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    explicit Figure(QPointF point, QObject *parent = 0 );
    ~Figure() {}

    QPointF startPoint() ;
    QPointF endPoint() ;

    void setStartPoint( QPointF point ) ;
    void setEndPoint( QPointF point ) ;

private:
    QRectF boundingRect() const ;

private:
    QPointF m_startPoint ;
    QPointF m_endPoint ;

};

#endif // FIGURE_H
