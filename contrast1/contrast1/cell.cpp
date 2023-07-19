#include "cell.h"
#include "mainwindow.h"

cell::cell()
{
    color = qRgb(255,255,255);
    xmax = 0;
    xmin = 0;
    ymax = 0;
    ymin = 0;
    change = 0;
    type = 0; //изначально тип неизвестен
    center = std::make_pair(0,0);
    edge.clear();
    //edge.append(std::make_pair(0,0));
    mask.clear();
    //mask.append(std::make_pair(0,0));
}

cell::cell(int x, int y, QRgb clr)
{
    color = clr;
    //связь id клетки и цвета заливки. number = QGreen(pixel)*256+QBlue(pixel);
    xmax = x;
    xmin = x;
    ymax = y;
    ymin = y;
    change = 0; //изначально тип неизвестен
    type = 0;
    center = std::make_pair(x,y);
    edge.clear();
    edge.append(std::make_pair(x,y));
    mask.clear();
    mask.append(std::make_pair(x,y));
}

void cell::checkEdge(QImage img) //передача изображения - костыль
{
    bool in = false;
    int x1, y1;
    for (int n = 0; n < edge.size(); n++)
    {
        in = true; //сразу предполагаем что точка внутренняя
        for (int i = 0; i < 4; i++)
        {   //проверяем четырех соседей
            if (i == 0) { x1 = edge[n].first+1; y1 = edge[n].second; }
            if (i == 1) { x1 = edge[n].first; y1 = edge[n].second+1; }
            if (i == 2) { x1 = edge[n].first-1; y1 = edge[n].second; }
            if (i == 3) { x1 = edge[n].first; y1 = edge[n].second-1; }

            if ( img.pixel(x1,y1)!=color )  { in = false; }
        }
        if ( in==true )     { edge.erase(edge.begin()+n); } //если внутренняя - удаляем из списка граничных
    }
}

void cell::addEdge(int x, int y)
{
    edge.append(std::make_pair(x, y));
    mask.append(std::make_pair(x, y));
}

void cell::paint(QImage basis, QRgb color)
{
    QImage nw(basis);
    for (int i = 0; i<mask.size(); i++)
    {
        nw.setPixel(mask[i].first, mask[i].second, color);
    }    
}
