#ifndef LINE_H
#define LINE_H

#include <QObject>
#include <QGraphicsItem>
#include "figure.h"

/* Наследуемся от класса Figure,
 * в котором реализован общий для всех фигур функционал
 * */
class Line :public Figure
   {
    Q_OBJECT

public:
    explicit Line(QPointF point, QObject *parent = 0);
    ~Line();

private:
    // Для line реализуем только саму отрисовку
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // LINE_H
