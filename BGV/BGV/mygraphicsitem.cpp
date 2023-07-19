#include "mygraphicsitem.h"

MyGraphicsItem::MyGraphicsItem(const QPixmap &pm) : QGraphicsPixmapItem(pm)
{

}

MyGraphicsItem::~MyGraphicsItem()
{

}

QRectF MyGraphicsItem::boundingRect() const
{
    return QGraphicsPixmapItem::boundingRect();
}
