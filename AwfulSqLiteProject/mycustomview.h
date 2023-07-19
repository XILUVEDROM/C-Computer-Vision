#ifndef MYCUSTOMVIEW_H
#define MYCUSTOMVIEW_H
#include <QMouseEvent>
#include <QWheelEvent>


class MyCustomView
{
public:
    MyCustomView();
protected:
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
};

#endif // MYCUSTOMVIEW_H
