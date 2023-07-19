#ifndef MYGRAPHICSITEM_H
#define MYGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QObject>
#include <QPainter>

class mygraphicsitem : public QGraphicsPixmapItem
{
public:
    mygraphicsitem(const QPixmap& pm);
    mygraphicsitem();
    QRectF boundingRect() const;
};

#endif // MYGRAPHICSITEM_H
