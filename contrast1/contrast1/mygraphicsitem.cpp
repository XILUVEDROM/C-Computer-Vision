#include "mygraphicsitem.h"

//!!!на кой хрен оно вообще надо было - непонятно

MyGraphicsItem::MyGraphicsItem (const QPixmap& pm): QGraphicsPixmapItem(pm)
{

}
 MyGraphicsItem::~MyGraphicsItem()
 {

 }

//!!!ДАЛЬШЕ КАКАЯ-ТО ХРЕНЬ. Хз как переопределить да и непонятно зачем
 /*
 QRectF MyGraphicsItem::boundingRect() const
 {
    QRectF *Rect = tmp->QGraphicsPixmapItem::boundingRect();

    return *Rect;
 }
*/
 //!!!выводило кусок MyGraphicsItem из-за нереализованного, но перезагруженного BoundingRect
 //!!!BoundingRect описывает ограничениеобласти отображения на сцене

