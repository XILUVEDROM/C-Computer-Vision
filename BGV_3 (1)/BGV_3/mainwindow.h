#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <mygraphicsitem.h>
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QImage>
#include <QColor>
#include <math.h>
#include <QtMath>
#include <QFile>
#include <QtGui>
#include <string>
#include <iostream>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    MyGraphicsItem *gpiSrc;
    MyGraphicsItem *gpiDst;

private slots:
    void on_pushButton_clicked();

     void on_action_triggered();
     void on_verticalScrollBar_valueChanged(int value);

     void on_horizontalSlider_valueChanged(int value);

     void on_pushButton_2_clicked();

     void on_pushButton_3_clicked();
     void peregacha(int fga,int fgas);

     //void on_pushButton_4_clicked();

     void on_Assimetrya_clicked();

     void on_pushButton_5_clicked();

     void on_pushButton_14_clicked();

     void on_pushButton_6_clicked();

     void on_pushButton_7_clicked();

     void on_pushButton_8_clicked();

     void on_pushButton_9_clicked();

     void on_pushButton_10_clicked();

     void on_pushButton_11_clicked();

     void on_pushButton_12_clicked();

     void on_pushButton_13_clicked();

private:
    Ui::MainWindow *ui;

    QImage src;
    QImage dst;
     QImage tmp;

      QGraphicsScene scSrc;
      QGraphicsScene scDst;
      QGraphicsScene *sciSrc; //для вывода в UI через GraphicsView
      QGraphicsScene *sciDst;

       QImage kmean(int k, QImage img, QImage mask);
        QImage innerFill(QImage img);
        QImage distance(QImage bin, QRgb in, QRgb out);
        QImage Fill(QImage, int, int, QRgb, QRgb);
        QImage watershed1(QImage dist);
        QImage delBorders(QImage);
};
#endif // MAINWINDOW_H
