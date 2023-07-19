#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "obrabotkaalg.cpp"

#include <QMainWindow>
#include <QGraphicsScene>
#include <mygraphicsitem.h>
#include <imagobject.h>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QStandardPaths>
#include <centrmass.h>
#include <tuple>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void MedianF(QImage &dst,int r);

    MyGraphicsItem *gpiSrc;
    MyGraphicsItem *gpiDst;
    MyGraphicsItem *gpiOriginal;

    IplImage *QImage2IplImage(const QImage * image);

        void Peredaxa(QImage fga);

        void FoundYellow();

        void FoundBlue();

        void FoundDBlue();

        void FoundDDblue();

        void PolOridKoor(int x,int y);

        QString Pertfunc();

        MyGraphicsItem *gpiAnz;
      MyGraphicsItem *PolOrig(MyGraphicsItem *pereg);
signals:
    void sendData(QString str,uint g1,uint g2,float g3,uint g4,uint g5);
    void sendData1(uint g,uint g1,uint g2,uint g3);
    void sendData2(QString str);
    void sendData3(QString str);
    void sendData4(QString str);
    void sendData5(QString str);
private slots:
    void on_action_triggered();

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalSlider_2_valueChanged(int value);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_action_2_triggered();

    void on_pushButton_4_clicked();

    void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

    void on_pushButton_5_clicked();

    QColor resolveColor(const QString &name);

     void onButtonSend();
     void onButtonSend1();
     void onButtonSend2(){emit sendData2("НЕ выполнена функция на симметрию");}
    void onButtonSend3(){emit sendData3("НЕ выполнена функция на Прерывание пигементой сети");}
    void onButtonSend4(){emit sendData4("НЕ выполнена функция на Цвет");}
    void onButtonSend5() {emit sendData5("Не выполнена func на структруные комненты");}
    void on_pushButton_6_clicked();

    void Test();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();


    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_17_clicked();

    void on_horizontalSlider_3_actionTriggered(int action);

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_22_clicked();

    void ObjectDat();

    void PaintBorders();

    void PaintCountours();

    void StoreCountours();

    void RemoveAreas();

    void FillAreaInArea(QImage& img, int x, int y, ImgObject& object);

    void FindClosestWhitePixel(QImage img, int& x, int& y);

    void Purify();

    void peredacha(int vec,int vecs);

    bool CheckNearbyPixels(int red, int green,int blue, int i, int j);

    void on_pushButton_23_clicked();

    void on_pushButton_24_clicked();


    void on_pushButton_25_clicked();

    void on_pushButton_26_clicked();

    void on_pushButton_27_clicked();

    void on_pushButton_28_clicked();


    void on_pushButton_29_clicked();

    void on_pushButton_30_clicked();

    void on_pushButton_31_clicked();

    void on_pushButton_32_clicked();

    void on_pushButton_33_clicked();

    void on_pushButton_34_clicked();

    void on_pushButton_35_clicked();

    void on_pushButton_36_clicked();

    void on_horizontalSlider_4_actionTriggered(int action);

    void on_pushButton_37_clicked();

    void on_pushButton_38_clicked();

    void on_pushButton_39_clicked();

    void Obrabotka();

    void Ochistka();

    void on_Osi_clicked();

    void peregacha(int fga,int fgas);

    void findCircles(IplImage* _image);

    void on_pushButton_41_clicked();

    void on_Ochistka_clicked();

    void on_pushButton_40_clicked();

    void EnclosingCircle(IplImage* _image);

    void on_Original_clicked();

    void FindAssimtrya();

    void on_Assimetrya_clicked();

    void on_FDM_clicked();

    void FoundTochka(int i,int j);

    void on_findAreas_clicked();

    void FoundAreas(int i, int j);

    void ObrabotkaBlye();

    void OnrabotkaYellow();

    void ObrabotkaDBlue();

    void ObrabotkaDDBlue();

    void obrabotYel(int i,int j);

    void on_pushButton_42_clicked();

    void on_FDO_clicked();

    void on_Nf_clicked();

    void on_testyel_clicked();

    void on_pushButton_43_clicked();

    void on_pushButton_44_clicked();

    void on_pushButton_45_clicked();

private:
    Ui::MainWindow *ui;

    QImage src;
    QImage tmp;
    QImage dst;
    QImage bin;
    QImage flood;
    QImage nin;

    QGraphicsScene scSrc;
    QGraphicsScene scDst;
    QGraphicsScene *sciSrc; //для вывода в UI через GraphicsView
    QGraphicsScene *scTmp;
    QGraphicsScene *sciDst;

    QGraphicsScene *sciAnz;
    QGraphicsPixmapItem *itemAnz;

     QImage watershed1(QImage dist);
     QImage delBorders(QImage);
     QImage distance(QImage bin, QRgb in, QRgb out);
     QImage innerFill(QImage img);
     QImage Fill(QImage, int, int, QRgb, QRgb);
     QImage kmean(int k, QImage img, QImage mask);
};



#endif // MAINWINDOW_H
