#include "paintscene.h"
#include "romb.h"
#include "triangle.h"
#include "square.h"
#include "line.h"

PaintScene::PaintScene(QObject *parent) : QGraphicsScene(parent)
{

}

PaintScene::~PaintScene()
{

}

int PaintScene::typeFigure() const
{
    return m_typeFigure;
}

void PaintScene::setTypeFigure(const int type)
{
    m_typeFigure = type;
}

void PaintScene::setDepthFigure(const int newDepth)
{
    depth = newDepth;
}

void PaintScene::setColorFigure(const QColor newColor)
{
    color = newColor;
}

void PaintScene::removeLastFigure()
{
    tempFigure = FigureList.back();
    FigureList.pop_back();
    this->removeItem(tempFigure);
}

void PaintScene::removeALlFigure()
{
    while(!FigureList.empty()){
        this->removeItem(FigureList.back());
        FigureList.pop_back();
    }
}

void PaintScene::restoreFigure()
{
    if(tempFigure != nullptr){
        FigureList.push_back(tempFigure);
        this->addItem(tempFigure);
        delete tempFigure;
        tempFigure = nullptr;
    }
}

void PaintScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    /* Устанавливаем конечную координату положения мыши
     * в текущую отрисовываемую фигуру
     * */
    tempFigure->setEndPoint(event->scenePos());
    /* Обновляем содержимое сцены,
     * необходимо для устранения артефактов при отрисовке фигур
     * */
    this->update(QRectF(0,0,this->width(), this->height()));
}

void PaintScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    tempFigure = nullptr;
    emit QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

/* Как только нажали кнопку мыши, создаём фигуру одного из трёх типов
 * и помещаем её на сцену, сохранив указатель на неё в переменной
 * tempFigure
 * */
void PaintScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    delete tempFigure;
    switch (m_typeFigure) {
    case SquareType: {
        tempFigure = new Square(event->scenePos());
        break;
    }
    case RombType: {
        tempFigure = new Romb(event->scenePos());
        break;
    }
    case TriangleType: {
        tempFigure = new Triangle(event->scenePos());
        break;
    }
    case LineType: {
        //tempFigure = new Line(event->scenePos());
        break;
    }
    default:{
        tempFigure = new Square(event->scenePos());
        break;
    }
    }
    tempFigure->setPos(event->pos());
    tempFigure->setColor(color);
    tempFigure->setDepth(depth);
    FigureList.push_back(tempFigure);
    this->addItem(tempFigure);
}
