#ifndef PAINTSCENE_H
#define PAINTSCENE_H

// Для создания сцены выделения

#include <QGraphicsScene>
#include "figure.h"

class PaintScene : public QGraphicsScene
{
    Q_OBJECT

public:
   explicit PaintScene( QObject *parent = 0 ) ;
    ~PaintScene() {}

    Figure *getFigure() const ;

    void setVisibleFigure( bool b ) ;

    void createFigure( QVector<QPointF> p ) ;

    double mx, my, mwidth, mheigh;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) ;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) ;


private:
    Figure *mTempFigure ;

signals:
    void mouseReleased();

};

#endif // PAINTSCENE_H
