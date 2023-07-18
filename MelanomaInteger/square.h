#ifndef SQUARE_H
#define SQUARE_H

// Для создания прямоугольников выделения

#include "figure.h"

class Square : public Figure
{
    Q_OBJECT

public:
    explicit Square( QPointF point, QObject *parent = 0 ) ;
    ~Square(){}

private:
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget) ;
};

#endif // SQUARE_H
