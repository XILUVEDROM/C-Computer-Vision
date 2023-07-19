#ifndef VIEWER_H
#define VIEWER_H

#include <QWidget>
#include <QGraphicsView>
#include <QWheelEvent>
#include <QAction>

/*!
 * \brief Класс отображения изображений на форме
 *
 * Класс-наследник от QGraphicsView с переопределенным событием wheelEvent
 * для реализации функции масштабирования изображения прокруткой колеса мыши.
 */
class Viewer: public QGraphicsView
{
    Q_OBJECT
public:
    explicit Viewer(QWidget *parent = nullptr);
    ~Viewer();

signals:
    void changeZoom ();

public slots:
    void zoomIn();
    void zoomOut();

public:
    void wheelEvent(QWheelEvent *event);
    int count=5;
};

#endif // VIEWER_H
