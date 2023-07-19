#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->ScrView->setScene(&scSrc);
    ui->ScrView->setScene(&scDst);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_triggered()
{
 QString fp = QFileDialog::getOpenFileName();

 gpiSrc = new MyGraphicsItem(QPixmap(fp));
 gpiDst = new MyGraphicsItem(QPixmap(fp));

 scSrc.addItem(gpiSrc);
 scDst.addItem(gpiDst);

}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    for(int y=0; y<tmpSrc.height();++y)
        for(int x=0; x<tmpSrc.width();++x)
        {
            if(qGreen(tmpSrc.pixel(x,y))>value)
                tmpDst.setPixel(x,y,qRgb(255,255,255));
                    else
                tmpDst.setPixel(x,y,qRgb(0,0,0));
        }
    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}


void MainWindow::on_pushButton_clicked()
{
    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    int znachbin = ui->lineEdit-> text().toInt();

    int znb=znachbin+1;


    QVector <QRgb> vec;
    int h,w;

      for (h = znb; h < tmpSrc.height()-znb; h++)
      {
          for(w = znb; w < tmpSrc.width()-znb; w++)
          {

             for (int i=-znb;i<znb;i++){
                  for (int j=-znb;j<znb;j++){
                      vec.append(QRgb(tmpSrc.pixel(w+i,h+j)));
                  }
              }
             std::sort(vec.begin(),vec.end());
             tmpDst.setPixel(w,h,vec.at(znachbin*znachbin*4));
             vec.clear();
          }
      }
       gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}

