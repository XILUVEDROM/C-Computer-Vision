#include "paintscene.h" // Для создания прямоугольников выделения

#include <QGraphicsSceneMouseEvent>

#include "square.h"
#include <QDebug>

PaintScene::PaintScene( QObject *parent )
    : QGraphicsScene( parent )
{

}

Figure* PaintScene::getFigure() const
{
    return mTempFigure ;
}

void PaintScene::setVisibleFigure(bool b)
{
    mTempFigure->setVisible(b) ;
    update() ;
}

void PaintScene::createFigure(QVector<QPointF> p)
{
    Square *item =  new Square( p[0]  ) ;
    item->setStartPoint( p[0] ) ;
    item->setEndPoint( p[1] );
    mTempFigure = item ;
    this->addItem( mTempFigure ) ;
    update() ;
}

void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Square *item =  new Square( event->scenePos() ) ;
    item->setStartPoint( event->scenePos() ) ;
    mTempFigure = item ;
    this->addItem( mTempFigure ) ;
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    mTempFigure->setEndPoint( event->scenePos() );
    this->update( QRectF(0, 0, this->width(), this->height() ) );

}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    mTempFigure->startPoint();
    mTempFigure->endPoint();

    mx = qMin(mTempFigure->startPoint().x(),mTempFigure->endPoint().x());
    my = qMin(mTempFigure->startPoint().y(),mTempFigure->endPoint().y());

    mwidth = qAbs(mTempFigure->startPoint().x() - mTempFigure->endPoint().x());
    mheigh = qAbs(mTempFigure->startPoint().y() - mTempFigure->endPoint().y());

    emit mouseReleased();
    setVisibleFigure(false);
}

