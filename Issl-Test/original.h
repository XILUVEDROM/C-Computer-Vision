#ifndef ORIGINAL_H
#define ORIGINAL_H

#include <QMainWindow>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <mygraphicsitem.h>
#include <imagobject.h>
#include <stdlib.h>
#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <tuple>


namespace Ui {
class Original;
}

class Original : public QMainWindow
{
    Q_OBJECT

public:
    explicit Original(QWidget *parent = nullptr);
    ~Original();

    void PolOriginal(QImage fgaO);
      MyGraphicsItem *Origin;
      MyGraphicsItem *gpiOriginal;
    std::tuple<int , int, int> GiveOriginal(int x,int y);

private slots:
      void on_Pribl_clicked();

      void on_Otdal_clicked();

      void DiveOriginal();

      void on_Original_2_clicked();

private:
    Ui::Original *ui;

    QImage srcO;
    QImage tmpO;
    QImage dstO;

    QGraphicsScene scSrc;
    QGraphicsScene *sciAnzO;
    QGraphicsPixmapItem *itemAnzO;
    QGraphicsScene scOri;

    MyGraphicsItem *gpiSrcO;
    MyGraphicsItem *gpiDstO;
};

#endif // ORIGINAL_H
