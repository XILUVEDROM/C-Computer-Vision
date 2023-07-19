#ifndef CELL_H
#define CELL_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <vector>
#include <QRect>

class cell
{
public:
    cell();
    cell(int, int, QRgb);
    //~cell(); - пустой бесполезен.
    void setXmax(int i) { xmax = i; }
    void setXmin(int i) { xmin = i; }
    void setYmax(int i) { ymax = i; }
    void setYmin(int i) { ymin = i; }
    void setType(int i) { type = i; }

    void setAvgRed(int i) { avgRed = i; }
    void setAvgGreen(int i) { avgGreen = i; }
    void setAvgBlue(int i) { avgBlue = i; }

    void setID(int i) { id = i; }
    void setColor(QRgb clr)   { color=clr; }
    void setChange(bool t)  { change = t; }

    void addEdge(int x, int y); //добавление граничной точки
    void checkEdge(QImage img); //проверка границы на содержание неграничных точек

    void paint(QImage basis, QRgb color);

    std::pair<int,int> getEdge(int num) { return std::make_pair(edge[num].first, edge[num].second); }//обращение к элементу границы
    std::pair<int,int> getMask(int num) { return std::make_pair(mask[num].first, mask[num].second); }//обращение к элементу границы

    QRgb getColor()   { return color; }

    bool getChange()   { return change; }

    int getID()     { return id; }
    int getAvgRed() { return avgRed; }
    int getAvgGreen() { return avgGreen; }
    int getAvgBlue() { return avgBlue; }
    int getType()   { return type; }
    int edgeSize()  { return edge.size(); }
    int maskSize()  { return mask.size(); }

    QRect getBorder()   { return QRect(QPoint(xmin,ymax),QPoint(xmax,ymin)); } //topleft, bottomright

private:

    QRgb color;
    int xmax, xmin, ymax, ymin, id;
    int avgRed, avgGreen, avgBlue;
    int type; //0 - хз, 1 - норм, -1 - бласт.    
    std::pair<int,int> center;
    QVector<std::pair<int, int>> edge, mask;
    bool change; //костыль для эффективности алгоритмов        
};

#endif // CELL_H
