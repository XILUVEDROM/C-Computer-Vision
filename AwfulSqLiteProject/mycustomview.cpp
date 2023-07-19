#include "mycustomview.h"
#include <QtGui>
#include <QGraphicsView>
#include <QMouseEvent>

class CustomView : public QGraphicsView
{
protected:
    void mousePressEvent(QMouseEvent *event)
    {
        qDebug() << "Custom view clicked.";
        QGraphicsView::mousePressEvent(event);
    }

    void wheelEvent(QWheelEvent *event)
    {
        QGraphicsView::wheelEvent(event);
    }
};


