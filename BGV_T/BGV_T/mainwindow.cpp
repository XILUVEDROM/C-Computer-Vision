#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->srcGrView->verticalScrollBar(), SIGNAL(sliderMoved(int)), ui->dstGrView->verticalScrollBar(),  SLOT(setValue(int)));
    connect(ui->dstGrView->verticalScrollBar(), SIGNAL(sliderMoved(int)), ui->srcGrView->verticalScrollBar(),SLOT(setValue(int)));
    connect(ui->srcGrView->horizontalScrollBar(), SIGNAL(sliderMoved(int)), ui->dstGrView->horizontalScrollBar(), SLOT(setValue(int)));
    connect(ui->dstGrView->horizontalScrollBar(), SIGNAL(sliderMoved(int)), ui->srcGrView->horizontalScrollBar(), SLOT(setValue(int)));

     ui->srcGrView->setScene(&scSrc);
     ui->dstGrView->setScene(&scDst);

}

QString Pert;

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_triggered()
{
    QString fp = QFileDialog::getOpenFileName();
    Pert = fp;

    gpiSrc = new MyGraphicsItem(QPixmap(fp));
    gpiDst = new MyGraphicsItem(QPixmap(fp));


    scSrc.addItem(gpiSrc);
    scDst.addItem(gpiDst);
}

class Izobr {
public:
//    QImage inSrc;
//    QImage outSrc;
//    int porog;
    QImage obrabotka (QImage inSrc, QImage outSrc, int porog)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("in");

    QImage tmpSrc =inSrc;
    QImage tmpDst(tmpSrc);
    QString after;
    QSet <QString> colorh;
    int schn=0;

//    int porog = ui->lineEdit-> text().toInt();

    bool mas[18];
    for(int i=0;i<18;i++)
        mas[i]=false;

      for (int i=0;i<tmpSrc.width();i++){
          for (int j=0;j<tmpSrc.height();j++){
              QRgb color =tmpSrc.pixel( i, j );
              QColor col =QColor(color);

              QString ng = col.name();
              QString nb = QColor(color).name();

              nb.remove(0,1);

              int d = nb.toInt(nullptr,16);

              int red = qRed(tmpDst.pixel(i,j));
              int blue = qBlue(tmpDst.pixel(i,j));
              int green = qGreen(tmpDst.pixel(i,j));

              if (red<=blue+porog && green<=blue+porog )
              {
                  schn++;
                  tmpDst.setPixel(i,j,qRgb(255,0,0));

              }

//              outSrc=tmpDst;
//              return tmpDst;
          //    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

              }
        }
      msgBox.setText("ret2");
      msgBox.exec();

return tmpDst;
};
};

void MainWindow::on_pushButton_clicked()
{
 Izobr ddd;
 QImage inSrc=gpiDst->pixmap().toImage();
 QImage outSrc;
 int porog = ui->lineEdit-> text().toInt();
 outSrc=ddd.obrabotka(inSrc, outSrc, porog);
 gpiDst->setPixmap(QPixmap::fromImage(outSrc));

/*
    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    QString after;
    QSet <QString> colorh;
    int schn=0;

    int porog = ui->lineEdit-> text().toInt();

    bool mas[18];
    for(int i=0;i<18;i++)
        mas[i]=false;

      for (int i=0;i<tmpSrc.width();i++){
          for (int j=0;j<tmpSrc.height();j++){
              QRgb color =tmpSrc.pixel( i, j );
              QColor col =QColor(color);

              QString ng = col.name();
              QString nb = QColor(color).name();

              nb.remove(0,1);

              int d = nb.toInt(nullptr,16);

              int red = qRed(tmpDst.pixel(i,j));
              int blue = qBlue(tmpDst.pixel(i,j));
              int green = qGreen(tmpDst.pixel(i,j));

              if (red<=blue+porog && green<=blue+porog )
              {
                  schn++;
                  tmpDst.setPixel(i,j,qRgb(255,0,0));
              }

              gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

              }
        }
*/
}

void MainWindow::on_verticalScrollBar_valueChanged(int value)
{
    QMatrix matrix;
    qreal m = qreal(value)/5;
    matrix.scale(m, m);
    ui->srcGrView->setMatrix(matrix);
    ui->dstGrView->setMatrix(matrix);
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

void MainWindow::on_pushButton_2_clicked()
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

             for (int i=-znachbin;i<znb;i++){
                  for (int j=-znachbin;j<znb;j++){
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

void MainWindow::on_pushButton_3_clicked()
{

    on_pushButton_2_clicked();
    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    int Jxy=0;

      for (int i=1;i<tmpSrc.width()-1;i++){
          for (int j=1;j<tmpSrc.height()-1;j++){

  //Block for checking if surrounding pixels is white

              int red = qRed(tmpSrc.pixel(i,j));
              int blue = qBlue(tmpSrc.pixel(i,j));
              int green = qGreen(tmpSrc.pixel(i,j));


              bool UpPixelIsWhite = false;
              int redU = qRed(tmpSrc.pixel(i,j+1));
              int blueU = qBlue(tmpSrc.pixel(i,j+1));
              int greenU = qGreen(tmpSrc.pixel(i,j+1));
              if(redU==0 && blueU==0 && greenU==0){
                  UpPixelIsWhite=true;

              }

              bool RightPixelIsWhite = false;
              int redR = qRed(tmpSrc.pixel(i+1,j));
              int blueR = qBlue(tmpSrc.pixel(i+1,j));
              int greenR = qGreen(tmpSrc.pixel(i+1,j));
              if(redR==0 && blueR==0 && greenR==0){
                  RightPixelIsWhite=true;

              }

              bool DownPixelIsWhite = false;
              int redD = qRed(tmpSrc.pixel(i,j-1));
              int blueD = qBlue(tmpSrc.pixel(i,j-1));
              int greenD = qGreen(tmpSrc.pixel(i,j-1));
              if(redD==0 && blueD==0 && greenD==0){
                  DownPixelIsWhite=true;

              }

              bool LeftPixelIsWhite = false;
              int redL = qRed(tmpSrc.pixel(i-1,j));
              int blueL = qBlue(tmpSrc.pixel(i-1,j));
              int greenL = qGreen(tmpSrc.pixel(i-1,j));
              if(redL==0 && blueL==0 && greenL==0){
                  LeftPixelIsWhite=true;

              }

              bool UpRightPixelIsWhite = false;
              int redUR = qRed(tmpSrc.pixel(i+1,j+1));
              int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
              int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
              if(redUR==0 && blueUR==0 && greenUR==0){
                  UpRightPixelIsWhite=true;

              }

              bool UpLeftPixelIsWhite = false;
              int redUL = qRed(tmpSrc.pixel(i-1,j+1));
              int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
              int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
              if(redUL==0 && blueUL==0 && greenUL==0){
                  UpLeftPixelIsWhite=true;

              }

              bool DownRightPixelIsWhite = false;
              int redDR = qRed(tmpSrc.pixel(i+1,j-1));
              int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
              int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
              if(redDR==0 && blueDR==0 && greenDR==0){
                  DownRightPixelIsWhite=true;

              }

              bool DownLeftPixelIsWhite = false;
              int redDL = qRed(tmpSrc.pixel(i-1,j-1));
              int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
              int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
              if(redDL==0 && blueDL==0 && greenDL==0){
                  DownLeftPixelIsWhite=true;

              }

              if ((red==255 && blue==255 && green==255) && (UpPixelIsWhite==true || RightPixelIsWhite==true || DownPixelIsWhite==true || LeftPixelIsWhite==true ||
                      UpRightPixelIsWhite==true || UpLeftPixelIsWhite==true || DownLeftPixelIsWhite==true || DownRightPixelIsWhite==true)){
                  tmpDst.setPixel(i,j,qRgb(255,0,0));
                  peregacha(i,j);
              }
          }
      }

  gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}


void MainWindow::peregacha(int fga, int fgas)
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    for (int i=0;i<tmpSrc.width();i++){
        for (int j=0;j<tmpSrc.height();j++){
            if (i==fga && j==fgas)
            {
                tmpDst.setPixel(i,j,qRgb(0,255,255));
            }
        }
    }
    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}




void MainWindow::on_Assimetrya_clicked()
{
/*    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    IplImage * pere;
    pere = QImage2IplImage(&tmpDst);
    CentrMass * CM = new CentrMass();

    cv::Point2f center = CM->Simmetr(*pere);
    int Xc = center.x;
    int Yc = center.y;
    qDebug() << "Xc=" << Xc<< "Yc=" << Yc;
    ui->textBrowser_2->append("Xc="+ QString::number(Xc));
    ui->textBrowser_2->append("Yc=" + QString::number(Yc));
    tmpDst.setPixel(Xc,Yc,qRgb(0,255,0));
    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));

    QString fga = ui->comboBox->currentText();
    ui->textBrowser_2->append(fga);

    int schS=0,schN=0;
    QSize soi=tmpDst.size();

    int height = soi.height();
    int width = soi.width();
    int heightY = Yc;
    int wigthX = Xc;
    int xOtr = Xc;
    int SchYup=0; int KoefX=0;
    float ResY=0;
    int SchPol=0 , SchOtr=0;
    int YPol = Yc;
    int YOtr = Yc;

    if (fga=="Y")
    {
    int red = qRed(tmpSrc.pixel(Xc,Yc));
    int blue = qBlue(tmpSrc.pixel(Xc,Yc));
    int green = qGreen(tmpSrc.pixel(Xc,Yc));

    int redPol = qRed(tmpSrc.pixel(wigthX,heightY));
    int bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
    int greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));

    int redOtr = qRed(tmpSrc.pixel(wigthX,heightY));
    int blueOtr = qBlue(tmpSrc.pixel(wigthX,heightY));
    int greenOtr = qGreen(tmpSrc.pixel(wigthX,heightY));

    while (red!=0 && green!=255 && blue!=255)
    {
    while(redPol!=0 && bluePol!=255 && greenPol!=255)
    {
    SchPol++;
    redPol = qRed(tmpSrc.pixel(wigthX,heightY));
    greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
    bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
    wigthX++;
    }

    redPol = qRed(tmpSrc.pixel(1,1));
    greenPol = qGreen(tmpSrc.pixel(1,1));
    bluePol = qBlue(tmpSrc.pixel(1,1));

    while(redOtr!=0 && blueOtr != 255 && greenOtr != 255)
    {
    SchOtr++;
    redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
    blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
    greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));
    xOtr--;
    }

    redOtr = qRed(tmpSrc.pixel(1,1));
    blueOtr = qBlue(tmpSrc.pixel(1,1));
    greenOtr = qGreen(tmpSrc.pixel(1,1));

    if(SchPol==SchOtr || SchPol== SchOtr+1 || SchPol== SchOtr-1)
    {
    KoefX++;
    for (SchOtr;SchOtr>0;SchOtr--)
    {
    tmpDst.setPixel(xOtr+SchOtr,heightY,qRgb(0,255,0));
    }

    for (SchPol;SchPol>0;SchPol--)
    {
    tmpDst.setPixel(wigthX-SchPol,heightY,qRgb(0,255,0));
    }
    }
    else {
    for (SchOtr;SchOtr>0;SchOtr--)
    {
    tmpDst.setPixel(xOtr+SchOtr,heightY,qRgb(255,0,0));
    }

    for (SchPol;SchPol>0;SchPol--)
    {
    tmpDst.setPixel(wigthX-SchPol,heightY,qRgb(255,0,0));
    }
    }
    tmpDst.setPixel(Xc,heightY,qRgb(255,0,255));
    xOtr = Xc;
    wigthX = Xc;
    SchPol = 0;
    SchOtr = 0;
    heightY++;
    red = qRed(tmpSrc.pixel(Xc,heightY));
    blue = qBlue(tmpSrc.pixel(Xc,heightY));
    green = qGreen(tmpSrc.pixel(Xc,heightY));
    qDebug() << "Red " << red << " Green " << green << " Blue " << blue;
    SchYup++;

    }

    red = qRed(tmpSrc.pixel(Xc,Yc));
    blue = qBlue(tmpSrc.pixel(Xc,Yc));
    green = qGreen(tmpSrc.pixel(Xc,Yc));

    heightY = Yc;
    while (red!=0 && green!=255 && blue!=255)
    {
    while(redPol!=0 && bluePol!=255 && greenPol!=255)
    {
    SchPol++;
    redPol = qRed(tmpSrc.pixel(wigthX,heightY));
    greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
    bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
    wigthX++;
    }

    redPol = qRed(tmpSrc.pixel(1,1));
    greenPol = qGreen(tmpSrc.pixel(1,1));
    bluePol = qBlue(tmpSrc.pixel(1,1));

    while(redOtr!=0 && blueOtr != 255 && greenOtr != 255)
    {
    SchOtr++;
    redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
    blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
    greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));
    xOtr--;
    }

    redOtr = qRed(tmpSrc.pixel(1,1));
    blueOtr = qBlue(tmpSrc.pixel(1,1));
    greenOtr = qGreen(tmpSrc.pixel(1,1));

    if(SchPol==SchOtr || SchPol== SchOtr+1 || SchPol== SchOtr-1)
    {
    KoefX++;
    for (SchOtr;SchOtr>0;SchOtr--)
    {
    tmpDst.setPixel(xOtr+SchOtr,heightY,qRgb(0,255,0));
    }

    for (SchPol;SchPol>0;SchPol--)
    {
    tmpDst.setPixel(wigthX-SchPol,heightY,qRgb(0,255,0));
    }
    }
    else {

    for (SchOtr;SchOtr>0;SchOtr--)
    {
    tmpDst.setPixel(xOtr+SchOtr,heightY,qRgb(255,0,0));
    }

    for (SchPol;SchPol>0;SchPol--)
    {
    tmpDst.setPixel(wigthX-SchPol,heightY,qRgb(255,0,0));
    }
    }
    tmpDst.setPixel(Xc,heightY,qRgb(255,0,255));
    xOtr = Xc;
    wigthX = Xc;
    SchPol = 0;
    SchOtr = 0;
    heightY--;
    red = qRed(tmpSrc.pixel(Xc,heightY));
    blue = qBlue(tmpSrc.pixel(Xc,heightY));
    green = qGreen(tmpSrc.pixel(Xc,heightY));
    qDebug() << "Red " << red << " Green " << green << " Blue " << blue;
    SchYup++;

   }
   }
*/
}


