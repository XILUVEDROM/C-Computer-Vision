#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "mygraphicsitem.h"
#include <QFileDialog>
#include <QVector>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include "centrmass.h"
#include <QListWidget>
#include <opencv/cv.h>
#include <opencv2/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    mygraphicsitem *gpiSrc;
    mygraphicsitem *gpiDst;

    IplImage* image =0 , *gray =0, *bin=0, *dsp=0;

     IplImage *QImage2IplImage(const QImage * image);

private slots:
    void on_MedianFilt_clicked();

    void on_horizontalSlider_valueChanged(int value);

    void on_actionOpenImage_triggered();

    void on_WorkImage_clicked();

    void peredacha(int width, int height);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;

    QImage src, dst;
    QGraphicsScene scSrc, scDst;
    QGraphicsScene *sciSrc, *sciDst;
};
#endif // MAINWINDOW_H
