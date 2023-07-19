#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "mygraphicsitem.h"
#include "cell.h"
#include "tablet.h"

#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void MedianF(QImage &dst, int r);
    void cellScan();
    void showSrc(QImage img) { scSrc->clear(); scSrc->addPixmap(QPixmap::fromImage(img)); }
    void defineCellsType();

    QImage invert(QImage bin);
    QImage Fill(QImage, int, int, QRgb, QRgb);
    QImage addBorders(QImage, QRgb);
    QImage delBorders(QImage);
    QImage kmean(int k, QImage img, QImage mask);
    QImage distance(QImage bin, QRgb in, QRgb out);
    QImage watershed1(QImage dist);
    QImage innerFill(QImage img);
    QImage filterEdge(QImage img); //исключаем клетки у краев.
    QImage filterArea(QImage img); //исключаем мелкие клетки.
    QImage getSrc() { return src; }
    QImage getDst() { return dst; }

    QRgb generateColor(int N);       //цвет из номера
    int degenerateColor(QRgb color); //номер из цвета
    int getN() { return cells.size(); }        //число клеток
    int getBlast() { return blast; }
    int getNorm() { return normal; }
    int cellNumberFromId(int id);
    int cellNumberFromColor(QRgb color);

    cell getCell(int number) { return cells[number]; }

    void gSize();
    void gGreen();
    void showSize();
    void showGreen();

    void edgeFilter();
    void sizeFilter();

    int getAvgColor(cell temp, int mode);
    void setAvgColor(int number, int mode, int color);    

    void setTrsGreen(int i) { trsGreen = i; }   //для установки через бегунок
    void setTrsSize(int i) { trsSize = i; }     //для установки через бегунок
    void setTrsBin(int i)  { trsBin = i; }      //для установки через бегунок
    void setMinSize(int i) { minSize = i; }     //для установки через бегунок

private slots:

    void on_action_triggered();

    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_horizontalSlider_4_valueChanged(int value);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_7_clicked();    
    void on_pushButton_8_clicked();    
    void on_pushButton_9_clicked();
    void on_pushButton_10_clicked();
    void on_pushButton_11_clicked();    
    void on_pushButton_12_clicked();
    void on_pushButton_13_clicked();
    void on_pushButton_14_clicked();

private:

    Ui::MainWindow *ui;

    QGraphicsScene *scSrc; //для вывода в UI через GraphicsView
    QGraphicsScene *scTmp;
    QGraphicsScene *scDst;

    QImage src; //исходное изображение
    QImage tmp; //промежуточные
    QImage dst; //финальное
    QImage bin; //бинарник
    QImage flood;//результаты преобразования расстояний и водораздела    

    QVector<cell> cells; //общее хранилище.
    QVector<int> gistoSize, gistoGreen;//массивы для гистограмм

    int N, normal, blast; //общее число клеток, нормальных и бластов.
    float ratio; //отношение клеток

    int maxSize = 0, minSize = 2500, trsBin = 0, trsSize=0, trsRed=256, trsBlue=256, trsGreen=0; //изначально ничего не отбрасывается.
};

#endif // MAINWINDOW_H
