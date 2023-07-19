#include "mygraphicsitem.h"



mygraphicsitem::mygraphicsitem(const QPixmap &pm) : QGraphicsPixmapItem(pm)
{

}

mygraphicsitem::mygraphicsitem()
{

}

QRectF mygraphicsitem::boundingRect() const
{
    return QGraphicsPixmapItem::boundingRect();
}
