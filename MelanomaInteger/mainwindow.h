#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QFileDialog>
#include <QPixmap>
#include <QFile>
#include <QGraphicsItem>

#include "algorithms.h"
#include "miniview.h"
#include <QMainWindow>
#include <QTableWidgetItem>
#include <QGraphicsScene>
#include "paintscene.h"
#include "label.h"
#include <QPaintDevice>
#include "paintscene.h"

#include "opencv/cv.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/ximgproc.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    Ui::MainWindow *ui;

    QImage Mat2QImage( cv::Mat const& src ) ;
    cv::Mat QImage2Mat( QImage const& src ) ;

    QImage MatToQImage(const cv::Mat& mat);
    cv::Mat QImageToMat(QImage &image);

private:
    void setForm();
    void setConnect();
    void setObjects();

private slots:
    void on_pushButton_clicked();
    void MainImage_RB_Clicked();

    void obrezka();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_open_im_clicked();

    cv::Mat otrez(cv::Mat image);

    cv::Mat mindlin( cv::Mat image);

    void on_jangsuen_clicked();

    void on_pushButtonPlus_clicked();

    void on_pushButtonMinus_clicked();

    void on_porog_sl_valueChanged(int value);

    void on_blocksize_sl_valueChanged(int value);

    void on_C_sl_valueChanged(int value);

    void on_porog_textChanged(const QString &arg1);

    void on_blocksize_textEdited(const QString &arg1);

    void on_C_textChanged(const QString &arg1);

    void on_Glob_image_clicked();

    void on_bl_gl_valueChanged(int value);

    void on_C_gl_valueChanged(int value);

    void on_bl_L_gl_textEdited(const QString &arg1);

    void on_C_L_gl_textChanged(const QString &arg1);

    void on_pushButton_Glob_clicked();

    void on_Giper_clicked();

    void on_G_porog_valueChanged(int value);

    void on_Kontr_clicked();

    void on_Toch_clicked();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:


    enum{segmentImg, areaImg, pointImg, darkAreaImg, cannyImg,
         borderImg, watershedImg, sobelImg, kmeansImg, laplaceImg,
         binarizationImg, meanShiftImg};

    int showNow = -1;
    bool isOriginal = true;

    QString fileName;

    QPixmap *original = nullptr;
    QPixmap *segmentation = nullptr;
    QPixmap *area = nullptr;
    QPixmap *point = nullptr;
    QPixmap *darkArea = nullptr;
    QPixmap *border = nullptr;
    QPixmap *watershed = nullptr;
    QPixmap *sobel = nullptr;
    QPixmap *kmeans = nullptr;
    QPixmap *laplace = nullptr;
    QPixmap *binarization = nullptr;
    QPixmap *meanShift = nullptr;

    QStringList pathFiles;

    PaintScene *mScene1;

    QGraphicsPixmapItem *nPixSrc;
    QGraphicsPixmapItem *nPixDst;

    QGraphicsScene scSrc;
    QGraphicsScene scDst;
};

#endif // MAINWINDOW_H
