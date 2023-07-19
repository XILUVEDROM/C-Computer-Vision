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

    // void on_pushButton_4_clicked();

     void on_Assimetrya_clicked();

private:
    Ui::MainWindow *ui;

    QImage src;
    QImage dst;

      QGraphicsScene scSrc;
      QGraphicsScene scDst;
      QGraphicsScene *sciSrc; //для вывода в UI через GraphicsView
      QGraphicsScene *sciDst;
};
#endif // MAINWINDOW_H
