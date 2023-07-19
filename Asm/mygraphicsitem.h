#ifndef MYGRAPHICSITEM_H
#define MYGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>


class MyGraphicsItem : public QGraphicsPixmapItem
{
public:
    MyGraphicsItem(const QPixmap& pm);
    ~MyGraphicsItem();
    QRectF boundingRect() const;

};

#endif // MYGRAPHICSITEM_H

