#include "miniview.h"


MiniView::MiniView(QWidget *parent): QGraphicsView(parent)
{
    setScene(new QGraphicsScene);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MiniView::setScalableView(ImageView *sender)
{
    if (_sender != nullptr)
        _sender->disconnect(this);
    _sender = sender;
    coef_x=coef_y=-1;

    connect(_sender, SIGNAL(mouseEvent()), this, SLOT(areaChanged()));
    connect(_sender, SIGNAL(imageChange(const QPixmap&)), this, SLOT(setMinimap(const QPixmap&)));
    connect(this, SIGNAL(positionChanged(QPointF)), _sender, SLOT(focusOn(QPointF)));
}

void MiniView::setMinimap(const QPixmap &pixmap)
{
    scene()->clear();
    QPixmap minimap = pixmap.scaled(width(), height());
    scene()->setSceneRect(minimap.rect());
    scene()->addPixmap(minimap);
    coef_x = coef_y = -1;
    areaChanged();
}

void MiniView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    QPointF pos = {event->x() / coef_x, event->y() / coef_y};
    emit positionChanged(pos);
}

void MiniView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    QPointF pos = {event->x() / coef_x, event->y() / coef_y};
    emit positionChanged(pos);
}

void MiniView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    QPointF pos = {event->x() / coef_x, event->y() / coef_y};
    emit positionChanged(pos);
}


MiniView::~MiniView()
{
    delete scene();
}

void MiniView::areaChanged()
{
    if (scene()->items(Qt::AscendingOrder).size() > 1)
        scene()->removeItem(scene()->items(Qt::AscendingOrder).last());

    scene()->addRect(visibleAreaRect(), QPen(Qt::green));
    scene()->update();
}

QRectF MiniView::visibleAreaRect()
{
    QRectF area = _sender->mapToScene(0, 0, _sender->width(), _sender->height()).boundingRect();

    if (coef_x < 0 || coef_y < 0) {
        coef_x = (scene()->width()) / (_sender->scene()->width());
        coef_y = (scene()->height()) / (_sender->scene()->height());
    }

    qreal x, y, w, h;
    x = coef_x*area.x();
    y = coef_y*area.y();
    w = coef_x*area.width();
    h = coef_y*area.height();

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;
    if (w > scene()->width())
        w = scene()->width() - x;
    if (h > scene()->height())
        h = scene()->height() - y;

    return {x, y, w, h};
}
