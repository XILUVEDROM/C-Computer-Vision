#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>

/*! \ingroup Widgets
 * \brief Окно с изображением.
 * \details Для данного окна (QGraphicsView) можно увеличивать, уменьшать изображение,
 * а также смещать его зажав левую кнопку мыши.
 *
 */
class ImageView:public QGraphicsView
{
    Q_OBJECT
public:
    ImageView(QWidget *parent): QGraphicsView(parent)
    {
        setDragMode(QGraphicsView::ScrollHandDrag);
    }
    void addPixmap(const QPixmap& pixmap)
    {
        if (scene() != nullptr)
        {
            scene()->clear();
            scene()->setSceneRect(pixmap.rect());
            scene()->addPixmap(pixmap);
            scene()->update();
            emit imageChange(pixmap);
        }
    }

public slots:

    void wheelEvent(QWheelEvent *event)
    {
        qreal delta = event->delta();
        qreal coef = 1. + delta / 800.;
        scale(coef, coef);
        centerOn(mapToScene(event->pos()));
        emit mouseEvent();
    }

    void mousePressEvent(QMouseEvent *event)
    {
        QGraphicsView::mousePressEvent(event);
        emit mouseEvent();
        if(event->button() == Qt::RightButton)
            emit rightButtonClicked();

    }

    void mouseMoveEvent(QMouseEvent *event)
    {
        QGraphicsView::mouseMoveEvent(event);
        emit mouseEvent();
    }

    void focusOn(QPointF p)
    {
        centerOn(p);
        emit mouseEvent();
    }


signals:
    void imageChange(const QPixmap &pixmp);
    void mouseEvent();
    void rightButtonClicked();
private:
    QPoint last;
};


#endif // IMAGEVIEW_H
