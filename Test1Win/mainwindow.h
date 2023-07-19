#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QGraphicsScene>
#include "mygraphicsitem.h"
#include <algorithm>
#include <iostream>
#include <vector>
#include <iterator>


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
    void on_action_triggered();

    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_clicked();

private:


    QImage src;
    QImage dst;

    QGraphicsScene scSrc;
    QGraphicsScene *sciSrc; //для вывода в UI через GraphicsView
    QGraphicsScene scDst;
    QGraphicsScene *sciDst;


    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
