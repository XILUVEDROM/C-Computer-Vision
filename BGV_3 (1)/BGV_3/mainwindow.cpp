#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

using namespace std;

 uint StructKomp=0;
 uint Prepyp=0;

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
            if(qBlue(tmpSrc.pixel(x,y))>value)
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

    int znachbin = ui->lineEdit_2-> text().toInt();
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
                tmpDst.setPixel(i,j,qRgb(255,255,0));
            }
        }
    }
    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}




void MainWindow::on_Assimetrya_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);


    int Xc = tmpSrc.width()/2;
    int Yc = tmpSrc.height()/2;
    qDebug() << "Xc=" << Xc<< "Yc=" << Yc;
    tmpDst.setPixel(Xc,Yc,qRgb(0,255,0));
    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));

    int schS=0,schN=0;
    QSize soi=tmpDst.size();

    int height = tmpSrc.height();
    int width = tmpSrc.width();
    int heightY = Yc;
    int wigthX = Xc;
    int xOtr = Xc;
    int SchYup=0; int KoefX=0;
    float ResY=0;
    int SchPol=0 , SchOtr=0;
    int YPol = Yc;
    int YOtr = Yc;
    int schn=0;

    int porog = ui->lineEdit-> text().toInt();


        int red = qRed(tmpSrc.pixel(Xc,Yc));
        int blue = qBlue(tmpSrc.pixel(Xc,Yc));
        int green = qGreen(tmpSrc.pixel(Xc,Yc));

        int redPol = qRed(tmpSrc.pixel(wigthX,heightY));
        int bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
        int greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));

        int redOtr = qRed(tmpSrc.pixel(wigthX,heightY));
        int blueOtr = qBlue(tmpSrc.pixel(wigthX,heightY));
        int greenOtr = qGreen(tmpSrc.pixel(wigthX,heightY));

        while (red!= 255 && blue!=0 && green!=255)
        {

              while(redPol!=255 && bluePol!=0 && greenPol!=255)
    {
                   wigthX++;
             redPol = qRed(tmpSrc.pixel(wigthX,heightY));
                greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
                 bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));

                 int red = qRed(tmpDst.pixel(wigthX,heightY));
                 int blue = qBlue(tmpDst.pixel(wigthX,heightY));
                 int green = qGreen(tmpDst.pixel(wigthX,heightY));

                 if ( green<=blue+porog )
                 {
                     schn++;
                     tmpDst.setPixel(wigthX,heightY,qRgb(255,0,0));

                 }

    }

    redPol = qRed(tmpSrc.pixel(1,1));
    greenPol = qGreen(tmpSrc.pixel(1,1));
    bluePol = qBlue(tmpSrc.pixel(1,1));

    while(redOtr!=255 && blueOtr != 0 && greenOtr != 255)
    {

     xOtr--;
    redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
    blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
    greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));

        int red1 = qRed(tmpDst.pixel(xOtr,heightY));
        int blue1 = qBlue(tmpDst.pixel(xOtr,heightY));
        int green1 = qGreen(tmpDst.pixel(xOtr,heightY));

    if ( green<=blue+porog )
    {
        schn++;
        tmpDst.setPixel(xOtr,heightY,qRgb(255,0,0));

    }


    }

    redOtr = qRed(tmpSrc.pixel(1,1));
    blueOtr = qBlue(tmpSrc.pixel(1,1));
    greenOtr = qGreen(tmpSrc.pixel(1,1));


  tmpDst.setPixel(Xc,Yc,qRgb(255,255,0));
    xOtr = Xc;
    wigthX = Xc;
    heightY--;
   red = qRed(tmpSrc.pixel(Xc,heightY));
   blue = qBlue(tmpSrc.pixel(Xc,heightY));
   green = qGreen(tmpSrc.pixel(Xc,heightY));

        }

wigthX=Xc;
xOtr = Xc;
heightY=Yc;
red = qRed(tmpSrc.pixel(Xc,Yc));
blue = qBlue(tmpSrc.pixel(Xc,Yc));
green = qGreen(tmpSrc.pixel(Xc,Yc));

redPol = qRed(tmpSrc.pixel(wigthX,heightY));
bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));

redOtr = qRed(tmpSrc.pixel(wigthX,heightY));
blueOtr = qBlue(tmpSrc.pixel(wigthX,heightY));
greenOtr = qGreen(tmpSrc.pixel(wigthX,heightY));


        while (red!= 255 && blue!=0 && green!=255)
        {

              while(redPol!=255 && bluePol!=0 && greenPol!=255)
    {
                   wigthX++;
             redPol = qRed(tmpSrc.pixel(wigthX,heightY));
                greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
                 bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));

                 int red = qRed(tmpDst.pixel(wigthX,heightY));
                 int blue = qBlue(tmpDst.pixel(wigthX,heightY));
                 int green = qGreen(tmpDst.pixel(wigthX,heightY));

                 if ( green<=blue+porog )
                 {
                     schn++;
                     tmpDst.setPixel(wigthX,heightY,qRgb(255,0,0));

                 }


    }

    redPol = qRed(tmpSrc.pixel(1,1));
    greenPol = qGreen(tmpSrc.pixel(1,1));
    bluePol = qBlue(tmpSrc.pixel(1,1));

    while(redOtr!=255 && blueOtr != 0 && greenOtr != 255)
    {
    xOtr--;
    redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
    blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
    greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));
    int red = qRed(tmpDst.pixel(xOtr,heightY));
    int blue = qBlue(tmpDst.pixel(xOtr,heightY));
    int green = qGreen(tmpDst.pixel(xOtr,heightY));

    if ( green<=blue+porog )
    {
        schn++;
        tmpDst.setPixel(xOtr,heightY,qRgb(255,0,0));

    }


    }

    redOtr = qRed(tmpSrc.pixel(1,1));
    blueOtr = qBlue(tmpSrc.pixel(1,1));
    greenOtr = qGreen(tmpSrc.pixel(1,1));

    xOtr = Xc;
    wigthX = Xc;
    heightY++;
   red = qRed(tmpSrc.pixel(Xc,heightY));
   blue = qBlue(tmpSrc.pixel(Xc,heightY));
   green = qGreen(tmpSrc.pixel(Xc,heightY));

        }


   gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));

   }




void MainWindow::on_pushButton_5_clicked()
{
    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    QString after;
    QSet <QString> colorh;

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
              //cout<<"d="<<d<<endl;

              if(0<=d && d<=64)
              {
                  ng="black";
                  colorh.insert(ng);

              }
              if(64<d && d<=192)
              {
                  ng="Dblue";
                  colorh.insert(ng);
              }
              if(192<d && d<=16512)
              {
                  ng="Blue";
                    colorh.insert(ng);
              }
              if(16512<d && d<=32832)
              {
                  ng="DGreen";
                    colorh.insert(ng);
              }
              if(32832<d && d<=49088)
              {
                  ng="Temno-Golyboi";
                    colorh.insert(ng);
              }
              if(49088<d && d<=65408)
              {
                  ng="Green";
                    colorh.insert(ng);
              }
              if(65408<d && d<=4227072)
              {
                  ng="Golyboi";
                    colorh.insert(ng);
              }
              if(4227072<d && d<=8388672)
              {
                  ng="Korichevyi";
                    colorh.insert(ng);
              }
              if(8388672<d && d<=8405056)
              {
                  ng="magenta";
                     colorh.insert(ng);
              }
              if(8405056<d && d<=8421440)
              {
                  ng="T-yellow";
                     colorh.insert(ng);
              }
              if(8421440<d && d<=9474194)
              {
                  ng="Grey";
                     colorh.insert(ng);
              }
              if(9474194<d && d<=11579570)
              {
                  ng="s_grey";
                     colorh.insert(ng);
              }
              if(11579570<d && d<=14671968)
              {
                  ng="Serebro";
                     colorh.insert(ng);
              }
              if(14671968<d && d<=16711808)
              {
                  ng="Red";
                     colorh.insert(ng);
              }
              if(16711808<d && d<=16744448)
              {
                  ng="S-magenta";
                     colorh.insert(ng);
              }
              if(16744448<d  && d<=16777088)
              {
                  ng="Yellow";
                    colorh.insert(ng);
              }
              if(16777088<d && d<=16777215)
              {
                  ng="White";
                    colorh.insert(ng);
              }


              cout << "Содержимое set: ";

              //copy(colorh.begin(), colorh.end(), ostream_iterator<QString>(cout));
              //for_each(colorh.begin(),colorh.end(),[](const auto& t){cout<<t.toStdString().c_str()<<' ';});


              //copy(colorh.begin(), colorh.end(), ostream_iterator<QString>(cout));
              //for_each(colorh.begin(),colorh.end(),[](const auto& t){cout<<t.toStdString().c_str()<<' ';});
              //for_each(colorh.begin(),colorh.end(),[](const auto& t){if(t=="White"|| t=="Red"||t="Korichevyi"||t=="blue"||t=="black"||t=="s_grey") {Svet++;}});


              }
        }
}

void MainWindow::on_pushButton_14_clicked()
{
    QImage ghy = gpiDst->pixmap().toImage();
    int integer_value = ui->lineEdit_3->text().toInt();
    tmp = kmean(integer_value, src, ghy);
    gpiDst->setPixmap(QPixmap::fromImage(tmp));
}

QImage MainWindow::kmean(int k, QImage img, QImage mask) //К средних
{
    int max, min;
    max = 0;
    min = 255;
    for (int x = 0; x < img.width(); x++)
    {
        for (int y = 0; y < img.height(); y++)
        {
            if (mask.pixel(x,y)==qRgb(0,0,0))
            {
                if (qGreen(img.pixel(x,y))>max) { max=qGreen(img.pixel(x,y)); }
                if (qGreen(img.pixel(x,y))<min) { min=qGreen(img.pixel(x,y)); }
            }

        }
    }
    //  k - число кластеров. тогда стенок между кластерами k+1. тогда
    bool set;
    int wall[k+1]; //координаты стенок в цветовом пространстве (градации серого)
    for (int i = 0; i < k+1; i++) //перебираем стенки
    {
        wall[i] = i*(max-min)/k; //линейное разбиение
    }
    for (int x = 0; x < img.width(); x++)
    {
        for (int y = 0; y < img.height(); y++)
        {
            if (mask.pixel(x,y)==qRgb(0,0,0))
            {
                set = false;
                for (int n = 0; n < k; n++) //перебираем кластеры
                {
                    if (qGreen(img.pixel(x,y))<wall[n+1])
                    {
                        img.setPixel(x,y,qRgb(min+wall[n],min+wall[n],min+wall[n]));
                        break;
                    }
                    //if ( set==false )   { img.setPixel(x,y,qRgb(wall[0],wall[0],wall[0])); }
                }
            }
            else { img.setPixel(x,y,qRgb(255,255,255)); }
        }
    }
     StructKomp = 0;
     StructKomp=rand()*5-1;
    return img;
}

void MainWindow::on_pushButton_6_clicked()
{
    tmp = gpiDst->pixmap().toImage();
    QImage nin=innerFill(tmp);
    //bin = nin;
    //Fill(0,0,qRgb(255,255,255),qRgb(255,0,0));
    gpiDst->setPixmap(QPixmap::fromImage(nin));
}
QImage MainWindow::innerFill(QImage img)
{
    QImage mask(img);
    QImage mask1(img);

    //просто зальем внешнюю область. она ОДНА. ОДНА. ОДНА, КАРЛ. ВСЕ ОСТАЛЬНЫЕ ВНУТРЕННИЕ.

    //находим у края пустое место и заливаем зеленым на маске
    for ( int y = 0; y < mask.height(); y++ )
    {
        if ( mask.pixel(0, y)!=qRgb(0,0,0) ) { mask = Fill( mask, 0, y, mask.pixel(0, y), qRgb(0, 255, 0)); }
        if ( mask.pixel(mask.width()-1, y)!=qRgb(0,0,0) ) { mask = Fill(mask, mask.width()-1, y, mask.pixel(mask.width()-1, y), qRgb(0, 255, 0)); }
    }
    for ( int x = 0; x < mask.height(); x++ )
    {
        if ( mask.pixel(x, 0)!=qRgb(0,0,0) ) { mask = Fill(mask, x, 0, mask.pixel(x, 0), qRgb(0, 255, 0)); }
        if ( mask.pixel(x, mask.height()-1)!=qRgb(0,0,0) ) { mask = Fill(mask, x, mask.height()-1, mask.pixel(x, mask.height()-1), qRgb(0, 255, 0)); }
    }


    //а теперь заливаем черным все, кроме маски
    QImage tmpSrc(img);
    for (int y = 0; y < mask.height(); y++)
    {
        for (int x = 0; x < mask.width(); x++)
        {
            if ( mask.pixel(x, y)==qRgb(255,255,255) )  { tmpSrc.setPixel(x, y, qRgb(0, 0, 0)); }
        }
    }
    return tmpSrc;
}

QImage MainWindow::Fill(QImage img, int x, int y, QRgb colorFrom, QRgb colorTo) //заливка области
{
    QImage tmpSrc(img);

    std::stack<std::pair<int,int>> stack;
    stack.push(std::make_pair(x,y)); //закидываем координаты в стек, одновременно преобразуя их в пару
    int x1 = 0, y1 = 0;

    while (!stack.empty())
    {
        x = stack.top().first;
        y = stack.top().second;
        stack.pop();

        if ( x>=0 && x<tmpSrc.width() && y>=0 && y<tmpSrc.height())
        {
            if (tmpSrc.pixel(x,y) == colorFrom) { tmpSrc.setPixel(x,y,colorTo); }
        }
        for (int i = 0; i < 4; i++)
        {
            if (i == 0) { x1 = x+1; y1 = y; }
            if (i == 1) { x1 = x; y1 = y+1; }
            if (i == 2) { x1 = x-1; y1 = y; }
            if (i == 3) { x1 = x; y1 = y-1; }

            if ( x1>=0 && x1<tmpSrc.width() && y1>=0 && y1<tmpSrc.height())
            {
                if (tmpSrc.pixel(x1,y1) == qRgb(255,255,255))
                    {
                        stack.push(std::make_pair(x1,y1));
                        tmpSrc.setPixel(x,y,colorTo);
                    }
            }
        }
    }
    img = tmpSrc;
    //scDst->addPixmap(QPixmap::fromImage(tmp)); //нельзя добавлять GPI - скажет что уже добавлен
    return img;
}
void MainWindow::on_pushButton_7_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    int green;

    for (int x = 0; x < tmpSrc.width(); x++)
    {
        for (int y = 0; y < tmpSrc.height(); y++)
        {
            green = qGreen(tmpSrc.pixel(x,y));
            tmpDst.setPixel(x,y,QColor(0, green, 0).rgb());
        }
    }
    tmp = tmpDst;

    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::on_pushButton_8_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    int grey;

    for (int x = 0; x < tmpSrc.width(); x++)
    {
        for (int y = 0; y < tmpSrc.height(); y++)
        {
            grey = qGray(tmpSrc.pixel(x,y));
            tmpDst.setPixel(x,y,QColor(grey, grey, grey).rgb());
        }
    }

    tmp = tmpDst;

   gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

    //ui->dstGrView->setScene(scTmp);
}

void MainWindow::on_pushButton_9_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    int sobelY[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {+1, +2, +1}
    };
    int sobelX[3][3] = {
        {-1, 0, +1},
        {-2, 0, +2},
        {-1, 0, +1}
    };

    int Gx, Gy, pix, grad;

    for (int x = 0; x < tmpSrc.width(); x++)
    {
        for (int y = 0; y < tmpSrc.height(); y++)
        {
            Gx = 0;
            Gy = 0;
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++)
                {
                    if ( ((x+i)==tmpSrc.width()) || ((x+i)==-1) || ((y+j)==tmpSrc.height()) || ((y+j)==-1) ) { pix = 0; }
                    else { pix = qGreen(tmpSrc.pixel(x+i,y+j)); }

                    //срисовали пиксели. во избежание ошибок выходящие за пределы исходного изображения - забили нулями
                    Gx += abs( qGreen(tmpSrc.pixel(x,y)) - pix )*abs(sobelX[1+i][1+j]);
                    Gy += abs( qGreen(tmpSrc.pixel(x,y)) - pix )*abs(sobelY[1+j][1+i]);

                    //Gx += pix*sobelX[1+i][1+j];
                    //Gy += pix*sobelY[1+j][1+i];
                }
            }
            grad = sqrt(Gx*Gx+Gy*Gy);
            tmpDst.setPixel(x,y,qRgb(grad, grad, grad));
        }
    }
    tmp = tmpDst;

    Prepyp=grad;

    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::on_pushButton_10_clicked()
{
    tmp = gpiSrc->pixmap().toImage();
    QImage nin = watershed1(tmp);

    gpiDst->setPixmap(QPixmap::fromImage(nin));
}

QImage MainWindow::watershed1(QImage dist)
{
    // на входе - изображение после преобразования расстояний -

    QImage label(dist);

    int w = dist.width();
    int h = dist.height();

    //====================================================================================================
    // ищем плато минимумов.

    for (int y = 1; y < h; y++) //пробегаемся снизу верх, слева направо
    {
        for (int x = 1; x < w; x++)
        {
            if ( qGreen(dist.pixel(x,y))<=qGreen(dist.pixel(x-1,y)) && qGreen(dist.pixel(x,y))<=qGreen(dist.pixel(x,y-1)) )

            {
                //маркируем как член плато минимума;
                label.setPixel(x,y,qRgb(0,0,0));
            }
            else
            {
                label.setPixel(x,y,qRgb(255,255,255));
            }
        }
    }
    for (int y = h-2; y >= 0; y--) //пробегаемся сверху вниз, справа налево
    {
        for (int x = w-2; x >=0 ; x--)
        {
            if ( label.pixel(x,y)==qRgb(0,0,0) )
            {
                if ( ( qGreen(dist.pixel(x,y))<=qGreen(dist.pixel(x+1,y)) || qGreen(dist.pixel(x,y))<=qGreen(dist.pixel(x,y+1)) ))
                {
                    //маркируем как член плато минимума;
                    label.setPixel(x,y,qRgb(0,0,0));
                }
                else
                {
                    label.setPixel(x,y,qRgb(255,255,255));
                }
            }
        }
    }


delBorders(label);
return label;
}

QImage MainWindow::delBorders(QImage img)
{
    //QImage img=gpiSrc->pixmap().toImage();
    QImage imh1(img);
    QImage ltl(img.width()-2, img.height()-2, img.format());
    for (int i = 0; i < ltl.width(); i++)
    {
        for (int j = 0; j < ltl.height(); j++)
        {
            ltl.setPixel(i, j, imh1.pixel(i+1,j+1));
        }
    }

    //img = ltl;
    //tmp = tmpSrc;
   gpiDst->setPixmap(QPixmap::fromImage(ltl));
    return ltl;
}
void MainWindow::on_pushButton_11_clicked()
{
    QImage tmpSrc = gpiSrc->pixmap().toImage();
    QImage gis(255,100,QImage::Format_RGB888);
    gis.fill(0x00FF00FF);
    QPainter p(&gis);
    int g[255];
    for(int i=0;i<255;i++)
        g[i]=0;
    for(int y=0;y<tmpSrc.height();y++)
        for(int x=0;x<tmpSrc.width();x++)
            ++g[qGreen(tmpSrc.pixel(x,y))];
    g[0]=0;
    int max = g[0];
    for(int i=0;i<255;i++){
        if(g[i]>max)
            max=g[i];
    }
    for(int i=0;i<255;i++)
        g[i]=(100*g[i]/max);
    for(int i=0;i<255;i++)
        p.drawRect(QRect(QPoint(i,gis.height()-0),QPoint(i,gis.height()-g[i])));
    ui->label ->setPixmap(QPixmap::fromImage(gis));
}

void MainWindow::on_pushButton_12_clicked()
{

}

void MainWindow::on_pushButton_13_clicked()
{
    QImage nin = gpiDst->pixmap().toImage();
    dst = distance(nin, qRgb(0,0,0), qRgb(255,255,255) ); //внутри - черное. темное - глубоко, светлое - мелко.
      gpiDst->setPixmap(QPixmap::fromImage(dst));
}

QImage MainWindow::distance(QImage bin, QRgb in, QRgb out)
{
    QImage src(bin);
    QImage distance(bin);

    //int maxdistance=0;

    //определим начальные расстояния для внутренних точек, расстояния для внешних
    float dcs[src.width()][src.height()];
    for (int x = 0; x < src.width(); x++)
    {
        for (int y = 0; y < src.height(); y++)
        {
            if (src.pixel(x,y)==out) { dcs[x][y]=0; }
            else { dcs[x][y]=100000; }
        }
    }

    //==============================================================================
    //==============================================================================

    float dx, dy, dxy;
    int temp;
    dx = 1;
    dy = 1;
    dxy = sqrt(2);

    //perform the first pass ("first picture scan")
    for (int y=1; y<=src.height()-1; y++)
    {
        for (int x=0; x<=src.width()-1; x++)
        {
            if (src.pixel(x,y)==in)
            {
                if (x>0)    { dcs[x][y] = std::min(dcs[x-1][y-1]+dxy, dcs[x][y]); }

                dcs[x][y] = std::min(dcs[x][y-1]+dy, dcs[x][y]);

                if (x<src.width()-1)  { dcs[x][y] = std::min(dcs[x+1][y-1]+dxy, dcs[x][y]); }
            }
        }
        for (int x=1; x<=src.width()-1; x++)
        {
            if (src.pixel(x,y)==in) { dcs[x][y] = std::min(dcs[x-1][y]+dx, dcs[x][y]); }
        }
        for (int x=src.width()-2; x>=0; x--)
        {
            if (src.pixel(x,y)==in) { dcs[x][y] = std::min(dcs[x+1][y]+dx, dcs[x][y]); }
        }
    }

    //perform the final pass ("second picture scan")
    for (int y=src.height()-2; y>=0; y--)
    {
        for (int x=0; x<=src.width()-1; x++)
        {
            if (src.pixel(x,y)==in)
            {
                if (x>0)    { dcs[x][y] = std::min(dcs[x-1][y+1]+dxy, dcs[x][y]); }

                dcs[x][y] = std::min(dcs[x][y+1]+dy, dcs[x][y]);

                if (x<src.width()-1) { dcs[x][y] = std::min(dcs[x+1][y+1]+dxy, dcs[x][y]); }
            }
        }
        for (int x=1; x<=src.width()-1; x++)
        {
            if (src.pixel(x,y)==in) { dcs[x][y] = std::min(dcs[x-1][y]+dx, dcs[x][y]); }
        }

        for (int x=src.width()-2; x>=0; x--)
        {
            if (src.pixel(x,y)==in) { dcs[x][y] = std::min(dcs[x+1][y]+dx, dcs[x][y]); }
        }
    }

    //========================================================================================
    //========================================================================================
    //ищем максимум для правильного преобразования.

    int maxdistance = 0;

    for (int x = 0; x < src.width(); x++)
    {
        for (int y = 0; y < src.height(); y++)
        {
            maxdistance = std::max((int)dcs[x][y], maxdistance);
        }
    }

    //===========================================================================================
    //===========================================================================================

    for (int x = 0; x < src.width(); x++)
    {
        for (int y = 0; y < src.height(); y++)
        {
            temp = dcs[x][y]/maxdistance*255;
            distance.setPixel(x,y, qRgb(temp,temp,temp));
        }
    }
     StructKomp=temp;
    return distance;
}
