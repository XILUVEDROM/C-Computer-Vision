#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QDebug>

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>

#include "base.h"
#include "features.h"
#include "form.h"
#include "imagematrix.h"
#include "processing.h"
#include "qcustomplot.h"
#include "viewer.h"
#include "skelet/skiletizationfilter.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();    

signals:
    void loopCount(int);

private:    
    void createMenu();
    void createTool();
    void calculateSymmetry();

    Ui::MainWindow *ui;
    QString file_name;
    QGraphicsScene *scene;
    QImage img, proc, proc_s, buf, dst;
    bool reprocess, ttt;
    Base *db;
    Form *form;
    QCPBars *regen;
    QCPColorScale *colorScale;
    QCPColorMap *colorMap;
    QCustomPlot *cp;
    std::vector<QString> points;
    std::vector<std::vector<int>> features;
    std::vector<std::vector<QString>> blocks;
    std::vector<std::vector<int>> ranges;
    int centrx = 0, centry =0, DlinaSegmenta = 0, RadiusCircle =0;

private slots:
    void openImage();
    void process();
    void removeSmall();
    void findSegments();
    void saveFeatures();
    void prepareHistogram();
    void changeProgressBar(int);
    void mousePressOnPlot(QCPAbstractPlottable *plottable, int dataIndex, QMouseEvent  *event);
    void showContour(bool);
    void showMask(bool);
    void showMap(bool);
    void openForm();

    void Proxodpofunc();
    void on_pushButton_6_clicked();
};

#endif // MAINWINDOW_H
