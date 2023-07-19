#ifndef MYGRAPHICSITEM_H
#define MYGRAPHICSITEM_H
#include <QGraphicsItem>
#include <QObject>
#include <QPainter>

//!!!на кой хрен оно вообще надо было - непонятно

namespace mine{
class MyGraphicsItem; //шаманство чтоб можно было ссылаться в реализации на обьект реализуемого класса. АХТУНГ КОСТЫЛЬ!!!
}

class MyGraphicsItem : public QGraphicsPixmapItem //элемент растрового изображения, который можн добавить в сцену
{
public:
    MyGraphicsItem( const QPixmap& pm);
    ~MyGraphicsItem();

    //!!!ХЗ как переопределить, будучи переопределенным пустышкой работает НЕВЕРНО
    //QRectF boundingRect() const;


private:
    mine::MyGraphicsItem *tmp;

};

#endif // MYGRAPHICSITEM_H

