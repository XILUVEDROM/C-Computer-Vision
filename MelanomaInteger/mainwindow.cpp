#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QPainter>
#include <QDebug>
#include <QScrollArea>
#include <QScrollBar>

#include "opencv/cv.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/imgcodecs.hpp"
#include <opencv2/core/mat.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/ximgproc.hpp>
#include <opencv2/ximgproc/segmentation.hpp>

#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->dstGraphicsView->setScene( &scSrc ) ;
    ui->dstGraphicsView->setScene( &scDst ) ;

/*    connect(ui->srcGraphicsView->verticalScrollBar(), SIGNAL(sliderMoved(int)), ui->dstGraphicsView->verticalScrollBar(),  SLOT(setValue(int)));
    connect(ui->dstGraphicsView->verticalScrollBar(), SIGNAL(sliderMoved(int)), ui->srcGraphicsView->verticalScrollBar(),SLOT(setValue(int)));

    connect(ui->srcGraphicsView->horizontalScrollBar(), SIGNAL(sliderMoved(int)), ui->dstGraphicsView->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->dstGraphicsView->horizontalScrollBar(), SIGNAL(sliderMoved(int)), ui->srcGraphicsView->horizontalScrollBar(), SLOT(setValue(int)));
*/
    setObjects();
    setForm();
    setConnect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setForm()
{
    ui->mainImageShow->setScene(new QGraphicsScene);
    ui->minimapShow->setScalableView(ui->mainImageShow);
}

void MainWindow::setConnect()
{
    connect(ui->mainImageShow, SIGNAL(rightButtonClicked()),
            this, SLOT(MainImage_RB_Clicked()));
}

void MainWindow::setObjects()
{

}

//_________________________________________________________________________________
QImage White_Pic, Source, Needed_Area, After_Contrast, After_Median_Filter, After_Median_Filter_Bin, After_Binar, Pre_Contoured, Contoured; //для хранения промежуточных результатов
QImage White_Pic_G, Source_G, Needed_Area_G, After_Contrast_G, After_Median_Filter_G, After_Median_Filter_Bin_G, After_Binar_G, Pre_Contoured_G, Contoured_G; //для хранения промежуточных результатов
int w = 0, h = 0 ;  //размеры изображения (ширина, высота)
bool opened = 0 ;   //индикатор того, что файл открыт

double K_Contrast = 0.9; //Коэффициент преобразования контраста по умолчанию

int N1 = 40;    //размер скользящего окна по умолчанию
double porog_percents = 10;    //% от средней яркости - начальное значение порога бинаризации
int porog_levels = 5;    //уровней яркости от средней яркости - начальное значение порога бинаризации
bool binarized = 0 ;    //было бинаризовано
QImage Gray_for_binarize;
QImage Gray_for_binarize_G;

int colour_contour = 0;    //индекс цвета контура для выделения комков
int cont_red = 173 ;
int cont_green = 216 ;
int cont_blue = 230 ;
QImage SaveBandIm;  //белое полотно для рисования на нем контуров комков и дальнейшего сохранения
QImage SaveBandIm_G;
QImage FiltWalk;  //фильтрация для скользящего (хранит границы выделенных областей)
QImage FiltWalk_G;
QImage KomkiWalk;  //для скользящего (хранит границы выделенных областей до фильтрации)
QImage KomkiWalk_G;
QImage Filtered;
QImage Filtered_G;
bool contoured = 0;

void MainWindow::on_Glob_image_clicked()
{
    scSrc.clear();
    QString fp = QFileDialog::getOpenFileName() ;
    QPixmap SaveBand;
    nPixSrc = new QGraphicsPixmapItem( QPixmap(fp) ) ;
    nPixDst = new QGraphicsPixmapItem( QPixmap(fp) ) ;

    scSrc.addItem( nPixSrc ) ;
    scDst.addItem( nPixDst ) ;
    binarized = 0 ;
    contoured = 0 ;

    SaveBand = nPixSrc->pixmap() ;
    SaveBandIm = SaveBand.toImage() ;   //определение картинки с белым фоном
    Source = SaveBandIm ; //сохраняем оригинал изображения
    Needed_Area = SaveBandIm ;
    After_Contrast = SaveBandIm ;
    After_Median_Filter = SaveBandIm ;
    After_Binar = SaveBandIm ;
    Contoured = SaveBandIm ;
    Pre_Contoured = SaveBandIm ;
    Gray_for_binarize = SaveBandIm ;

    w = SaveBand.width() ;
    h = SaveBand.height() ;

    for (int x=0; x<w; ++x)
        for (int  y=0; y<h; ++y)
        {
            SaveBandIm.setPixel(x,y, qRgb(255,255,255));    //заполнить все белым
        }
    White_Pic = SaveBandIm ;
    FiltWalk = SaveBandIm ;
    KomkiWalk = SaveBandIm ;

    //Выделим область интереса
    QImage src = Source ;//nPixSrc->pixmap().toImage() ;

    QImage dst(src);

    for (int x=0; x<w; ++x)
        for (int  y=0; y<h; ++y)
        {
            if (qGreen(src.pixel(x,y))> 128)      //бинаризация по зеленой компоненте
            {
                dst.setPixel(x,y, qRgb(0,0,0));
            }
            else
                dst.setPixel(x,y, qRgb(255,255,255));
        }

    for (int x=0; x<w; ++x)
        for (int  y=0; y<h; ++y)
            if(qRed(dst.pixel(x,y)) == 255)//0 или 255 в зависимости от использования предобработки (255 с использованием HSV)
            {
                dst.setPixel(x,y,src.pixel(x,y));
            }
            else
            {
                dst.setPixel(x,y,qRgb(0,0,0));//цвет фона (чёрный, белый)
            }


    nPixDst->setPixmap( QPixmap::fromImage(dst) ) ;
    Needed_Area = dst ; //сохраняем изображение с выделенной областью интереса
    After_Contrast = dst ;
    After_Median_Filter = dst ;
    opened = 1 ;

    QImage temp = Needed_Area ;

    int w = temp.width()  ;
    int h = temp.height() ;

    for( int y = 0 ; y < h ; ++y )
    {
        for( int x = 0 ; x < w ; ++x )
        {
            int r, g, b, h, s, v ;
            QColor tempRGB = QColor::fromRgb(temp.pixel(x,y) ) ;
            tempRGB.getRgb(&r, &g, &b) ;
            if( (r != 0) && (g != 0) && (b != 0))
            {
                QColor tempHSV = tempRGB.toHsv() ;

                tempHSV.getHsv(&h, &s, &v) ;

                v = int(255*pow( (double(v)/255) , 0.6 ) ) ;      //v * Kbr ;
                if(v > 255)
                    v = 255;

                tempHSV.setHsv(h, s, v) ;

                tempRGB = tempHSV.toRgb() ;
                tempRGB.getRgb(&r, &g, &b) ;
                temp.setPixel(x, y, qRgb( r, g, b) );
            }
        }
    }

    nPixDst->setPixmap(QPixmap::fromImage(temp)) ;
    After_Contrast = temp ;
    After_Median_Filter = temp ;

    QImage tempf = nPixDst->pixmap().toImage() ;

    QVector<QRgb> vec;

    for( int y = 1 ; y < (h - 1) ; ++y )
    {
        for ( int x = 1 ; x < (w - 1) ; ++x)
        {
            for( int i = -1 ; i <= 1; ++i)
                for( int j = -1 ; j <= 1; ++j)
                {
                    vec.append( QColor( tempf.pixel( x + i, y + j )).rgb() ) ;
                }
            std::sort( vec.begin(), vec.end() ) ;
            tempf.setPixel( x, y, vec.at(4) ) ;
            vec.clear() ;
        }
    }

    nPixDst->setPixmap(QPixmap::fromImage(tempf)) ;
    After_Median_Filter = tempf;

    //mScene1 = new PaintScene(this);
    //mScene1->addItem(nPixDst);
    //ui->mainImageShow->setScene(mScene1);

    ui->mainImageShow->setScene(&scSrc);
}

void MainWindow::on_Giper_clicked()
{
    scSrc.clear();
    QString fp = QFileDialog::getOpenFileName() ;
    QPixmap SaveBand;
    nPixSrc = new QGraphicsPixmapItem( QPixmap(fp) ) ;
    nPixDst = new QGraphicsPixmapItem( QPixmap(fp) ) ;

    scSrc.addItem( nPixSrc ) ;
    scDst.addItem( nPixDst ) ;
    binarized = 0 ;
    contoured = 0 ;

    SaveBand = nPixSrc->pixmap() ;
    SaveBandIm_G = SaveBand.toImage() ;   //определение картинки с белым фоном
    Source_G = SaveBandIm_G ; //сохраняем оригинал изображения
    Needed_Area_G = SaveBandIm_G ;
    After_Contrast_G = SaveBandIm_G ;
    After_Median_Filter_G = SaveBandIm_G ;
    After_Binar_G = SaveBandIm_G ;
    Contoured_G = SaveBandIm_G ;
    Pre_Contoured_G = SaveBandIm_G ;
    Gray_for_binarize_G = SaveBandIm_G ;

    w = SaveBand.width() ;
    h = SaveBand.height() ;

    for (int x=0; x<w; ++x)
        for (int  y=0; y<h; ++y)
        {
            SaveBandIm_G.setPixel(x,y, qRgb(255,255,255));    //заполнить все белым
        }
    White_Pic_G = SaveBandIm_G ;
    FiltWalk_G = SaveBandIm_G ;
    KomkiWalk_G = SaveBandIm_G ;

    //Выделим область интереса
    QImage src = Source_G ;//nPixSrc->pixmap().toImage() ;
    QImage src_G = Source_G;
    src_G = src_G.convertToFormat(QImage::Format_Grayscale8);
    QImage dst(src);

    for (int x=0; x<w; ++x)
        for (int  y=0; y<h; ++y)
        {
            if (qGreen(src.pixel(x,y)) > 20)      //бинаризация по зеленой компоненте
            {
                dst.setPixel(x,y, qRgb(0,0,0));
            }
            else
                dst.setPixel(x,y, qRgb(255,255,255));
        }

    for (int x=0; x<w; ++x)
        for (int  y=0; y<h; ++y)
            if(qRed(dst.pixel(x,y)) == 255)//0 или 255 в зависимости от использования предобработки (255 с использованием HSV)
            {
                dst.setPixel(x,y,qRgb(255,255,255));
            }
            else
            {
                dst.setPixel(x,y,src.pixel(x,y));//цвет фона (чёрный, белый)
            }


    nPixDst->setPixmap( QPixmap::fromImage(dst) ) ;
    Needed_Area_G = dst ; //сохраняем изображение с выделенной областью интереса
    After_Contrast_G = dst ;
    After_Median_Filter_G = dst ;
    opened = 1 ;

    ui->mainImageShow->setScene(&scSrc);

}

void MainWindow::on_G_porog_valueChanged(int value)
{
    QString qval = QString::number(value);
    ui->Lab_gp->setText(qval);
    if(opened == 1)
    {
        QImage src = Source_G;
        QImage src_G = Source_G;
        src_G = src_G.convertToFormat(QImage::Format_Grayscale8);

        QImage dst(src);
        QImage st(src);


        for (int x=0; x<w; ++x)
            for (int  y=0; y<h; ++y)
            {
                if (qGreen(src.pixel(x,y)) > value)      //бинаризация по зеленой компоненте
                {
                    dst.setPixel(x,y, qRgb(0,0,0));
                }
                else
                    dst.setPixel(x,y, qRgb(255,255,255));
            }

        for (int x=0; x<w; ++x)
            for (int  y=0; y<h; ++y)
                if(qRed(dst.pixel(x,y)) == 255)//0 или 255 в зависимости от использования предобработки (255 с использованием HSV)
                {
                    dst.setPixel(x,y,src.pixel(x,y));
                    st.setPixel(x,y,qRgb(255,255,255));
                }
                else
                {
                    dst.setPixel(x,y,src_G.pixel(x,y));//цвет фона (чёрный, белый)
                    st.setPixel(x,y,src.pixel(x,y));
                }


        nPixDst->setPixmap( QPixmap::fromImage(st) ) ;
        Needed_Area_G = st ; //сохраняем изображение с выделенной областью интереса
        After_Contrast_G = st ;
        After_Median_Filter_G = st ;

        mScene1 = new PaintScene(this);
        mScene1->addItem(nPixDst);
        ui->mainImageShow->setScene(mScene1);
    }
}

void MainWindow::on_Kontr_clicked()
{
    QImage tempf = nPixDst->pixmap().toImage() ;

    QVector<QRgb> vec;

    for( int y = 1 ; y < (h - 1) ; ++y )
    {
        for ( int x = 1 ; x < (w - 1) ; ++x)
        {
            for( int i = -2 ; i <= 2; ++i)
                for( int j = -2 ; j <= 2; ++j)
                {
                    vec.append( QColor( tempf.pixel( x + i, y + j )).rgb() ) ;
                }
            std::sort( vec.begin(), vec.end() ) ;
            tempf.setPixel( x, y, vec.at(12) ) ;
            vec.clear() ;
        }
    }

    nPixDst->setPixmap(QPixmap::fromImage(tempf)) ;
    After_Median_Filter_G = tempf;

    QImage cont = nPixDst->pixmap().toImage() ;
    QImage dst(cont);
    QImage st(cont);
    QImage src = Source_G;

    for (int x=0; x<w - 1; ++x)
        for (int  y=0; y<h - 1; ++y)
            if((qRed(dst.pixel(x,y)) == 255) && ((qRed(dst.pixel(x + 1, y)) != 255)
                                              || (qRed(dst.pixel(x + 1 ,y + 1)) != 255)
                                              || (qRed(dst.pixel(x, y + 1)) != 255)
                                              || (qRed(dst.pixel(x - 1 ,y + 1)) != 255)
                                              || (qRed(dst.pixel(x - 1 ,y)) != 255)
                                              || (qRed(dst.pixel(x - 1 ,y - 1)) != 255)
                                              || (qRed(dst.pixel(x ,y - 1)) != 255)
                                              || (qRed(dst.pixel(x + 1 ,y - 1)) != 255)
                                              || (qRed(dst.pixel(x + 1,y)) != 255)))//0 или 255 в зависимости от использования предобработки (255 с использованием HSV)
            {
                st.setPixel(x,y,qRgb(209, 237, 242));
            }
            else
            {
                st.setPixel(x,y,src.pixel(x,y));
            }

    nPixDst->setPixmap(QPixmap::fromImage(st)) ;
    mScene1 = new PaintScene(this);
    mScene1->addItem(nPixDst);
    ui->mainImageShow->setScene(mScene1);

}

//_________________________________________________________________________________

void MainWindow::on_pushButton_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Select one or more files to open",
                                                    "/home", "Images (*.png *.xpm *.jpg *.bmp)");
    if (fileName.isEmpty())
        return;
    if (original != nullptr) {
        delete original;
        original = nullptr;
    }
    if (segmentation != nullptr) {
        delete segmentation;
        segmentation = nullptr;
    }
    if (area != nullptr) {
        delete area;
        area = nullptr;
    }
    if (point != nullptr) {
        delete point;
        point = nullptr;
    }
    if (darkArea != nullptr){
        delete darkArea;
        darkArea = nullptr;
    }
    if (border != nullptr){
        delete border;
        border = nullptr;
    }
    if (binarization != nullptr){
        delete binarization;
        binarization = nullptr;
    }
    if (meanShift != nullptr){
        delete meanShift;
        meanShift = nullptr;
    }

    original = new QPixmap(fileName);
    ui->mainImageShow->addPixmap(*original/*QPixmap().fromImage(segmentation(fileName))*/);
    showNow = -1;
    isOriginal = true;
}

void MainWindow::MainImage_RB_Clicked()
{
  QMenu *RBmenu = new QMenu();
  QAction *remove = new QAction("Сохранить", this);
  RBmenu->addAction(remove);
  connect(remove, &QAction::triggered, [&] {
      QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save File"), "untitled.jpg",
                                                          tr("Images (*.png *.bmp *.jpg)"));
      switch (showNow) {
      case segmentImg:
          segmentation->save(saveFileName);
          break;
      case areaImg:
          area->save(saveFileName);
          break;
      case pointImg:
          point->save(saveFileName);
          break;
      case darkAreaImg:
          darkArea->save(saveFileName);
          break;
      case borderImg:
          border->save(saveFileName);
          break;
      case binarizationImg:
          binarization->save(saveFileName);
          break;
      case meanShiftImg:
          meanShift->save(saveFileName);
          break;
      default:
          break; } });
  RBmenu->exec(QCursor::pos());
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    ui->minimapShow->areaChanged();
}

void MainWindow::on_pushButton_4_clicked()
{
    if (fileName.isEmpty())
        return;
    if (segmentation == nullptr)
        segmentation = new QPixmap(QPixmap().fromImage(getNevusSegment(fileName, true)));
    ui->mainImageShow->items().removeLast();
    ui->mainImageShow->addPixmap(*segmentation);
    showNow = segmentImg;
    isOriginal = false;
}

void MainWindow::on_pushButton_2_clicked()
{
    if (fileName.isEmpty())
        return;
    if (point == nullptr)
    {
/*        if (ui->gaussFilter->isChecked())
            point = new QPixmap(QPixmap().fromImage(npointSelection(fileName, gauss, ui->comboBox_2->currentText().toInt(),
                                                                    ui->coreGauss_x->value(), ui->coreGauss_y->value(),
                                                                    ui->spaceCenterPoint->value(), ui->treshold->value(),
                                                                    ui->tresholdStordge->value(), ui->minRadius->value(),
                                                                    ui->maxRadius->value())));
        if (ui->medianFilter->isChecked())
            point = new QPixmap(QPixmap().fromImage(npointSelection(fileName, median, ui->comboBox_2->currentText().toInt(),
                                                                 ui->coreGauss_x->value(), ui->coreGauss_y->value(),
                                                                 ui->spaceCenterPoint->value(), ui->treshold->value(),
                                                                 ui->tresholdStordge->value(), ui->minRadius->value(),
                                                                 ui->maxRadius->value())));
*/        if (ui->boxFilter->isChecked())
            point = new QPixmap(QPixmap().fromImage(npointSelection(fileName, box, ui->comboBox_2->currentText().toInt(),
                                                                 2, 2,
                                                                 ui->spaceCenterPoint_1->value(), ui->treshold_1->value(),
                                                                 ui->tresholdStordge_1->value(), ui->MinRad->value(),
                                                                 ui->MaxRad->value())));
    }
    ui->mainImageShow->items().removeLast();
    ui->mainImageShow->addPixmap(*point);
    showNow = pointImg;
    isOriginal = false;
}

void MainWindow::on_pushButton_3_clicked()
{
    if (fileName.isEmpty())
        return;
    if (point == nullptr)
    {
        if (ui->boxFilter->isChecked() && ui->checkBox->isChecked())
            point = new QPixmap(QPixmap().fromImage(pointSelection(fileName, box, ui->comboBox_2->currentText().toInt(),
                                                                 2, 2,
                                                                 ui->spaceCenterPoint->value(), ui->treshold->value(),
                                                                 ui->tresholdStordge->value(), ui->minRadius->value(),
                                                                 ui->maxRadius->value(), 1,
                                                                 ui->minTh->value(), ui->maxTh->value())));
          if (ui->boxFilter->isChecked() && ui->checkBox_2->isChecked())
             point = new QPixmap(QPixmap().fromImage(pointSelection(fileName, box, ui->comboBox_2->currentText().toInt(),
                                                                  2, 2,
                                                                  ui->spaceCenterPoint->value(), ui->treshold->value(),
                                                                  ui->tresholdStordge->value(), ui->minRadius->value(),
                                                                  ui->maxRadius->value(), 2,
                                                                    ui->minTh->value(), ui->maxTh->value())));
          if (ui->boxFilter->isChecked() && ui->checkBox_3->isChecked())
             point = new QPixmap(QPixmap().fromImage(pointSelection(fileName, box, ui->comboBox_2->currentText().toInt(),
                                                                  2, 2,
                                                                  ui->spaceCenterPoint->value(), ui->treshold->value(),
                                                                  ui->tresholdStordge->value(), ui->minRadius->value(),
                                                                  ui->maxRadius->value(), 3,
                                                                    ui->minTh->value(), ui->maxTh->value())));
          if (ui->boxFilter->isChecked() && ui->checkBox_4->isChecked())
             point = new QPixmap(QPixmap().fromImage(pointSelection(fileName, box, ui->comboBox_2->currentText().toInt(),
                                                                  2, 2,
                                                                  ui->spaceCenterPoint->value(), ui->treshold->value(),
                                                                  ui->tresholdStordge->value(), ui->minRadius->value(),
                                                                  ui->maxRadius->value(), 4,
                                                                    ui->minTh->value(), ui->maxTh->value())));
          if (ui->boxFilter->isChecked() && ui->checkBox_5->isChecked())
             point = new QPixmap(QPixmap().fromImage(pointSelection(fileName, box, ui->comboBox_2->currentText().toInt(),
                                                                  2, 2,
                                                                  ui->spaceCenterPoint->value(), ui->treshold->value(),
                                                                  ui->tresholdStordge->value(), ui->minRadius->value(),
                                                                  ui->maxRadius->value(), 5,
                                                                  ui->minTh->value(), ui->maxTh->value())));
          if (ui->boxFilter->isChecked() && ui->checkBox_6->isChecked())
             point = new QPixmap(QPixmap().fromImage(pointSelection(fileName, box, ui->comboBox_2->currentText().toInt(),
                                                                  2, 2,
                                                                  ui->spaceCenterPoint->value(), ui->treshold->value(),
                                                                  ui->tresholdStordge->value(), ui->minRadius->value(),
                                                                  ui->maxRadius->value(), 6,
                                                                  ui->minTh->value(), ui->maxTh->value())));
    }
    ui->mainImageShow->items().removeLast();
    ui->mainImageShow->addPixmap(*point);
    showNow = pointImg;
    isOriginal = false;
}

void MainWindow::on_pushButton_5_clicked()
{
    if (fileName.isEmpty())
        return;
    if (!isOriginal) {
        ui->mainImageShow->items().removeLast();
        ui->mainImageShow->addPixmap(*original);
        isOriginal = true;
    }
    else {
        ui->mainImageShow->items().removeLast();
        switch (showNow) {
        case segmentImg:
            ui->mainImageShow->addPixmap(*segmentation);
            break;
        case areaImg:
            ui->mainImageShow->addPixmap(*area);
            break;
        case pointImg:
            ui->mainImageShow->addPixmap(*point);
            break;
        case darkAreaImg:
            ui->mainImageShow->addPixmap(*darkArea);
            break;
        case borderImg:
            ui->mainImageShow->addPixmap(*border);
            break;
        case watershedImg:
            ui->mainImageShow->addPixmap(*watershed);
            break;
        case sobelImg:
            ui->mainImageShow->addPixmap(*sobel);
            break;
        case kmeansImg:
            ui->mainImageShow->addPixmap(*kmeans);
            break;
        case laplaceImg:
            ui->mainImageShow->addPixmap(*laplace);
            break;
        case binarizationImg:
            ui->mainImageShow->addPixmap(*binarization);
            break;
        case meanShiftImg:
            ui->mainImageShow->addPixmap(*meanShift);
            break;
        default:
            break;
        }
        isOriginal = false;
    }
}

void MainWindow::on_pushButton_8_clicked()
{
    if (fileName.isEmpty())
        return;
    delete border;
    border = new QPixmap(QPixmap().fromImage(getImgBorders(fileName)));

    ui->mainImageShow->items().removeLast();
    ui->mainImageShow->addPixmap(*border);
    showNow = borderImg;
    isOriginal = false;
}


void MainWindow::on_pushButton_17_clicked()
{

}

void MainWindow::on_pushButton_18_clicked()
{

}



void MainWindow::on_open_im_clicked()
{
    scSrc.clear();
    QString fp = QFileDialog::getOpenFileName() ;

    nPixSrc = new QGraphicsPixmapItem( QPixmap(fp) ) ;
    nPixDst = new QGraphicsPixmapItem( QPixmap(fp) ) ;

    scSrc.addItem( nPixSrc ) ;
    scDst.addItem( nPixDst ) ;

    mScene1 = new PaintScene(this);
    ui->mainImageShow->setScene(&scSrc);
    ui->dstGraphicsView->setScene(mScene1);
    connect(mScene1, &PaintScene::mouseReleased, this, &MainWindow::obrezka);
    mScene1->addItem(nPixDst);
}

void MainWindow::obrezka()
{
    QRect rect(mScene1->mx,mScene1->my, mScene1->mwidth, mScene1->mheigh);
    QPixmap cropped = nPixSrc->pixmap().copy(rect);

    nPixSrc->setPixmap(cropped);
    nPixDst->setPixmap(cropped);
}

Mat delete_single_pixel (Mat image) {
    for (int x = 1; x < image.cols-1; x++)
        for (int y=1; y < image.rows-1; y++) {
            if (image.at<Vec3b>(y,x)[0]==255){
                int tmp = 0;
                if (image.at<Vec3b>(y-1,x-1)[0]==0)
                    tmp++;
                if (image.at<Vec3b>(y-1,x)[0]==0)
                    tmp++;
                if (image.at<Vec3b>(y-1,x+1)[0]==0)
                    tmp++;
                if (image.at<Vec3b>(y,x+1)[0]==0)
                    tmp++;
                if (image.at<Vec3b>(y+1,x+1)[0]==0)
                    tmp++;
                if (image.at<Vec3b>(y+1,x)[0]==0)
                    tmp++;
                if (image.at<Vec3b>(y+1,x-1)[0]==0)
                    tmp++;
                if (image.at<Vec3b>(y,x-1)[0]==0)
                    tmp++;

                if (tmp == 8){
                    image.at<Vec3b>(y,x) = Vec3b(0,0,0);
                }
            }
        }
    return image;
}

static void thinningIteration(Mat img, int iter, int thinningType){
    Mat marker = Mat::zeros(img.size(), CV_8UC1);

    if(thinningType == cv::ximgproc::THINNING_ZHANGSUEN){
        for (int i = 1; i < img.rows-1; i++)
        {
            for (int j = 1; j < img.cols-1; j++)
            {
                uchar p2 = img.at<uchar>(i-1, j);
                uchar p3 = img.at<uchar>(i-1, j+1);
                uchar p4 = img.at<uchar>(i, j+1);
                uchar p5 = img.at<uchar>(i+1, j+1);
                uchar p6 = img.at<uchar>(i+1, j);
                uchar p7 = img.at<uchar>(i+1, j-1);
                uchar p8 = img.at<uchar>(i, j-1);
                uchar p9 = img.at<uchar>(i-1, j-1);

                int A  = (p2 == 0 && p3 == 1) + (p3 == 0 && p4 == 1) +
                         (p4 == 0 && p5 == 1) + (p5 == 0 && p6 == 1) +
                         (p6 == 0 && p7 == 1) + (p7 == 0 && p8 == 1) +
                         (p8 == 0 && p9 == 1) + (p9 == 0 && p2 == 1);
                int B  = p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9;
                int m1 = iter == 0 ? (p2 * p4 * p6) : (p2 * p4 * p8);
                int m2 = iter == 0 ? (p4 * p6 * p8) : (p2 * p6 * p8);

                if (A == 1 && (B >= 2 && B <= 6) && m1 == 0 && m2 == 0)
                    marker.at<uchar>(i,j) = 1;
            }
        }
    }
    if(thinningType == cv::ximgproc::THINNING_GUOHALL){
        for (int i = 1; i < img.rows-1; i++)
        {
            for (int j = 1; j < img.cols-1; j++)
            {
                uchar p2 = img.at<uchar>(i-1, j);
                uchar p3 = img.at<uchar>(i-1, j+1);
                uchar p4 = img.at<uchar>(i, j+1);
                uchar p5 = img.at<uchar>(i+1, j+1);
                uchar p6 = img.at<uchar>(i+1, j);
                uchar p7 = img.at<uchar>(i+1, j-1);
                uchar p8 = img.at<uchar>(i, j-1);
                uchar p9 = img.at<uchar>(i-1, j-1);

                int C  = ((!p2) & (p3 | p4)) + ((!p4) & (p5 | p6)) +
                         ((!p6) & (p7 | p8)) + ((!p8) & (p9 | p2));
                int N1 = (p9 | p2) + (p3 | p4) + (p5 | p6) + (p7 | p8);
                int N2 = (p2 | p3) + (p4 | p5) + (p6 | p7) + (p8 | p9);
                int N  = N1 < N2 ? N1 : N2;
                int m  = iter == 0 ? ((p6 | p7 | (!p9)) & p8) : ((p2 | p3 | (!p5)) & p4);

                if ((C == 1) && ((N >= 2) && ((N <= 3)) & (m == 0)))
                    marker.at<uchar>(i,j) = 1;
            }
        }
    }

    img &= ~marker;
}

void thinning(InputArray input, OutputArray output, int thinningType){
    Mat processed = input.getMat().clone();
    // Enforce the range of the input image to be in between 0 - 255
    processed /= 255;

    Mat prev = Mat::zeros(processed.size(), CV_8UC1);
    Mat diff;

    do {
        thinningIteration(processed, 0, thinningType);
        thinningIteration(processed, 1, thinningType);
        absdiff(processed, prev, diff);
        processed.copyTo(prev);
    }
    while (countNonZero(diff) > 0);

    processed *= 255;

    output.assign(processed);
}


Mat modify_suen (Mat image) {
    for (int x = 1; x < image.cols-1; x++)
        for (int y=1; y < image.rows-1; y++) {
            if (image.at<Vec3b>(y,x)[0]==255){
                if ((image.at<Vec3b>(y-1,x)[0]==255) && (image.at<Vec3b>(y,x+1)[0]==255) && (image.at<Vec3b>(y+1,x-1)[0]==0))
                    image.at<Vec3b>(y,x) = Vec3b(0,0,0);
                if ((image.at<Vec3b>(y-1,x)[0]==255) && (image.at<Vec3b>(y,x-1)[0]==255) && (image.at<Vec3b>(y+1,x+1)[0]==0))
                    image.at<Vec3b>(y,x) = Vec3b(0,0,0);
                if ((image.at<Vec3b>(y,x-1)[0]==255) && (image.at<Vec3b>(y+1,x)[0]==255) && (image.at<Vec3b>(y-1,x+1)[0]==0))
                    image.at<Vec3b>(y,x) = Vec3b(0,0,0);
                if ((image.at<Vec3b>(y,x+1)[0]==255) && (image.at<Vec3b>(y+1,x)[0]==255) && (image.at<Vec3b>(y-1,x-1)[0]==0))
                    image.at<Vec3b>(y,x) = Vec3b(0,0,0);
            }
        }
    return image;
}

bool finder (int first, int second, vector<Point> v){
    bool flag;
    Point p;
    p.y = first;
    p.x = second;
    for (unsigned i = 0; i < v.size(); i++){
        if (p == v[i]){
            flag = true;
            break;
        }
        else
            flag = false;
    }
    return flag;
}

Mat MainWindow::otrez( Mat image){
    vector <Point> kon;
    vector <Point> vet;

    for (int x = 1; x < image.cols-1; x++)
        for (int y=1; y < image.rows-1; y++)
        {
            if (image.at<Vec3b>(y,x)[0]==255){
                int tmp = 0;
                if (image.at<Vec3b>(y-1,x-1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y-1,x)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y-1,x+1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y,x+1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y+1,x+1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y+1,x)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y+1,x-1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y,x-1)[0]==255)
                    tmp++;

                if (tmp == 1){
                    kon.push_back(Point(x,y));
                }
                if (tmp > 2){
                    vet.push_back(Point(x,y));
                }
            }
        }

    int dlina = ui->min_otv->text().toInt();

    for (int vnesh = 0; vnesh < dlina; vnesh++){
        for (unsigned i = 0; i < vet.size()-1; i++){
            for (unsigned j = 0; j < kon.size()-1; j++){

                if (norm(vet[i]-kon[j]) < dlina)
                {
                    image.at<Vec3b>(kon[j]) = Vec3b(0,0,0);

                    if ((image.at<Vec3b>(kon[j].y-1,kon[j].x-1)[0]==255) && (!finder(kon[j].y-1, kon[j].x-1,vet))){
                        kon[j].y = kon[j].y-1;
                        kon[j].x = kon[j].x-1;
                        break;
                    }

                    if ((image.at<Vec3b>(kon[j].y-1,kon[j].x)[0]==255) && (!finder(kon[j].y-1, kon[j].x,vet))){
                        kon[j].y = kon[j].y-1;
                        kon[j].x = kon[j].x;
                        break;
                    }

                    if ((image.at<Vec3b>(kon[j].y-1,kon[j].x+1)[0]==255) && (!finder(kon[j].y-1, kon[j].x+1,vet))){
                        kon[j].y = kon[j].y-1;
                        kon[j].x = kon[j].x+1;
                        break;
                    }

                    if ((image.at<Vec3b>(kon[j].y,kon[j].x+1)[0]==255) && (!finder(kon[j].y, kon[j].x+1,vet))){
                        kon[j].y = kon[j].y;
                        kon[j].x = kon[j].x+1;
                        break;
                    }

                    if ((image.at<Vec3b>(kon[j].y+1,kon[j].x+1)[0]==255) && (!finder(kon[j].y+1, kon[j].x+1,vet))){
                        kon[j].y = kon[j].y+1;
                        kon[j].x = kon[j].x+1;
                        break;
                    }

                    if ((image.at<Vec3b>(kon[j].y+1,kon[j].x)[0]==255) && (!finder(kon[j].y+1, kon[j].x,vet))){
                        kon[j].y = kon[j].y+1;
                        kon[j].x = kon[j].x;
                        break;
                    }

                    if ((image.at<Vec3b>(kon[j].y+1,kon[j].x-1)[0]==255) && (!finder(kon[j].y+1, kon[j].x-1,vet))){
                        kon[j].y = kon[j].y+1;
                        kon[j].x = kon[j].x-1;
                        break;
                    }

                    if ((image.at<Vec3b>(kon[j].y,kon[j].x-1)[0]==255) && (!finder(kon[j].y, kon[j].x-1,vet))){
                        kon[j].y = kon[j].y;
                        kon[j].x = kon[j].x-1;
                        break;
                    }
                }
            }
        }
    }
    kon.clear();
    vet.clear();
    return image;
}

void MainWindow::on_jangsuen_clicked()
{
    QImage temp = nPixSrc->pixmap().toImage();

       Mat image = QImage2Mat(temp);
       cvtColor(image, image, COLOR_BGRA2BGR);

       Mat tmp = image.clone();

       cvtColor(tmp, tmp, COLOR_BGR2GRAY);

       int porog = ui->porog->text().toInt();
       int blocksize = ui->blocksize->text().toInt();
       int smesh = ui->C->text().toInt();
       Mat edin = tmp.clone();
       Mat adapt = tmp.clone();
       cv::threshold(edin, edin, porog, 255, 1);
       cv::adaptiveThreshold(adapt, adapt, 255, ADAPTIVE_THRESH_GAUSSIAN_C, CV_THRESH_BINARY_INV, blocksize, smesh);
       bitwise_and(edin,adapt,tmp);

       Mat se1 = getStructuringElement(MORPH_RECT, Size(5, 5));
       Mat se2 = getStructuringElement(MORPH_RECT, Size(3, 3));

       Mat clos;
       morphologyEx(tmp, clos, MORPH_CLOSE, se1);

       Mat open;
       morphologyEx(clos, open, MORPH_OPEN, se2);

       thinning(open, open, 1);

       cvtColor(open, open, COLOR_GRAY2BGR);
       Mat obrez;
       obrez = otrez(open);

       Mat mod;
       mod = modify_suen(obrez);

       Mat del;
       del = delete_single_pixel(mod);

       Mat minl;
       minl = mindlin(del);

       Mat res;
       bitwise_or(image, minl, res);
       nPixSrc->setPixmap( QPixmap::fromImage( Mat2QImage( res )) );
}





Mat MainWindow::mindlin( Mat image){
    vector <Point> kon;

    for (int x = 1; x < image.cols-1; x++)
        for (int y=1; y < image.rows-1; y++)
        {
            if (image.at<Vec3b>(y,x)[0]==255){
                int tmp = 0;
                if (image.at<Vec3b>(y-1,x-1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y-1,x)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y-1,x+1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y,x+1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y+1,x+1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y+1,x)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y+1,x-1)[0]==255)
                    tmp++;
                if (image.at<Vec3b>(y,x-1)[0]==255)
                    tmp++;

                if (tmp == 1){
                    kon.push_back(Point(x,y));
                }
            }
        }

    int dlina = ui->min_lin->text().toInt();

    for (int vnesh = 0; vnesh < dlina; vnesh++){
        for (unsigned i = 0; i < kon.size(); i++){
            for (unsigned j = 0; j < kon.size(); j++){

                if (norm(kon[i]-kon[j]) < dlina)
                {
                    image.at<Vec3b>(kon[i]) = Vec3b(0,0,0);
                    image.at<Vec3b>(kon[j]) = Vec3b(0,0,0);

                    if (image.at<Vec3b>(kon[i].y-1,kon[i].x-1)[0]==255){
                        kon[i].y = kon[i].y-1;
                        kon[i].x = kon[i].x-1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[i].y-1,kon[i].x)[0]==255){
                        kon[i].y = kon[i].y-1;
                        kon[i].x = kon[i].x;
                        break;
                    }

                    if (image.at<Vec3b>(kon[i].y-1,kon[i].x+1)[0]==255){
                        kon[i].y = kon[i].y-1;
                        kon[i].x = kon[i].x+1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[i].y,kon[i].x+1)[0]==255){
                        kon[i].y = kon[i].y;
                        kon[i].x = kon[i].x+1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[i].y+1,kon[i].x+1)[0]==255){
                        kon[i].y = kon[i].y+1;
                        kon[i].x = kon[i].x+1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[i].y+1,kon[i].x)[0]==255){
                        kon[i].y = kon[i].y+1;
                        kon[i].x = kon[i].x;
                        break;
                    }

                    if (image.at<Vec3b>(kon[i].y+1,kon[i].x-1)[0]==255){
                        kon[i].y = kon[i].y+1;
                        kon[i].x = kon[i].x-1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[i].y,kon[i].x-1)[0]==255){
                        kon[i].y = kon[i].y;
                        kon[i].x = kon[i].x-1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[j].y-1,kon[j].x-1)[0]==255){
                        kon[j].y = kon[j].y-1;
                        kon[j].x = kon[j].x-1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[j].y-1,kon[j].x)[0]==255){
                        kon[j].y = kon[j].y-1;
                        kon[j].x = kon[j].x;
                        break;
                    }

                    if (image.at<Vec3b>(kon[j].y-1,kon[j].x+1)[0]==255){
                        kon[j].y = kon[j].y-1;
                        kon[j].x = kon[j].x+1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[j].y,kon[j].x+1)[0]==255){
                        kon[j].y = kon[j].y;
                        kon[j].x = kon[j].x+1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[j].y+1,kon[j].x+1)[0]==255) {
                        kon[j].y = kon[j].y+1;
                        kon[j].x = kon[j].x+1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[j].y+1,kon[j].x)[0]==255){
                        kon[j].y = kon[j].y+1;
                        kon[j].x = kon[j].x;
                        break;
                    }

                    if (image.at<Vec3b>(kon[j].y+1,kon[j].x-1)[0]==255){
                        kon[j].y = kon[j].y+1;
                        kon[j].x = kon[j].x-1;
                        break;
                    }

                    if (image.at<Vec3b>(kon[j].y,kon[j].x-1)[0]==255){
                        kon[j].y = kon[j].y;
                        kon[j].x = kon[j].x-1;
                        break;
                    }
                }
            }
        }
    }
    kon.clear();
    return image;
}

QImage MainWindow::Mat2QImage(cv::Mat const& src)
{
    switch(src.type())
    {
    case CV_8UC4:
    {
        cv::Mat view(src);
        QImage view2(view.data, view.cols, view.rows, view.step[0], QImage::Format_ARGB32);
        return view2.copy();
        break;
    }
    case CV_8UC3:
    {
        cv::Mat mat;
        cvtColor(src, mat, cv::COLOR_BGR2BGRA); //COLOR_BGR2RGB doesn't behave so use RGBA
        QImage view(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
        return view.copy();
        break;
    }
    case CV_8UC1:
    {
        cv::Mat mat;
        cvtColor(src, mat, cv::COLOR_GRAY2BGRA);
        QImage view(mat.data, mat.cols, mat.rows, mat.step[0], QImage::Format_ARGB32);
        return view.copy();
        break;
    }
    default:
    {
        //throw invalid_argument("Image format not supported");
        return QImage();
        break;
    }
    }
}

cv::Mat MainWindow::QImage2Mat(QImage const& src)
{
    switch(src.format()) {
    case QImage::Format_Invalid:
    {
        cv::Mat empty ;
        return empty  ;
        break;
    }
    case QImage::Format_RGB32:
    {
        cv::Mat view(src.height(),src.width(),CV_8UC4,(void *)src.constBits(),src.bytesPerLine()) ;
        return view ;
        break;
    }
    case QImage::Format_RGB888:
    {
        cv::Mat out ;
        cv::Mat view(src.height(),src.width(),CV_8UC3,(void *)src.constBits(),src.bytesPerLine());
        cv::cvtColor(view, out, cv::COLOR_RGB2BGR);
        return out ;
        break;
    }
    default:
    {
        QImage conv = src.convertToFormat(QImage::Format_ARGB32);
        cv::Mat view(conv.height(),conv.width(),CV_8UC4,(void *)conv.constBits(),conv.bytesPerLine());
        return view ;
        break;
    }
    }
}

void MainWindow::on_pushButtonPlus_clicked()
{
    //ui->srcGraphicsView->scale(1.1, 1.1);
    //ui->dstGraphicsView->scale(1.1, 1.1);
}

void MainWindow::on_pushButtonMinus_clicked()
{
    //ui->srcGraphicsView->scale(1/1.1, 1/1.1);
    //ui->dstGraphicsView->scale(1/1.1, 1/1.1);
}


void MainWindow::on_porog_sl_valueChanged(int value)
{
    QString qval = QString::number(value);
    ui->porog->setText(qval);
}

void MainWindow::on_blocksize_sl_valueChanged(int value)
{
    QString qval = QString::number(value);
    ui->blocksize->setText(qval);
}

void MainWindow::on_C_sl_valueChanged(int value)
{
    QString qval = QString::number(value);
    ui->C->setText(qval);
}

void MainWindow::on_porog_textChanged(const QString &arg1)
{
    int porog = ui->porog->text().toInt();
    if (porog < 0){
        porog = 0;
    }
    if (porog > 255){
        porog = 255;
    }
    on_porog_sl_valueChanged(porog);
    ui->porog_sl->setSliderPosition(porog);
}

void MainWindow::on_blocksize_textEdited(const QString &arg1)
{
    int blocksize = ui->blocksize->text().toInt();
    if (blocksize % 2 == 0){
        blocksize++;
    }
    if (blocksize < 3){
        blocksize = 3;
    }
    if (blocksize > 251){
        blocksize = 251;
    }
    on_blocksize_sl_valueChanged(blocksize);
    ui->blocksize_sl->setSliderPosition(blocksize);
}

void MainWindow::on_C_textChanged(const QString &arg1)
{
    int C = ui->C->text().toInt();
    if (C % 2 != 0){
        C++;
    }
    if (C < 2){
        C = 2;
    }
    if (C > 50){
        C = 50;
    }
    on_C_sl_valueChanged(C);
    ui->C_sl->setSliderPosition(C);
}


void MainWindow::on_bl_gl_valueChanged(int value)
{
    QString qval = QString::number(value);
    ui->bl_L_gl->setText(qval);
    N1 = value;
}

void MainWindow::on_C_gl_valueChanged(int value)
{
    QString qval = QString::number(value);
    ui->C_L_gl->setText(qval);
    porog_percents = value;
}


void MainWindow::on_bl_L_gl_textEdited(const QString &arg1)
{
    int blocksize = ui->bl_L_gl->text().toInt();
    if (blocksize % 2 == 0){
        blocksize++;
    }
    if (blocksize < 3){
        blocksize = 3;
    }
    if (blocksize > 251){
        blocksize = 251;
    }
    on_bl_gl_valueChanged(blocksize);
    ui->bl_gl->setSliderPosition(blocksize);
    N1 = blocksize;
}

void MainWindow::on_C_L_gl_textChanged(const QString &arg1)
{
    int C = ui->C_L_gl->text().toInt();
    if (C % 2 != 0){
        C++;
    }
    if (C < 2){
        C = 2;
    }
    if (C > 50){
        C = 50;
    }
    on_C_gl_valueChanged(C);
    ui->C_gl->setSliderPosition(C);
    porog_percents = C;
}

void MainWindow::on_pushButton_Glob_clicked()
{
    QImage temp = After_Median_Filter ;
    QImage gray = Gray_for_binarize ;
    QImage tempOut = After_Median_Filter ;
    int r, g, b ;
    int Aver = 0;       //средняя яркость в данном окне
    int Black_pixels ;  //количество черных пикселей, которые учитывать не нужно
    int averPix ;   //cредняя яркость текущего пиксела

    if(binarized == 0)
    {
        for( int y = 0 ; y < h ; ++y )
        {
            for( int x = 0 ; x < w ; ++x )
            {
                r = qRed( temp.pixel(x,y) ) ;
                g = qGreen( temp.pixel(x,y) ) ;
                b = qBlue( temp.pixel(x,y) ) ;
                gray.setPixel(x, y, qRgb( ((r + g + b)/3) , 0, 0 ) );
            }
        }
        binarized = 1 ;
        Gray_for_binarize = gray ;
    }

    if(binarized == 1)
    {
        for ( int x = N1/2 ; x < ( w - N1/2 ) ; ++x)
        {
            for( int y = N1/2 ; y < ( h - N1/2 ) ; ++y )
            {
                averPix = qRed( gray.pixel(x,y) ) ;
                if (averPix != 0 )
                {
                    Aver = 0;
                    Black_pixels = 0 ;

                    for ( int i = ( x - N1/2 ) ; i < ( x + N1/2 ) ; ++i )
                    {
                        for( int j = ( y - N1/2 ) ; j < ( y + N1/2 ) ; ++j )
                        {
                            int averPix = qRed( gray.pixel(i,j) ) ;

                            if (averPix == 0)
                            {
                                ++Black_pixels ;
                                continue;
                            }
                            Aver = Aver + averPix ;
                        }
                    }

                    Aver = (Aver / (N1*N1 - Black_pixels ) ) ;

                    if( averPix < ( ((100 - porog_percents )/100) *Aver ))
                        tempOut.setPixel( x, y, qRgb( 255, 255, 255) );
                    else
                        tempOut.setPixel( x, y, qRgb( 0, 0, 0) );
                }
            }
        }

        //Закрашивание "рамки" изображения черным
        for ( int x = 0 ; x < N1/2 ; ++x)   //первая вертикальная сторона
        {
            for( int y = 0 ; y < h ; ++y )
            {
                tempOut.setPixel( x, y, qRgb( 0, 0, 0) );
            }
        }

        for ( int x = (w-1-N1/2) ; x < w ; ++x)   //вторая вертикальная сторона
        {
            for( int y = 0 ; y < h ; ++y )
            {
                tempOut.setPixel( x, y, qRgb( 0, 0, 0) );
            }
        }

        for ( int x = N1/2 ; x < (w-1-N1/2) ; ++x)   //первая горизонтальная сторона
        {
            for( int y = 0 ; y < N1/2 ; ++y )
            {
                tempOut.setPixel( x, y, qRgb( 0, 0, 0) );
            }
        }

        for ( int x = N1/2 ; x < (w-1-N1/2) ; ++x)   //вторая горизонтальная сторона
        {
            for( int y = (h-1-N1/2) ; y < h ; ++y )
            {
                tempOut.setPixel( x, y, qRgb( 0, 0, 0) );
            }
        }
    }

    nPixDst->setPixmap(QPixmap::fromImage(tempOut)) ;
    After_Binar = tempOut ;
    contoured = 0 ;

    if(contoured == 0 && binarized == 1)
    {
        QImage temp3 = Source ;
        KomkiWalk = White_Pic ;
        FiltWalk = White_Pic ;
        SaveBandIm = White_Pic ;
        QImage temp5 = nPixDst->pixmap().toImage() ;
        QImage temp6 = temp5 ;


        for( int y = 0 ;  y < h ;  ++ y )   //перенос бинарного изображения на отдельное изображение для фильтрации по площади
        {
            for (int x = 0 ; x <  w ; ++ x )
            {
                if (qRed(temp5.pixel(x,y)) == 255)
                {
                    FiltWalk.setPixel(x, y, qRgb( 0, 0, 0) );
                }
                KomkiWalk.setPixel(x, y, qRgb( 200, 200, 200) );
            }
        }
    //qDebug() << "Point 1" ;
        bool check = 1; //индикатор возможности эрозии для данного пикселя
        unsigned int Mask[5][5] =
        { {0,0,1,0,0},
          {0,1,1,1,0},
          {1,1,1,1,1},
          {0,1,1,1,0},
          {0,0,1,0,0}
        } ;

        for( int y = 2 ;  y < h - 2 ;  ++y )   //эрозия
        {
            for (int x = 2 ; x <  w - 2 ; ++x )
            {
                check = 1;

                for ( int i = 0 ; i < 5 ; ++i)
                {
                    for ( int j = 0 ; j < 5 ; ++j)
                    {
                        if (Mask[i][j] == 1 && qRed(temp5.pixel(x+i-2,y+j -2)) == 0)
                        {
                            check = 0;
                            break;
                        }
                    }
                    if (check == 0)
                        break;
                }

                if (check == 1)
                {
                    temp6.setPixel(x, y, qRgb( 0, 0, 0) );
                }
            }
        }
    //qDebug() << "Point 2" ;
        for( int y = 1 ;  y < h;  ++ y )
        {
            for (int x = 1 ; x < w; ++ x )
            {
                if (qRed(temp6.pixel(x,y)) != 0)
                {
                    temp3.setPixel(x, y, qRgb( cont_red, cont_green, cont_blue ) );
                    SaveBandIm.setPixel(x, y, qRgb( cont_red, cont_green, cont_blue ) );
                    KomkiWalk.setPixel(x, y, qRgb( cont_red, cont_green, cont_blue ) );
                }
            }
        }
    //qDebug() << "Point 3" ;
        nPixSrc->setPixmap( QPixmap::fromImage( temp3 ) ) ;
        mScene1 = new PaintScene(this);
        mScene1->addItem(nPixSrc);
        ui->mainImageShow->setScene(mScene1);
        Pre_Contoured = Contoured ;
        Contoured = temp3 ;
        Filtered = KomkiWalk ;
        contoured = 1 ;
    }
    else if(contoured == 1)
        nPixSrc->setPixmap( QPixmap::fromImage(Contoured)) ;

    mScene1 = new PaintScene(this);
    mScene1->addItem(nPixSrc);
    ui->mainImageShow->setScene(mScene1);

}





void MainWindow::on_Toch_clicked()
{
    fileName = QFileDialog::getOpenFileName(this, "Select one or more files to open",
                                                    "/home", "Images (*.png *.xpm *.jpg *.bmp)");
    if (fileName.isEmpty())
        return;
    if (original != nullptr) {
        delete original;
        original = nullptr;
    }
    if (point != nullptr) {
        delete point;
        point = nullptr;
    }
    original = new QPixmap(fileName);
    ui->mainImageShow->addPixmap(*original/*QPixmap().fromImage(segmentation(fileName))*/);
    showNow = -1;
    isOriginal = true;
}
