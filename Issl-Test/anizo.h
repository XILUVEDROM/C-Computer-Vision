#ifndef ANIZO_H
#define ANIZO_H

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

namespace Ui {
class anizo;
}

class anizo : public QMainWindow
{
    Q_OBJECT

public:
    explicit anizo(QWidget *parent = nullptr);
    ~anizo();

    void Peredaxa(QImage fga);
      MyGraphicsItem *gpiAnz;
      MyGraphicsItem *Origin;
      MyGraphicsItem *gpiOriginal;
    MyGraphicsItem *PolOrig(MyGraphicsItem *pereg);
    void Vivod1st(float st1,float st2 );
    void Vivod1stS(float st1,float st2 );
    void Vivod2nd(float st2,float st1);
    void Vivod2ndS(float st2,float st1);
    void Vivod3ed(float st3,float st2);
    void Vivod3edS(float st3,float st2);
    void Colorper(bool f1,bool f2, bool f3, bool f4, bool f5, bool f6, bool f7, bool f8, bool f9, bool f10, bool f11, bool f12,bool f13,bool f14,bool f15, bool f16,bool f17,bool f18);

private slots:
       void on_action_triggered();

    void on_Otdal_clicked();

    void on_Pribl_clicked();

    void on_pushButton_clicked();

private:
    Ui::anizo *ui;

    QImage src;
    QImage tmp;
    QImage dst;

  //  QGraphicsScene *scene;
  //  QGraphicsPixmapItem *item;
  //  QGraphicsScene *scene2;
  //  QGraphicsPixmapItem *item2;


 //для вывода в UI через GraphicsView
    QGraphicsScene *sciAnz;
    QGraphicsPixmapItem *itemAnz;


    QGraphicsScene scOri;
    MyGraphicsItem *gpiSrc;
    MyGraphicsItem *gpiDst;


};

#endif // ANIZO_H
