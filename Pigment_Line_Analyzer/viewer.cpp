#include "viewer.h"

Viewer::Viewer(QWidget *parent)
{

}
void Viewer::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
    {
        zoomIn();
    }
    if (event->angleDelta().y() < 0)
    {
        zoomOut();
    }
}

void Viewer::zoomIn()
{
    if (count<10)
    {
        scale(2, 2);
        count++;
        emit changeZoom();
    }
}

void Viewer::zoomOut()
{
    if (count>0)
    {
        scale(0.5, 0.5);
        count--;
        emit changeZoom();
    }
}

Viewer::~Viewer()
{

}
