    #include "mainwindow.h"
    #include "ui_mainwindow.h"
    #include <QFileDialog>
    #include <set>
    #include <vector>
    #include <iostream>
    #include <iterator>
    #include <QSet>
    #include <string.h>
    #include <stack>
    #include <vector>
    #include "otchetx3.h"
    #include "ui_otchetx3.h"
    #include "otchetalddiv.h"
    #include "ui_otchetalddiv.h"
    #include "error.h"
    #include "ui_error.h"
    #include <countour.h>
    #include "imagobject.h"
    #include <vector>
    #include <stdlib.h>
    #include <chrono>
    #include <thread>
    #include <math.h>
    #include "opencv2/core/core.hpp"
    #include "opencv2/highgui/highgui.hpp"
    #include "opencv2/imgproc/imgproc.hpp"
    #include "anizo.h"
    #include "ui_anizo.h"
    #include "original.h"
    #include "ui_original.h"

    #define CV_PIXEL(type,img,x,y) (((type*)((img)->imageData+(y)*(img)->widthStep))+(x)*(img)->nChannels)

    using namespace std;

    int zfp;
    QString Pere="Ne vipolnena func simmetriya";
    uint f1=0,f2=0;
    float f3=0;
    QSet <QString> fka;
    uint OsXs=0,OsYs=0;
    uint Prepyv=0;
    uint Prepyp=0;
    uint Svet=0;
    uint StructKomp=0;
   // anizo * gtr = new anizo;
    //vector to store our objects
    vector <ImgObject> objects;
    //vector to store our countours
    vector <Countour> countours;
    QSet <int> svetu;
    anizo * gtr;
    QString pert;

    MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    {
    ui->setupUi(this);

    ui->srcGrView->setScene(&scSrc);
    ui->dstGrView->setScene(&scDst);

    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::MedianF(QImage &dst, int r)
    {

    }

    void MainWindow::on_action_triggered()
    {
    QString fp = QFileDialog::getOpenFileName();
    pert = fp;

    gpiSrc = new MyGraphicsItem(QPixmap(fp));
    gpiDst = new MyGraphicsItem(QPixmap(fp));
    gpiOriginal = new MyGraphicsItem (QPixmap(fp));

    QImage tmpOri = gpiOriginal->pixmap().toImage();

    scSrc.addItem(gpiSrc);
    scDst.addItem(gpiDst);

   Original *Ori = new Original();
   Ori->show();
   Ori->PolOriginal(tmpOri);

    ui->verticalSlider->setRange(0, scDst.width());
    ui->verticalSlider_4->setRange(0,scDst.width());
    ui->verticalSlider_2->setRange(0,scDst.height());
    ui->verticalSlider_3->setRange(0,scDst.height());
    }

    void MainWindow::on_horizontalSlider_valueChanged(int value)
    {
        QImage tmpSrc =gpiSrc->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        for(int y=0; y<tmpSrc.height();++y)
            for(int x=0; x<tmpSrc.width();++x)
            {
                int r = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))+value));
                int g = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))+value));
                int b = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))+value));

                tmpDst.setPixel(x,y,qRgb(r,g,b));
            }
        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_horizontalSlider_2_valueChanged(int value)
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
        QImage tmpSrc =gpiSrc->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        unsigned int MaskSumm=0;
        char Mask[5][5] = {{1,1,1,1,1},
                           {1,1,1,1,1},
                           {1,1,1,1,1},
                           {1,1,1,1,1},
                           {1,1,1,1,1}};
        for(int i=0;i<5;i++)
            for(int j=0;j<5;j++)
                MaskSumm+=Mask[i][j];

    for(int x=2; x<tmpSrc.width()-2;x++)
        for(int y=2;y<tmpSrc.height()-2;y++)
        {
            int sumR=0;int sumG=0;int sumB=0;
                for(int i=0;i<5;i++)
                    for (int j=0;j<5;j++)
                    {
                        sumR+=(Mask[i][j]*qRed(tmpSrc.pixel(x+i-2,y+j-2)));
                        sumG+=(Mask[i][j]*qRed(tmpSrc.pixel(x+i-2,y+j-2)));
                        sumB+=(Mask[i][j]*qRed(tmpSrc.pixel(x+i-2,y+j-2)));
                    }
                tmpDst.setPixel(x,y,qRgb(sumR/MaskSumm,sumG/MaskSumm,sumB/MaskSumm));
        }
        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_pushButton_2_clicked()
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        int znachbin = ui->lineEdit-> text().toInt();
        cout<<znachbin<<endl;
        int znb=znachbin+1;
        cout<<znb<<endl;

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
                 sort(vec.begin(),vec.end());
                 tmpDst.setPixel(w,h,vec.at(znachbin*znachbin*4));
                 vec.clear();
              }
          }
           gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_pushButton_3_clicked()
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
        ui->label_3 ->setPixmap(QPixmap::fromImage(gis));
    }

    void MainWindow::on_action_2_triggered()
    {
        exit(0);
    }

    void MainWindow::on_pushButton_4_clicked()
    {
        on_pushButton_2_clicked();
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);
       // on_pushButton_19_clicked();
        set <int> coordinatex;
        set <int> coordinatey;

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
                       coordinatex.insert(i);
                       coordinatey.insert(j);
                  }

                  bool RightPixelIsWhite = false;
                  int redR = qRed(tmpSrc.pixel(i+1,j));
                  int blueR = qBlue(tmpSrc.pixel(i+1,j));
                  int greenR = qGreen(tmpSrc.pixel(i+1,j));
                  if(redR==0 && blueR==0 && greenR==0){
                      RightPixelIsWhite=true;
                      coordinatex.insert(i);
                      coordinatey.insert(j);
                  }

                  bool DownPixelIsWhite = false;
                  int redD = qRed(tmpSrc.pixel(i,j-1));
                  int blueD = qBlue(tmpSrc.pixel(i,j-1));
                  int greenD = qGreen(tmpSrc.pixel(i,j-1));
                  if(redD==0 && blueD==0 && greenD==0){
                      DownPixelIsWhite=true;
                      coordinatex.insert(i);
                      coordinatey.insert(j);
                  }

                  bool LeftPixelIsWhite = false;
                  int redL = qRed(tmpSrc.pixel(i-1,j));
                  int blueL = qBlue(tmpSrc.pixel(i-1,j));
                  int greenL = qGreen(tmpSrc.pixel(i-1,j));
                  if(redL==0 && blueL==0 && greenL==0){
                      LeftPixelIsWhite=true;
                      coordinatex.insert(i);
                      coordinatey.insert(j);
                  }

                  bool UpRightPixelIsWhite = false;
                  int redUR = qRed(tmpSrc.pixel(i+1,j+1));
                  int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
                  int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
                  if(redUR==0 && blueUR==0 && greenUR==0){
                      UpRightPixelIsWhite=true;
                      coordinatex.insert(i);
                      coordinatey.insert(j);
                  }

                  bool UpLeftPixelIsWhite = false;
                  int redUL = qRed(tmpSrc.pixel(i-1,j+1));
                  int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
                  int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
                  if(redUL==0 && blueUL==0 && greenUL==0){
                      UpLeftPixelIsWhite=true;
                      coordinatex.insert(i);
                      coordinatey.insert(j);
                  }

                  bool DownRightPixelIsWhite = false;
                  int redDR = qRed(tmpSrc.pixel(i+1,j-1));
                  int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
                  int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
                  if(redDR==0 && blueDR==0 && greenDR==0){
                      DownRightPixelIsWhite=true;
                      coordinatex.insert(i);
                      coordinatey.insert(j);
                  }

                  bool DownLeftPixelIsWhite = false;
                  int redDL = qRed(tmpSrc.pixel(i-1,j-1));
                  int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
                  int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
                  if(redDL==0 && blueDL==0 && greenDL==0){
                      DownLeftPixelIsWhite=true;
                      coordinatex.insert(i);
                      coordinatey.insert(j);
                  }

                  if ((red==255 && blue==255 && green==255) && (UpPixelIsWhite==true || RightPixelIsWhite==true || DownPixelIsWhite==true || LeftPixelIsWhite==true ||
                          UpRightPixelIsWhite==true || UpLeftPixelIsWhite==true || DownLeftPixelIsWhite==true || DownRightPixelIsWhite==true)){
                      tmpDst.setPixel(i,j,qRgb(255,0,0));
                      peregacha(i,j);
                  }
              }
          }

          on_pushButton_26_clicked();
      gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_lineEdit_cursorPositionChanged(int arg1, int arg2) {}

    void MainWindow::on_pushButton_5_clicked()
    {
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        ui->lineEdit_5->clear();
        // Допилить привязку к значения внизу слайдера, сделать более правильное обновлние.
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        int x = ui->verticalSlider->value();
        int y = ui->verticalSlider_2->value();
        int heigh = ui->verticalSlider_3->value();
        int widt = ui->verticalSlider_4->value();

        QString g1 = QString::number(x);
        QString g2 = QString::number(y);
        QString g3 = QString::number(heigh);
        QString g4 = QString::number(widt);

        ui->lineEdit_2->insert(g1);
        ui->lineEdit_3->insert(g2);
        ui->lineEdit_4->insert(g3);
        ui->lineEdit_5->insert(g4);

        tmpDst = tmpDst.copy (x,y,heigh,widt);

        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
        gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_pushButton_6_clicked()
    {
        int schS=0,schN=0;
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);
        //connect(sliderA, SIGNAL(valueChanged(int)), lcdAL, SLOT(display(int)));

        QSize soi=tmpDst.size();

        int height = soi.height();
        int width = soi.width();

        QString g5 = QString::number(height);
        QString g6 = QString::number(width);

        ui->lineEdit_6->setText(g5);
        ui->lineEdit_7->setText(g6);

        int centrx = height/2;
        int centry = width/2;

        QString x= QString::number(centrx);
        QString y= QString::number(centry);

        ui->lineEdit_8->insert(x);
        ui->lineEdit_8->insert(y);

        for(int i=0;i<tmpSrc.width();i++)
        {
            for (int j=0;j<tmpSrc.height()/2;j++)
            {
                 QRgb color =tmpSrc.pixel( i, j );
                 QColor col =QColor(color);

                 QString ng = col.name();
                 QString nb = QColor(color).name();

                 nb.remove(0,1);
                 int d = nb.toInt(nullptr,16);
                 QRgb color1 =tmpSrc.pixel( i, tmpSrc.height()-j );
                 QColor col1 =QColor(color1);

                 QString ng1 = col1.name();
                 QString nb1 = QColor(color1).name();

                 nb1.remove(0,1);

                int d1 = nb1.toInt(nullptr,16);

                if(d==d1)
                {
                    schS++;
                }

        }
     }
        for(int i=0;i<tmpSrc.height();i++)
        {
            for (int j=0;j<tmpSrc.width()/2;j++)
            {
                 QRgb color =tmpSrc.pixel( j,i );
                 QColor col =QColor(color);

                 QString ng = col.name();
                 QString nb = QColor(color).name();

                 nb.remove(0,1);
                 int d = nb.toInt(nullptr,16);
                 QRgb color1 =tmpSrc.pixel(  tmpSrc.width()-j,i );
                 QColor col1 =QColor(color1);

                 QString ng1 = col1.name();
                 QString nb1 = QColor(color1).name();

                 nb1.remove(0,1);

                int d1 = nb1.toInt(nullptr,16);

                if(d==d1)
                {
                    schN++;
                }

        }
        }
        uint KolvoToch = tmpSrc.height()*tmpSrc.width();
        uint JKol = schN+schS;
        uint KolvoToch1= KolvoToch/2;
        float Simmx=(float) schS/KolvoToch1;
        if(Simmx<0.9f)
        {
            OsXs++;
        }
        float Simmy=(float) schN/KolvoToch1;
        if(Simmy<0.9f)
        {
            OsXs++;
        }

        float Pal =(float) JKol/KolvoToch;
        if(Pal<=0.9f)
        {
            cout<<"Assimetriya" <<endl;
            cout<<Pal<<endl;
            cout<<JKol<<endl<<KolvoToch<<endl;
            Pere ="Ассиметрия"; f1=JKol; f2=KolvoToch; f3=Pal;
            Prepyv = rand()*8-4;
        }
        else
        {
            cout<<"Simmetriya"<<endl;
            Pere="Симметрия";
            Prepyv = rand()*4-0;
            f1=JKol; f2=KolvoToch; f3=Pal;
        }

    }

    QColor MainWindow::resolveColor(const QString & name)
    {
        QColor color;
        if (QColor::isValidColor(name))
            color.setNamedColor(name);
        else
        {
            // trying to parse "#RRGGBBAA" color
            if (name.length() == 9)
            {
                QString solidColor = name.left(7);
                if (QColor::isValidColor(solidColor))
                {
                    color.setNamedColor(solidColor);
                    int alpha = name.right(2).toInt(0, 16);
                    color.setAlpha(alpha);
                }
            }
        }

        if (!color.isValid())
            cout<<"Color is  invalud"<<endl;
        return color;
    }

    void MainWindow::on_pushButton_7_clicked()
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
                  for_each(colorh.begin(),colorh.end(),[](const auto& t){cout<<t.toStdString().c_str()<<' ';});
                  //for_each(colorh.begin(),colorh.end(),[](const auto& t){if(t=="White"|| t=="Red"||t="Korichevyi"||t=="blue"||t=="black"||t=="s_grey") {Svet++;}});

                  Svet = svetu.size();
                  }
            }
          }


    void MainWindow::on_pushButton_8_clicked()
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);
        //connect(sliderA, SIGNAL(valueChanged(int)), lcdAL, SLOT(display(int)));

        QSize soi=tmpDst.size();

        int height = soi.height();
        int width = soi.width();

        int centrx = height/2;
        int centry = width/2;

        QString x= QString::number(centrx);
        QString y= QString::number(centry);

        ui->lineEdit_8->insert(x);
        ui->lineEdit_8->insert(y);
    }



    void MainWindow::on_pushButton_11_clicked()
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

    void MainWindow::on_pushButton_10_clicked()
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
        //ui->srcGrView->setScene(scSrc);

        //QImage tmpDst1(tmpDst);
        //scDst->addPixmap(QPixmap::fromImage(tmpDst1));
        //ui->dstGrView->setScene(scDst);
    }

    void MainWindow::on_pushButton_9_clicked()
    {
        QImage tmpSrc =gpiSrc->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
        //ui->srcGrView->setScene(scSrc);
    }

    void MainWindow::on_pushButton_12_clicked()
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

    void MainWindow::on_pushButton_15_clicked()
    {
        tmp = gpiSrc->pixmap().toImage();
        nin = watershed1(tmp);

        gpiDst->setPixmap(QPixmap::fromImage(nin));
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

    void MainWindow::on_pushButton_16_clicked()
    {
        bin = gpiDst->pixmap().toImage();
        dst = distance(bin, qRgb(0,0,0), qRgb(255,255,255) ); //внутри - черное. темное - глубоко, светлое - мелко.
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

    void MainWindow::on_pushButton_14_clicked()
    {
        tmp = gpiDst->pixmap().toImage();
        nin=innerFill(tmp);
        bin = nin;
        //Fill(0,0,qRgb(255,255,255),qRgb(255,0,0));
        gpiDst->setPixmap(QPixmap::fromImage(bin));
    }

    void MainWindow::on_pushButton_13_clicked()
    {
        bin = gpiDst->pixmap().toImage();
        int integer_value = ui->lineEdit_9->text().toInt();
        tmp = kmean(integer_value, src, bin);
        gpiDst->setPixmap(QPixmap::fromImage(tmp));
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


    void MainWindow::Test()
    {


    }
    void MainWindow::on_pushButton_17_clicked()
    {
        QImage tmpSrc =gpiSrc->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        int Laplasian[3][3] = {
            {0, 1, 0},
            {1, -4, 1},
            {0, 1, 0}
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
                        Gx += abs( qGreen(tmpSrc.pixel(x,y)) - pix )*abs(Laplasian[1+i][1+j]);
                        Gy += abs( qGreen(tmpSrc.pixel(x,y)) - pix )*abs(Laplasian[1+j][1+i]);

                        //Gx += pix*sobelX[1+i][1+j];
                        //Gy += pix*sobelY[1+j][1+i];
                    }
                }
                grad = sqrt(Gx*Gx+Gy*Gy);
                tmpDst.setPixel(x,y,qRgb(grad, grad, grad));
            }
        }
        tmp = tmpDst;



         gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_horizontalSlider_3_actionTriggered(int action)
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        for(int y=0; y<tmpSrc.height();++y)
            for(int x=0; x<tmpSrc.width();++x)
            {
                int r = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))+action));
                int g = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))+action));
                int b = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))+action));

                tmpDst.setPixel(x,y,qRgb(r,g,b));

                cout<<"r="<<r<<" g="<<g<<" b="<<b<<endl;
            }
        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_pushButton_18_clicked()
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        for(int y=0; y<tmpSrc.height();++y)
            for(int x=0; x<tmpSrc.width();++x)
            {
                int r = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))));
                int g = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))));
                int b = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))));

                if (r>127 &&b>127&&g>127)
                {
                    tmpDst.setPixel(x,y,qRgb(0,0,0));
                }
            }
        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_pushButton_19_clicked()
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);
        QString after;
        QSet <QString> colorh;
         gtr = new anizo;

        bool mas[18];
        for(int i=0;i<18;i++)
            mas[i]=false;

          for (int i=0;i<tmpSrc.width();i++){
              for (int j=0;j<tmpSrc.height();j++){
                  QRgb color =tmpSrc.pixel( i, j );
                  QColor col =QColor(color);

                  int red = qRed(tmpSrc.pixel(i,j));
                  int blue = qBlue(tmpSrc.pixel(i,j));
                  int green = qGreen(tmpSrc.pixel(i,j));

                  QString ng = col.name();
                  QString nb = QColor(color).name();

                  nb.remove(0,1);

                  int d = nb.toInt(nullptr,16);
                  //cout<<"d="<<d<<endl;

                  if(red < 30 && green < 30 && blue < 30)
                  {
                      ng="white";
                      colorh.insert(ng);
                       svetu.insert(1);
                       mas[0]=true;
                  }
                  if((red>29 && red<65) && (green>29 && green<65) && (blue>29 && blue<65))
                  {
                      ng = "Rozovyi";
                      colorh.insert(ng);
                      mas[17] = true;
                  }
                  if((red > 64 && red < 192) && green<64 && blue < 64 )
                  {
                      ng="Dark Red";
                      colorh.insert(ng);
                      mas[1]=true;
                  }
                  if(red > 191 && green<64 && blue<64)
                  {
                      ng="Red";
                        colorh.insert(ng);
                         svetu.insert(1);
                         mas[2]=true;
                  }
                  if(red<64 && (green>64 && green<193) && blue<64)
                  {
                      ng="Dark Green";
                        colorh.insert(ng);
                        mas[3]=true;
                  }
                  if(red<64 && green>192 && blue<64)
                  {
                      ng="Green";
                        colorh.insert(ng);
                        mas[4]=true;
                  }
                  if(red<64 && green<64 && (blue>64 && blue<193))
                  {
                      ng="Dark blue";
                        colorh.insert(ng);
                        mas[5]=true;
                  }
                  if(red<64 && green<64 && blue>192)
                  {
                      ng="blue";
                        colorh.insert(ng);
                        mas[6]=true;
                  }
                  if(red<64 &&(green>64 && green<193)&&(blue>64 && blue<193))
                  {
                      ng="Dark golyboi";
                        colorh.insert(ng);
                         svetu.insert(1);
                         mas[7]=true;
                  }
                  if(red<64 && green>192 && blue>192)
                  {
                      ng="Golyboi";
                         colorh.insert(ng);
                         mas[8]=true;
                  }
                  if((red>64 && red<193) && green<64 && (blue>64 && blue <193))
                  {
                      ng="Dark magenta";
                         colorh.insert(ng);
                         mas[9]=true;
                  }
                  if(red>192 && green< 64 && blue>192)
                  {
                      ng="Magenta";
                         colorh.insert(ng);
                          svetu.insert(1);
                          mas[10]=true;
                  }
                  if((red>64 && red<193) && (green>64 && green<193) && blue<64)
                  {
                      ng="Dark yellow";
                         colorh.insert(ng);
                          svetu.insert(1);
                          mas[11]=true;
                  }
                  if(red>192 && green>192 && blue<64)
                  {
                      ng="Yellow";
                         colorh.insert(ng);
                         mas[12]=true;
                  }
                  if((red>64 && red<160) && (green>64 && green<160) && (blue>64 && blue<164))
                  {
                      ng="dark Grey";
                         colorh.insert(ng);
                         svetu.insert(1);
                         mas[13]=true;
                  }
                  if(red==160 && green == 160 && blue==164)
                  {
                      ng="Grey";
                         colorh.insert(ng);
                         mas[14]=true;
                  }
                  if((red>160 && red<210) && (green>160 && green<210) && (blue>164 && blue<210))
                  {
                      ng="Light Grey";
                        colorh.insert(ng);
                        mas[15]=true;
                  }
                  if(red>209 && green>209 && blue>209)
                  {
                      ng="Black";
                        colorh.insert(ng);
                        mas[16]=true;
                  }
                   gtr->Colorper(mas[0],mas[1],mas[2],mas[3],mas[4],mas[5],mas[6],mas[7],mas[8],mas[9],mas[10],mas[11],mas[12],mas[13],mas[14],mas[15],mas[16],mas[17]);

                  // cout << "Содержимое set: ";
                  //copy(colorh.begin(), colorh.end(), ostream_iterator<QString>(cout));
                  //for_each(colorh.begin(),colorh.end(),[](const auto& t){cout<<t.toStdString().c_str()<<' ';});
                  //for_each(colorh.begin(),colorh.end(),[](const auto& t){if(t=="White"|| t=="Red"||t="Korichevyi"||t=="blue"||t=="black"||t=="s_grey") {Svet++;}});

                  Svet = svetu.size();
                  }
            }
    }

    void MainWindow::on_pushButton_20_clicked()
    {
        Error *Y1= new Error;
        cout<<"OsXs"<<OsXs<<endl;
        if (OsXs==0)
        {
            connect(ui->pushButton_20, SIGNAL(clicked()), this, SLOT(onButtonSend2())); // подключаем клик по кнопке к определенному нами слоту
            connect(this, SIGNAL(sendData2(QString)),Y1 , SLOT(recieveData(QString))); // подключение сигнала к слоту нашей формы
            connect(ui->pushButton_20, SIGNAL(clicked()),Y1,SLOT(show()));
        }
        else if (Prepyv==0) {
            connect(ui->pushButton_20, SIGNAL(clicked()), this, SLOT(onButtonSend3())); // подключаем клик по кнопке к определенному нами слоту
            connect(this, SIGNAL(sendData3(QString)),Y1 , SLOT(recieveData(QString))); // подключение сигнала к слоту нашей формы
            connect(ui->pushButton_20, SIGNAL(clicked()),Y1,SLOT(show()));
        }
        else if (Svet==0) {
            connect(ui->pushButton_20, SIGNAL(clicked()), this, SLOT(onButtonSend4())); // подключаем клик по кнопке к определенному нами слоту
            connect(this, SIGNAL(sendData4(QString)),Y1 , SLOT(recieveData(QString))); // подключение сигнала к слоту нашей формы
            connect(ui->pushButton_20, SIGNAL(clicked()),Y1,SLOT(show()));
        }
        Otchetx3 *R1 = new Otchetx3;
        connect(ui->pushButton_20, SIGNAL(clicked()), this, SLOT(onButtonSend())); // подключаем клик по кнопке к определенному нами слоту
        connect(this, SIGNAL(sendData(QString,uint,uint,float,uint,uint)),R1 , SLOT(recieveData(QString,uint,uint,float,uint,uint))); // подключение сигнала к слоту нашей формы
          connect(ui->pushButton_20, SIGNAL(clicked()),R1,SLOT(show()));
        cout<<flush;
    }

    void MainWindow::onButtonSend()
    {
        emit sendData(Pere,f1,f2,f3,Prepyv,Svet);// вызываем сигнал, в котором передаём введённые данные
    }

    void MainWindow::on_pushButton_21_clicked()
    {
        Error *Y1= new Error;
        cout<<"OsXs"<<OsXs<<endl;
        if (OsXs==0)
        {
            connect(ui->pushButton_21, SIGNAL(clicked()), this, SLOT(onButtonSend2())); // подключаем клик по кнопке к определенному нами слоту
            connect(this, SIGNAL(sendData2(QString)),Y1 , SLOT(recieveData(QString))); // подключение сигнала к слоту нашей формы
            connect(ui->pushButton_21, SIGNAL(clicked()),Y1,SLOT(show()));
        }
        else if (Prepyv==0) {
            connect(ui->pushButton_21, SIGNAL(clicked()), this, SLOT(onButtonSend3())); // подключаем клик по кнопке к определенному нами слоту
            connect(this, SIGNAL(sendData3(QString)),Y1 , SLOT(recieveData(QString))); // подключение сигнала к слоту нашей формы
            connect(ui->pushButton_21, SIGNAL(clicked()),Y1,SLOT(show()));
        }
        else if (Svet==0) {
            connect(ui->pushButton_21, SIGNAL(clicked()), this, SLOT(onButtonSend4())); // подключаем клик по кнопке к определенному нами слоту
            connect(this, SIGNAL(sendData4(QString)),Y1 , SLOT(recieveData(QString))); // подключение сигнала к слоту нашей формы
            connect(ui->pushButton_21, SIGNAL(clicked()),Y1,SLOT(show()));
        }
        else if (StructKomp) {
            connect(ui->pushButton_21, SIGNAL(clicked()), this, SLOT(onButtonSend5())); // подключаем клик по кнопке к определенному нами слоту
            connect(this, SIGNAL(sendData5(QString)),Y1 , SLOT(recieveData(QString))); // подключение сигнала к слоту нашей формы
            connect(ui->pushButton_21, SIGNAL(clicked()),Y1,SLOT(show()));
        }
        OtchetAldDiv *R1 = new OtchetAldDiv;
        connect(ui->pushButton_21, SIGNAL(clicked()), this, SLOT(onButtonSend1())); // подключаем клик по кнопке к определенному нами слоту
        connect(this, SIGNAL(sendData1(uint,uint,uint,uint)),R1 , SLOT(recieveData(uint,uint,uint,uint))); // подключение сигнала к слоту нашей формы
          connect(ui->pushButton_21, SIGNAL(clicked()),R1,SLOT(show()));
        cout<<flush;
    }
    void MainWindow::onButtonSend1()
    {
        emit sendData1(OsXs,Prepyv,Svet,StructKomp);// вызываем сигнал, в котором передаём введённые данные
    }


    void MainWindow::StoreCountours()
    {

    //std::this_thread::sleep_for(std::chrono::milliseconds(5000));

        QImage tmpSrc = gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);


        for (int i=1;i<tmpDst.width()-1;i++){
            for (int j=1;j<tmpDst.height()-1;j++){

                int red = qRed(tmpDst.pixel(i,j));
                int blue = qBlue(tmpDst.pixel(i,j));
                int green = qGreen(tmpDst.pixel(i,j));

                //check if pixel is red

                if(red==255 && green==0 && blue==0){

                    Countour c;
                    bool CountourTraceUnfinished=true;
                    int tmpi=i;
                    int tmpj=j;

                    while (CountourTraceUnfinished==true) {


                         c.PushValue(tmpi,tmpj);
                         tmpDst.setPixel(tmpi,tmpj,qRgb(0,0,0));
                         //int showme = qRed(tmpSrc.pixel(tmpi,tmpj));
                         //std::cout<<qRed(tmpDst.pixel(tmpi,tmpj))<<std::endl;

                         //finding closest red pixel
                         bool UpPixelIsRed = false;
                         int redU = qRed(tmpDst.pixel(tmpi,tmpj-1));
                         int blueU = qBlue(tmpDst.pixel(tmpi,tmpj-1));
                         int greenU = qGreen(tmpDst.pixel(tmpi,tmpj-1));
                         if(redU==255 && blueU==0 && greenU==0){
                             UpPixelIsRed=true;
                         }

                         bool RightPixelIsRed = false;
                         int redR = qRed(tmpDst.pixel(tmpi+1,tmpj));
                         int blueR = qBlue(tmpDst.pixel(tmpi+1,tmpj));
                         int greenR = qGreen(tmpDst.pixel(tmpi+1,tmpj));
                         if(redR==255 && blueR==0 && greenR==0){
                             RightPixelIsRed=true;
                         }

                         bool DownPixelIsRed = false;
                         int redD = qRed(tmpDst.pixel(tmpi,tmpj+1));
                         int blueD = qBlue(tmpDst.pixel(tmpi,tmpj+1));
                         int greenD = qGreen(tmpDst.pixel(tmpi,tmpj+1));
                         if(redD==255 && blueD==0 && greenD==0){
                             DownPixelIsRed=true;
                         }

                         bool LeftPixelIsRed = false;
                         int redL = qRed(tmpDst.pixel(tmpi-1,tmpj));
                         int blueL = qBlue(tmpDst.pixel(tmpi-1,tmpj));
                         int greenL = qGreen(tmpDst.pixel(tmpi-1,tmpj));
                         if(redL==255 && blueL==0 && greenL==0){
                             LeftPixelIsRed=true;
                         }

                         bool UpRightPixelIsRed = false;
                         int redUR = qRed(tmpDst.pixel(tmpi+1,tmpj-1));
                         int blueUR = qBlue(tmpDst.pixel(tmpi+1,tmpj-1));
                         int greenUR = qGreen(tmpDst.pixel(tmpi+1,tmpj-1));
                         if(redUR==255 && blueUR==0 && greenUR==0){
                             UpRightPixelIsRed=true;
                         }

                         bool UpLeftPixelIsRed = false;
                         int redUL = qRed(tmpDst.pixel(tmpi-1,tmpj-1));
                         int blueUL = qBlue(tmpDst.pixel(tmpi-1,tmpj-1));
                         int greenUL = qGreen(tmpDst.pixel(tmpi-1,tmpj-1));
                         if(redUL==255 && blueUL==0 && greenUL==0){
                             UpLeftPixelIsRed=true;
                         }

                         bool DownRightPixelIsRed = false;
                         int redDR = qRed(tmpDst.pixel(tmpi+1,tmpj+1));
                         int blueDR = qBlue(tmpDst.pixel(tmpi+1,tmpj+1));
                         int greenDR = qGreen(tmpDst.pixel(tmpi+1,tmpj+1));
                         if(redDR==255 && blueDR==0 && greenDR==0){
                             DownRightPixelIsRed=true;
                         }

                         bool DownLeftPixelIsRed = false;
                         int redDL = qRed(tmpDst.pixel(tmpi-1,tmpj+1));
                         int blueDL = qBlue(tmpDst.pixel(tmpi-1,tmpj+1));
                         int greenDL = qGreen(tmpDst.pixel(tmpi-1,tmpj+1));
                         if(redDL==255 && blueDL==0 && greenDL==0){
                             DownLeftPixelIsRed=true;
                         }


                         for(int i=0;i<1;i++){


                         if(RightPixelIsRed==true){tmpi=tmpi+1;tmpj=tmpj;break;}
                         if(UpPixelIsRed==true){tmpi=tmpi;tmpj=tmpj-1; break;}
                         if(UpRightPixelIsRed==true){tmpi=tmpi+1;tmpj=tmpj-1;break;}
                         if(LeftPixelIsRed==true){tmpi=tmpi-1;tmpj=tmpj;break;}
                         if(UpLeftPixelIsRed==true){tmpi=tmpi-1;tmpj=tmpj-1;break;}
                         if(DownRightPixelIsRed==true){tmpi=tmpi+1;tmpj=tmpj+1;break;}
                         if(DownPixelIsRed==true){tmpi=tmpi;tmpj=tmpj+1;break;}
                         if(DownLeftPixelIsRed==true){tmpi=tmpi-1;tmpj=tmpj+1;break;}

                        }

                            //if route is finished then exit cycle
                         if(UpPixelIsRed==false && RightPixelIsRed==false && DownPixelIsRed==false && LeftPixelIsRed==false &&
                             UpRightPixelIsRed==false && UpLeftPixelIsRed==false && DownLeftPixelIsRed==false && DownRightPixelIsRed==false){
                             CountourTraceUnfinished=false;
                         }

                    }

                    if(c.getSize()!=1){
                            countours.push_back(c);
                            //here goes some logs
                            std::cout<<"vyvod "<<std::endl;
                            std::cout<<"count of countorus - "<<countours.size()<<std::endl;
                    } else {tmpDst.setPixel(c.getCountourBeginning().first,c.getCountourBeginning().second,qRgb(0,0,0));}
                }
            }
        }
    //gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

    }

    void MainWindow::RemoveAreas()
    {

        QImage tmpSrc = gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        for(int i=0;i<objects.size();i++){
            if(objects[i].getSquare()==0){

                // paint area black
                int tmpx = objects[i].getCountour().getCountourBeginning().first;
                int tmpy = objects[i].getCountour().getCountourBeginning().second;

                FindClosestWhitePixel(tmpDst,tmpx,tmpy);
                //FillAreaBlack(tmpDst, tmpx, tmpy, objects[i]);

                //paint countour black
                for(int j=0;j<objects[i].getCountour().getSize();j++){
                    tmpDst.setPixel(objects[i].getCountour().getCountourPixel(j).first,objects[i].getCountour().getCountourPixel(j).second, qRgb(0,0,0));
                }
            }
        }

        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

    }

    void MainWindow::PaintCountours()
    {


            QImage tmpSrc =gpiDst->pixmap().toImage();
            QImage tmpDst(tmpSrc);
    //Painting borders

          for (int i=1;i<tmpSrc.width()-1;i++){
              for (int j=1;j<tmpSrc.height()-1;j++){


                  //Block for checking if surrounding pixels is white

                  int red = qRed(tmpSrc.pixel(i,j));
                  int blue = qBlue(tmpSrc.pixel(i,j));
                  int green = qGreen(tmpSrc.pixel(i,j));


                  bool UpPixelIsWhite = false;
                  int redU = qRed(tmpSrc.pixel(i,j-1));
                  int blueU = qBlue(tmpSrc.pixel(i,j-1));
                  int greenU = qGreen(tmpSrc.pixel(i,j-1));
                  if(redU==255 && blueU==255 && greenU==255){
                      UpPixelIsWhite=true;
                  }

                  bool RightPixelIsWhite = false;
                  int redR = qRed(tmpSrc.pixel(i+1,j));
                  int blueR = qBlue(tmpSrc.pixel(i+1,j));
                  int greenR = qGreen(tmpSrc.pixel(i+1,j));
                  if(redR==255 && blueR==255 && greenR==255){
                      RightPixelIsWhite=true;
                  }

                  bool DownPixelIsWhite = false;
                  int redD = qRed(tmpSrc.pixel(i,j+1));
                  int blueD = qBlue(tmpSrc.pixel(i,j+1));
                  int greenD = qGreen(tmpSrc.pixel(i,j+1));
                  if(redD==255 && blueD==255 && greenD==255){
                      DownPixelIsWhite=true;
                  }

                  bool LeftPixelIsWhite = false;
                  int redL = qRed(tmpSrc.pixel(i-1,j));
                  int blueL = qBlue(tmpSrc.pixel(i-1,j));
                  int greenL = qGreen(tmpSrc.pixel(i-1,j));
                  if(redL==255 && blueL==255 && greenL==255){
                      LeftPixelIsWhite=true;
                  }

                  bool UpRightPixelIsWhite = false;
                  int redUR = qRed(tmpSrc.pixel(i+1,j-1));
                  int blueUR = qBlue(tmpSrc.pixel(i+1,j-1));
                  int greenUR = qGreen(tmpSrc.pixel(i+1,j-1));
                  if(redUR==255 && blueUR==255 && greenUR==255){
                      UpRightPixelIsWhite=true;
                  }

                  bool UpLeftPixelIsWhite = false;
                  int redUL = qRed(tmpSrc.pixel(i-1,j-1));
                  int blueUL = qBlue(tmpSrc.pixel(i-1,j-1));
                  int greenUL = qGreen(tmpSrc.pixel(i-1,j-1));
                  if(redUL==255 && blueUL==255 && greenUL==255){
                      UpLeftPixelIsWhite=true;
                  }

                  bool DownRightPixelIsWhite = false;
                  int redDR = qRed(tmpSrc.pixel(i+1,j+1));
                  int blueDR = qBlue(tmpSrc.pixel(i+1,j+1));
                  int greenDR = qGreen(tmpSrc.pixel(i+1,j+1));
                  if(redDR==255 && blueDR==255 && greenDR==255){
                      DownRightPixelIsWhite=true;
                  }

                  bool DownLeftPixelIsWhite = false;
                  int redDL = qRed(tmpSrc.pixel(i-1,j+1));
                  int blueDL = qBlue(tmpSrc.pixel(i-1,j+1));
                  int greenDL = qGreen(tmpSrc.pixel(i-1,j+1));
                  if(redDL==255 && blueDL==255 && greenDL==255){
                      DownLeftPixelIsWhite=true;
                  }

                  // Paint borders with red

                  if ((red==0 && blue==0 && green==0) && (UpPixelIsWhite==true || RightPixelIsWhite==true || DownPixelIsWhite==true || LeftPixelIsWhite==true ||
                          UpRightPixelIsWhite==true || UpLeftPixelIsWhite==true || DownLeftPixelIsWhite==true || DownRightPixelIsWhite==true)){
                      tmpDst.setPixel(i,j,qRgb(255,0,0));
                  }
              }
          }
      gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::FillAreaInArea(QImage& img, int x, int y, ImgObject& object)
    {

        int red = qRed(img.pixel(x,y));
        int blue = qBlue(img.pixel(x,y));
        int green = qGreen(img.pixel(x,y));

        if (qRgb(qRed(img.pixel(x,y)),qGreen(img.pixel(x,y)),qBlue(img.pixel(x,y))) == qRgb(255,0,0) || qRgb(qRed(img.pixel(x,y)),qGreen(img.pixel(x,y)),qBlue(img.pixel(x,y))) == qRgb(255,255,255)) //if red or white
        {
            img.setPixel(x,y,qRgb(0,255,0));   //then paint
            object.PushValue(x,y);
            //std::cout<<"pixel "<<x<<":"<<y<<" painted green"<<endl;
            FillAreaInArea (img, x, y-1, object);
            FillAreaInArea (img, x+1, y, object); //check other pixels
            FillAreaInArea (img, x, y+1, object);
            FillAreaInArea (img, x-1, y, object);
        }
    }

    //have to recheck dat
    void FillArea (QImage& img, int x, int y, ImgObject& object)
    {

        if (qRgb(qRed(img.pixel(x,y)),qGreen(img.pixel(x,y)),qBlue(img.pixel(x,y)))!= qRgb(255,0,0) && qRgb(qRed(img.pixel(x,y)),qGreen(img.pixel(x,y)),qBlue(img.pixel(x,y)))!= qRgb(0,255,0)) //if not red or green
        {
            img.setPixel(x,y,qRgb(0,255,0));   //then paint
            object.PushValue(x,y);
            //std::cout<<"pixel "<<x<<":"<<y<<" painted green"<<endl;
            FillArea (img, x, y-1, object);
            FillArea (img, x+1, y, object); //check other pixels
            FillArea (img, x, y+1, object);
            FillArea (img, x-1, y, object);
        }

    }

    void MainWindow::FindClosestWhitePixel(QImage img, int& x, int& y){

        bool UpPixelIsWhite = false;
        int redU = qRed(img.pixel(x,y-1));
        int blueU = qBlue(img.pixel(x,y-1));
        int greenU = qGreen(img.pixel(x,y-1));
        if(redU==255 && blueU==255 && greenU==255){
            UpPixelIsWhite=true;
        }

        bool RightPixelIsWhite = false;
        int redR = qRed(img.pixel(x+1,y));
        int blueR = qBlue(img.pixel(x+1,y));
        int greenR = qGreen(img.pixel(x+1,y));
        if(redR==255 && blueR==255 && greenR==255){
            RightPixelIsWhite=true;
        }

        bool DownPixelIsWhite = false;
        int redD = qRed(img.pixel(x,y+1));
        int blueD = qBlue(img.pixel(x,y+1));
        int greenD = qGreen(img.pixel(x,y+1));
        if(redD==255 && blueD==255 && greenD==255){
            DownPixelIsWhite=true;
        }

        bool LeftPixelIsWhite = false;
        int redL = qRed(img.pixel(x-1,y));
        int blueL = qBlue(img.pixel(x-1,y));
        int greenL = qGreen(img.pixel(x-1,y));
        if(redL==255 && blueL==255 && greenL==255){
            LeftPixelIsWhite=true;
        }

        bool UpRightPixelIsWhite = false;
        int redUR = qRed(img.pixel(x+1,y-1));
        int blueUR = qBlue(img.pixel(x+1,y-1));
        int greenUR = qGreen(img.pixel(x+1,y-1));
        if(redUR==255 && blueUR==255 && greenUR==255){
            UpRightPixelIsWhite=true;
        }

        bool UpLeftPixelIsWhite = false;
        int redUL = qRed(img.pixel(x-1,y-1));
        int blueUL = qBlue(img.pixel(x-1,y-1));
        int greenUL = qGreen(img.pixel(x-1,y-1));
        if(redUL==255 && blueUL==255 && greenUL==255){
            UpLeftPixelIsWhite=true;
        }

        bool DownRightPixelIsWhite = false;
        int redDR = qRed(img.pixel(x+1,y+1));
        int blueDR = qBlue(img.pixel(x+1,y+1));
        int greenDR = qGreen(img.pixel(x+1,y+1));
        if(redDR==255 && blueDR==255 && greenDR==255){
            DownRightPixelIsWhite=true;
        }

        bool DownLeftPixelIsWhite = false;
        int redDL = qRed(img.pixel(x-1,y+1));
        int blueDL = qBlue(img.pixel(x-1,y+1));
        int greenDL = qGreen(img.pixel(x-1,y+1));
        if(redDL==255 && blueDL==255 && greenDL==255){
            DownLeftPixelIsWhite=true;
        }
        if(UpPixelIsWhite==true){x=x;y+y+1;return;}
        if(RightPixelIsWhite==true){x=x+1;y=y;return;}
        if(DownPixelIsWhite==true){x=x;y=y-1;return;}
        if(LeftPixelIsWhite==true){x=x-1;y=y;return;}
        if(UpRightPixelIsWhite==true){x=x+1;y=y-1;return;}
        if(UpLeftPixelIsWhite==true){x=x-1;y=y-1;return;}
        if(DownRightPixelIsWhite==true){x=x+1;y=y+1;return;}
        if(DownLeftPixelIsWhite==true){x=x-1;y=y+1;return;}
    }

    void MainWindow::Purify()
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

    //Painting borders

      for (int i=1;i<tmpDst.width()-1;i++){
          for (int j=1;j<tmpDst.height()-1;j++){


              int red = qRed(tmpDst.pixel(i,j));
              int blue = qBlue(tmpDst.pixel(i,j));
              int green = qGreen(tmpDst.pixel(i,j));

              bool WhiteFlag=CheckNearbyPixels(255,255,255,i,j);
              bool BlackFlag=CheckNearbyPixels(0,0,0,i,j);
              bool RedFlag=CheckNearbyPixels(255,0,0,i,j);

                if ((red==255 && green==0 & blue==0) && (RedFlag==false || BlackFlag==false || WhiteFlag==false)){
                    tmpDst.setPixel(i,j,qRgb(255,255,255));
                    WhiteFlag=false;
                    BlackFlag=false;
                    RedFlag=false;
                }

          }
      }
      gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

    }

    bool MainWindow::CheckNearbyPixels(int red, int green, int blue, int i, int j)
    {

        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        bool UpPixelIsColor = false;
        int redU = qRed(tmpDst.pixel(i,j+1));
        int blueU = qBlue(tmpDst.pixel(i,j+1));
        int greenU = qGreen(tmpDst.pixel(i,j+1));
        if(redU==red && blueU==blue && greenU==green){
            UpPixelIsColor=true;
        }

        bool RightPixelIsColor = false;
        int redR = qRed(tmpDst.pixel(i+1,j));
        int blueR = qBlue(tmpDst.pixel(i+1,j));
        int greenR = qGreen(tmpDst.pixel(i+1,j));
        if(redR==red && blueR==blue && greenR==green){
            RightPixelIsColor=true;
        }

        bool DownPixelIsColor = false;
        int redD = qRed(tmpDst.pixel(i,j-1));
        int blueD = qBlue(tmpDst.pixel(i,j-1));
        int greenD = qGreen(tmpDst.pixel(i,j-1));
        if(redD==red && blueD==blue && greenD==green){
            DownPixelIsColor=true;
        }

        bool LeftPixelIsColor = false;
        int redL = qRed(tmpDst.pixel(i-1,j));
        int blueL = qBlue(tmpDst.pixel(i-1,j));
        int greenL = qGreen(tmpDst.pixel(i-1,j));
        if(redL==red && blueL==blue && greenL==green){
            LeftPixelIsColor=true;
        }

        bool UpRightPixelIsColor = false;
        int redUR = qRed(tmpDst.pixel(i+1,j+1));
        int blueUR = qBlue(tmpDst.pixel(i+1,j+1));
        int greenUR = qGreen(tmpDst.pixel(i+1,j+1));
        if(redUR==red && blueUR==blue && greenUR==green){
            UpRightPixelIsColor=true;
        }

        bool UpLeftPixelIsColor = false;
        int redUL = qRed(tmpDst.pixel(i-1,j+1));
        int blueUL = qBlue(tmpDst.pixel(i-1,j+1));
        int greenUL = qGreen(tmpDst.pixel(i-1,j+1));
        if(redUL==red && blueUL==blue && greenUL==green){
            UpLeftPixelIsColor=true;
        }

        bool DownRightPixelIsColor = false;
        int redDR = qRed(tmpDst.pixel(i+1,j-1));
        int blueDR = qBlue(tmpDst.pixel(i+1,j-1));
        int greenDR = qGreen(tmpDst.pixel(i+1,j-1));
        if(redDR==red && blueDR==blue && greenDR==green){
            DownRightPixelIsColor=true;
        }

        bool DownLeftPixelIsColor = false;
        int redDL = qRed(tmpDst.pixel(i-1,j-1));
        int blueDL = qBlue(tmpDst.pixel(i-1,j-1));
        int greenDL = qGreen(tmpDst.pixel(i-1,j-1));
        if(redDL==red && blueDL==blue && greenDL==green){
            DownLeftPixelIsColor=true;
        }


        if(UpPixelIsColor==true){return UpPixelIsColor;}
        if(RightPixelIsColor==true){return RightPixelIsColor;}
        if(DownPixelIsColor==true){return DownPixelIsColor;}
        if(LeftPixelIsColor==true){return LeftPixelIsColor;}
        if(UpRightPixelIsColor==true){return UpRightPixelIsColor;}
        if(UpLeftPixelIsColor==true){return UpLeftPixelIsColor;}
        if(DownRightPixelIsColor==true){return DownRightPixelIsColor;}
        if(DownLeftPixelIsColor==true){return DownLeftPixelIsColor;}

    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

    }

    IplImage * MainWindow::QImage2IplImage(const QImage *image)
    {
        IplImage * cvImage = cvCreateImageHeader(cvSize(image->width(),
          image->height()), IPL_DEPTH_8U, 4);

         cvImage->imageData = (char*)image->bits();
         IplImage* colorImage = cvCreateImage(cvGetSize(cvImage), 8, 3);
         cvConvertImage(cvImage, colorImage, 0);
         return colorImage;
    }

    void findClosestBlackPixel(QImage img, int& x, int& y){
      /*  bool UpPixelIsBlack = false;
        int redU = qRed(img.pixel(x,y-1));
        int blueU = qBlue(img.pixel(x,y-1));
        int greenU = qGreen(img.pixel(x,y-1));
        if(redU==0 && blueU==0 && greenU==0){
            UpPixelIsBlack=true;
        } */

        bool RightPixelIsBlack = false;
        int redR = qRed(img.pixel(x+1,y));
        int blueR = qBlue(img.pixel(x+1,y));
        int greenR = qGreen(img.pixel(x+1,y));
        if(redR==0 && blueR==0 && greenR==0){
            RightPixelIsBlack=true;
        }

      /*  bool DownPixelIsBlack = false;
        int redD = qRed(img.pixel(x,y+1));
        int blueD = qBlue(img.pixel(x,y+1));
        int greenD = qGreen(img.pixel(x,y+1));
        if(redD==0 && blueD==0 && greenD==0){
            DownPixelIsBlack=true;
        }

        bool LeftPixelIsBlack = false;
        int redL = qRed(img.pixel(x-1,y));
        int blueL = qBlue(img.pixel(x-1,y));
        int greenL = qGreen(img.pixel(x-1,y));
        if(redL==0 && blueL==0 && greenL==0){
            LeftPixelIsBlack=true;
        }

        bool UpRightPixelIsBlack = false;
        int redUR = qRed(img.pixel(x+1,y-1));
        int blueUR = qBlue(img.pixel(x+1,y-1));
        int greenUR = qGreen(img.pixel(x+1,y-1));
        if(redUR==0 && blueUR==0 && greenUR==0){
            UpRightPixelIsBlack=true;
        }

        bool UpLeftPixelIsBlack = false;
        int redUL = qRed(img.pixel(x-1,y-1));
        int blueUL = qBlue(img.pixel(x-1,y-1));
        int greenUL = qGreen(img.pixel(x-1,y-1));
        if(redUL==0 && blueUL==0 && greenUL==0){
            UpLeftPixelIsBlack=true;
        } */

        bool DownRightPixelIsBlack = false;
        int redDR = qRed(img.pixel(x+1,y+1));
        int blueDR = qBlue(img.pixel(x+1,y+1));
        int greenDR = qGreen(img.pixel(x+1,y+1));
        if(redDR==0 && blueDR==0 && greenDR==0){
            DownRightPixelIsBlack=true;
        }

        /*
        bool DownLeftPixelIsBlack = false;
        int redDL = qRed(img.pixel(x-1,y+1));
        int blueDL = qBlue(img.pixel(x-1,y+1));
        int greenDL = qGreen(img.pixel(x-1,y+1));
        if(redDL==0 && blueDL==0 && greenDL==0){
            DownLeftPixelIsBlack=true;
        }  */

       // if(UpPixelIsBlack==true){x=x;y+y+1;}
        if(RightPixelIsBlack==true){x=x+1;y=y;}
        //if(DownPixelIsBlack==true){x=x;y=y-1;}
        //if(LeftPixelIsBlack==true){x=x-1;y=y;}
        //if(UpRightPixelIsBlack==true){x=x+1;y=y-1;};
        //if(UpLeftPixelIsBlack==true){x=x-1;y=y-1;};
        if(DownRightPixelIsBlack==true){x=x+1;y=y+1;};
        //if(DownLeftPixelIsBlack==true){x=x-1;y=y+1;};


    }

    void MainWindow::ObjectDat()
    {

        QImage tmpSrc = gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        // Set countours for objects
        for(int i=0;i<countours.size();i++){
            ImgObject imgobj;
            objects.push_back(imgobj); // creating objects
            objects[i].setCountour(countours[i]);
        }

        // Fill objects with green and write thier pixels to objects
        // we wont work with countours anymore

        //object by object
        for(int i=0;i<countours.size();i++){

              int tmpx = objects[i].getCountour().getCountourBeginning().first;
              int tmpy = objects[i].getCountour().getCountourBeginning().second;

              int checkx= tmpx;
              int checky= tmpy;

               findClosestBlackPixel(tmpDst, tmpx,tmpy);
               //проверка на полость

               if(checkx==tmpx && checky==tmpy){
                    //std::cout<<"Object is an area"<<endl;
                    objects[i].setIsArea(true);
                    FillAreaInArea(tmpDst,tmpx,tmpy, objects[i]);

               } else {
                   objects[i].setIsArea(false);
                   FillArea(tmpDst,tmpx,tmpy, objects[i]);
               }


               int p = objects[i].getPerimetr();
               std::cout<<"perimetr: "<<p<<std::endl;
               int s = objects[i].getSquare();
               std::cout<<"Square: "<<s<<std::endl;
               std::cout<<"Polost'? - "<<objects[i].getIsArea()<<std::endl;
        }




    //gpiDst->setPixmap(QPixmap::fromImage(tmpDst));


    }

    void MainWindow::PaintBorders()
    {
        QImage tmpSrc = gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        int tmpy = tmpSrc.height();
        int tmpx = tmpSrc.width();



    for(int j=0;j<tmpDst.width()/100;j++){


        for (int i=j; i<tmpSrc.width(); i++){
            tmpDst.setPixel(i,j,qRgb(255,255,255));
        }

        for (int i=j; i<tmpSrc.width(); i++){
            tmpDst.setPixel(i,tmpSrc.height()-j-1,qRgb(255,255,255));
        }

        for (int i=j; i<tmpSrc.height(); i++){
            tmpDst.setPixel(j,i,qRgb(255,255,255));
        }

        for (int i=j; i<tmpSrc.height(); i++){
            tmpDst.setPixel(tmpSrc.width()-1-j,i,qRgb(255,255,255));
        }
    }

        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_pushButton_22_clicked()
    {
        ui->srcGrView->scale(1.1,1.1);
        ui->dstGrView->scale(1.1,1.1);
    }

    void MainWindow::on_pushButton_23_clicked()
    {
        ui->srcGrView->scale(1/1.1,1/1.1);
        ui->dstGrView->scale(1/1.1,1/1.1);
    }

    void MainWindow::on_pushButton_24_clicked()
    {
        PaintBorders();
        PaintCountours();
        Purify();
        StoreCountours();
        //std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        ObjectDat();
    }

    void MainWindow::on_pushButton_25_clicked()
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);
        QString after;
        QSet <QString> colorh;
        int schn=0;

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
                //  cout<<"="<<ng<<endl;
                  int red = qRed(tmpDst.pixel(i,j));
                  int blue = qBlue(tmpDst.pixel(i,j));
                  int green = qGreen(tmpDst.pixel(i,j));

                  cout<<"Red"<<red<<endl<<"Blue"<<blue<<endl<<"Green"<<green<<endl;
                  //cout << "Содержимое set: ";
                  if (red<=blue && green<=blue )
                  {
                      schn++;
                      tmpDst.setPixel(i,j,qRgb(255,0,0));
                  }

                  //copy(colorh.begin(), colorh.end(), ostream_iterator<QString>(cout));
                  //for_each(colorh.begin(),colorh.end(),[](const auto& t){/*cout<<t.toStdString().c_str()<<' ';*/});
                  //for_each(colorh.begin(),colorh.end(),[](const auto& t){if(t=="White"|| t=="Red"||t="Korichevyi"||t=="blue"||t=="black"||t=="s_grey") {Svet++;}});

                  Svet = svetu.size();
                  gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

                  }
            }

    }

    void MainWindow::on_pushButton_26_clicked()
    {

        int schS=0,schN=0,SchSb=0,SchNb=0;
         gtr = new anizo;
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);
        //connect(sliderA, SIGNAL(valueChanged(int)), lcdAL, SLOT(display(int)));
        float znachsim = ui->lineEdit_11-> text().toFloat();
        cout<<znachsim<<endl;
        QSize soi=tmpDst.size();

        int height = soi.height();
        int width = soi.width();

        for(int i=0;i<tmpSrc.width();i++)
        {
            for (int j=0;j<tmpSrc.height()/2;j++)
            {
                 QRgb color =tmpSrc.pixel( i, j );
                 QColor col =QColor(color);

                 QString ng = col.name();
                 QString nb = QColor(color).name();

                 nb.remove(0,1);
                 int d = nb.toInt(nullptr,16);
                 QRgb color1 =tmpSrc.pixel( i, tmpSrc.height()-j );
                 QColor col1 =QColor(color1);

                 QString ng1 = col1.name();
                 QString nb1 = QColor(color1).name();

                 nb1.remove(0,1);

                int d1 = nb1.toInt(nullptr,16);

                if(d==d1 && d1!=16777215 && d!=16777215)
                {
                    schS++;
                }
                else {
                    SchSb++;
                }

        }
     }
        for(int i=0;i<tmpSrc.width()/2;i++)
        {
            for (int j=0;j<tmpSrc.height();j++)
            {
                 QRgb color =tmpSrc.pixel( i,j );
                 QColor col =QColor(color);

                 QString ng = col.name();
                 QString nb = QColor(color).name();

                 nb.remove(0,1);
                 int d = nb.toInt(nullptr,16);
                 QRgb color1 =tmpSrc.pixel(  tmpSrc.width()-i,j );
                 QColor col1 =QColor(color1);

                 QString ng1 = col1.name();
                 QString nb1 = QColor(color1).name();

                 nb1.remove(0,1);

                int d1 = nb1.toInt(nullptr,16);

                if(d==d1 && d1!=16777215 && d!=16777215)
                {
                    schN++;
                }
                else {
                    SchNb++;
                }

        }
        }
        uint KolvoToch = tmpSrc.height()*tmpSrc.width();
        uint JKol = schN+schS;
        ui->textBrowser->append("вывод количества нужных точек = " + QString::number(JKol));
        ui->textBrowser->append("Сумма по икс = " + QString::number(schS) + " Сумма по Икс белых = " + QString::number(SchSb));
        ui->textBrowser->append("Сумма по игрек = " + QString::number (schN) + " Сумма по Игрек белых = " + QString::number(SchNb));
        uint KolvoTochX= KolvoToch/2 - SchNb;
        uint KolvoTochY= KolvoToch/2 - SchSb;
        uint KolvoTochS= KolvoTochX+KolvoTochY;
        float Simmx=(float) schS/KolvoTochX;
        if (Simmx>1)
        {
            Simmx = (float)KolvoTochX/schS;
        }
        if(Simmx<znachsim)
        {
            ui->textBrowser->append("Ассиметрия по оси х");
            ui->textBrowser->append("Ассиметрия по икс = " + QString::number(Simmx));
            ui->textBrowser->append("Значение симметрии = " + QString::number(znachsim));
            gtr->Vivod1st(Simmx,znachsim);
        }
        else {
            ui->textBrowser->append("Симметрия по оси х");
            ui->textBrowser->append("Симметрия по икс = " + QString::number(Simmx));
            ui->textBrowser->append("Значение симметрии = " + QString::number(znachsim));
            gtr->Vivod1stS(Simmx,znachsim);
        }
        float Simmy=(float) schN/KolvoTochY;
        if (Simmy>1)
        {
            Simmy = (float)KolvoTochY/schN;
        }
        if(Simmy<znachsim)
        {
            ui->textBrowser->append("Ассиметрия по у");
            ui->textBrowser->append("Сумма значение по у = " + QString::number(Simmy));
            ui->textBrowser->append("Значение симметрии = " + QString::number(znachsim));
            ui->textBrowser->append("Вывод данных по расчету SchN = " + QString::number(schN) + "Количество точек У = " + QString::number(KolvoTochY));
            gtr->Vivod2nd(Simmy,znachsim);
        }
        else {
            ui->textBrowser->append("Симметрия по оси у");
            ui->textBrowser->append("Сумма значений по у = "+ QString::number(Simmy));
            ui->textBrowser->append("Значение симметрии = " +QString::number(znachsim) );
             ui->textBrowser->append("Вывод данных по расчету SchN = " + QString::number(schN) + "Количество точек У = " + QString::number(KolvoTochY));
            gtr->Vivod2ndS(Simmy,znachsim);
        }
        float Srsim = (Simmy+Simmx)/2;
        if(Srsim<0.9)
        {
            ui->textBrowser->append("Ассиметрия");
            ui->textBrowser->append("Сумма Совпадений = " + QString::number(Srsim));
            ui->textBrowser->append("Количество точек = " + QString::number(KolvoTochS));
            gtr->Vivod3ed(Srsim,KolvoToch);
             f1=JKol; f2=KolvoToch;
        }
        else
        {
            f1=JKol; f2=KolvoToch;
            ui->textBrowser->append("Симметрия");
            ui->textBrowser->append("Сумма Совпадений = " + QString::number(Srsim));
            ui->textBrowser->append("Количесство точек = " + QString::number(KolvoToch));
            gtr->Vivod3edS(Srsim,KolvoToch);
        }
    }

    void MainWindow::on_pushButton_27_clicked()
    {
        int schS=0,schN=0;
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);
        //connect(sliderA, SIGNAL(valueChanged(int)), lcdAL, SLOT(display(int)));

        QSize soi=tmpDst.size();

        int height = soi.height();
        int width = soi.width();



        for(int i=0;i<tmpSrc.width();i++)
        {
            for (int j=0;j<tmpSrc.height()/2;j++)
            {
                 QRgb color =tmpSrc.pixel( i, j );
                 QColor col =QColor(color);

                 QString ng = col.name();
                 QString nb = QColor(color).name();

                 nb.remove(0,1);
                 int d = nb.toInt(nullptr,16);
                 QRgb color1 =tmpSrc.pixel( i, tmpSrc.height()-j );
                 QColor col1 =QColor(color1);

                 QString ng1 = col1.name();
                 QString nb1 = QColor(color1).name();

                 nb1.remove(0,1);

                int d1 = nb1.toInt(nullptr,16);

                if(d==d1)
                {
                    schS++;
                }
                else {
                       tmpDst.setPixel(i,j,qRgb(255,0,0));
                       tmpDst.setPixel( i, tmpSrc.height()-j ,qRgb(255,0,0));
                }

        }
     }
        for(int i=0;i<tmpSrc.height();i++)
        {
            for (int j=0;j<tmpSrc.width()/2;j++)
            {
                 QRgb color =tmpSrc.pixel( j,i );
                 QColor col =QColor(color);

                 QString ng = col.name();
                 QString nb = QColor(color).name();

                 nb.remove(0,1);
                 int d = nb.toInt(nullptr,16);
                 QRgb color1 =tmpSrc.pixel(  tmpSrc.width()-j,i );
                 QColor col1 =QColor(color1);

                 QString ng1 = col1.name();
                 QString nb1 = QColor(color1).name();

                 nb1.remove(0,1);

                int d1 = nb1.toInt(nullptr,16);

                if(d==d1)
                {
                    schN++;
                }
                else {
                    tmpDst.setPixel(j,i,qRgb(255,0,0));
                    tmpDst.setPixel(tmpSrc.width()-j,i,qRgb(255,0,0));
                }

        }
        }
            gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_pushButton_28_clicked()
    {
        QImage tmpSrc =gpiSrc->pixmap().toImage();
            QImage tmpDst(tmpSrc);


            for (int i=1;i<tmpSrc.width()-2;i++){
                for (int j=1;j<tmpSrc.height()-2;j++){

                    QColor color = tmpSrc.pixelColor(i,j);
                    int Yar= color.lightness();
                    int Hue= color.hslHue();
                    int Sat= color.hslSaturation();

                    cout<< Yar<<endl;

                    bool LYar11=false;
                    QColor color11 = tmpSrc.pixelColor(i-1,j-1);
                    int Yar11=color11.lightness();
                    int Hue11=color11.hslHue();
                    int Sat11=color11.hslSaturation();
                    if (Yar<Yar11 && Hue<Hue11 && Sat<Sat11){
                        LYar11=true;
                    }
                    cout<<"Yar11 " <<Yar11<<endl;
                    bool LYar10=false;
                    QColor color10 = tmpSrc.pixelColor(i-1,j);
                    int Yar10=color10.lightness();
                    int Hue10=color10.hslHue();
                    int Sat10=color10.hslSaturation();
                    if (Yar<Yar10 && Hue<Hue10 && Sat<Sat10){
                        LYar10=true;
                    }
                    cout<<"Yar10 " <<Yar10<<endl;
                    bool LYar01=false;
                    QColor color01 = tmpSrc.pixelColor(i-1,j+1);
                    int Yar01=color01.lightness();
                    int Hue01=color01.hslHue();
                    int Sat01=color01.hslSaturation();
                    if (Yar<Yar01 && Hue<Hue01 && Sat<Sat01){
                        LYar01=true;
                    }
                    cout<<"Yar01 " <<Yar01<<endl;
                    bool LYar1=false;
                    QColor color1 = tmpSrc.pixelColor(i-1,j);
                    int Yar1=color1.lightness();
                    int Hue1=color1.hslHue();
                    int Sat1=color1.hslSaturation();
                    if (Yar<Yar1 && Hue<Hue1 && Sat<Sat1){
                        LYar1=true;
                    }
                      cout<<"Yar1 " <<Yar1<<endl;
                    bool LYar2=false;
                    QColor color2 = tmpSrc.pixelColor(i+1,j);
                    int Yar2=color2.lightness();
                    int Hue2=color2.hslHue();
                    int Sat2=color2.hslSaturation();
                    if (Yar<Yar2 && Hue<Hue2 && Sat<Sat2){
                        LYar2=true;
                    }
                      cout<<"Yar2 " <<Yar2<<endl;
                    bool PYar11=false;
                    QColor pcolor11 = tmpSrc.pixelColor(i+1,j-1);
                    int pYar11=pcolor11.lightness();
                    int pHue11=pcolor11.hslHue();
                    int pSat11=pcolor11.hslSaturation();
                    if (Yar<pYar11 && Hue<pHue11 && Sat<pSat11){
                        PYar11=true;
                    }
                      cout<<"pYar11 " <<pYar11<<endl;
                    bool PYar01=false;
                    QColor pcolor01 = tmpSrc.pixelColor(i+1,j);
                    int pYar01=pcolor01.lightness();
                    int pHue01=pcolor01.hslHue();
                    int pSat01=pcolor01.hslSaturation();
                    if (Yar<pYar01 && Hue<pHue01 && Sat<pSat01){
                        PYar01=true;
                    }
                      cout<<"pYar01 " <<pYar01<<endl;
                    bool PYar10=false;
                    QColor pcolor10 = tmpSrc.pixelColor(i+1,j+1);
                    int pYar10=pcolor10.lightness();
                    int pHue10=pcolor10.hslHue();
                    int pSat10=pcolor10.hslSaturation();
                    if (Yar<pYar10 && Hue<pHue10 && Sat<pSat10){
                        PYar10=true;
                    }
                      cout<<"pYar10 " <<pYar10<<endl;
                    if ((LYar11==true && LYar10==true && LYar01==true && LYar1==true &&
                            LYar2==true && PYar11==true && PYar10==true && PYar01==true)){
                        tmpDst.setPixel(i,j,qRgb(0,255,0));

                }


                }

            }
            gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
     /*   QImage tmpSrc =gpiSrc->pixmap().toImage();
        QImage tmpDst(tmpSrc);
        QImage tmpDst1(tmpSrc);
        QImage tmpDst2(tmpSrc);
        QImage tmpDst3(tmpSrc);
        QImage tmpDst4(tmpSrc);
        QImage tmpDst5(tmpSrc);
        QImage tmpDst6(tmpSrc);
        QImage tmpDst7(tmpSrc);
        QImage tmpDst8(tmpSrc);

        for(int y=1; y<tmpSrc.height()-1;++y)
            for(int x=1; x<tmpSrc.width()-1;++x)
            {
                int r = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))));
                int g = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))));
                int b = std::max(0,std::min(255,qRed(tmpSrc.pixel(x,y))));
                tmpDst.setPixel(x,y,qRgb(r,g,b));
              /*  bool UpPixelIsWhite = false;
                int redU = qRed(tmpSrc.pixel(i,j+1));
                int blueU = qBlue(tmpSrc.pixel(i,j+1));
                int greenU = qGreen(tmpSrc.pixel(i,j+1));
                if(redU==0 && blueU==0 && greenU==0){
                    UpPixelIsWhite=true;
                     coordinatex.insert(i);
                     coordinatey.insert(j);
                }

                bool UpPixYar = false;
                int rl1 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int gl1 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int bl1 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                tmpDst1.setPixel(x,y,qRgb(rl1,gl1,bl1));
                if() {
                    UpPixYar=true;
                }

                bool UpsPixYar = false;
                int rl2 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int gl2 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int bl2 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                if(rl2>r && gl2>g && bl2>b) {
                    UpsPixYar=true;
                }

                bool UppPixYar = false;
                int rl3 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int gl3 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int bl3 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                if(rl3>r && gl3>g && bl3>b) {
                    UppPixYar=true;
                }

                bool UplPixYar = false;
                int rl4 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int gl4 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int bl4 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                if(rl4>r && gl4>g && bl4>b) {
                    UplPixYar=true;
                }


                bool UprPixYar = false;
                int rl5 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int gl5 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int bl5 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                if(rl5>r && gl5>g && bl5>b) {
                    UprPixYar=true;
                }

                bool UpdPixYar = false;
                int rl6 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int gl6 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int bl6 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                if(rl6>r && gl6>g && bl6>b) {
                    UpdPixYar=true;
                }
                bool UpjPixYar = false;
                int rl7 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int gl7 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int bl7 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                if(rl7>r && gl7>g && bl7>b) {
                    UpdPixYar=true;
                }
                bool UpkPixYar = false;
                int rl8 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int gl8 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                int bl8 = std::max(0,std::min(255,qRed(tmpSrc.pixel(x+1,y+1))));
                if(rl8>r && gl8>g && bl8>b) {
                    UpdPixYar=true;
                }
                if ((UpPixYar==true && UpsPixYar==true && UppPixYar==true && UplPixYar==true &&
                        UprPixYar==true && UpdPixYar==true && UpjPixYar==true && UpkPixYar==true)){
                    tmpDst.setPixel(x,y,qRgb(0,255,0));
                }
            }
        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    */
    }

    void MainWindow::on_pushButton_29_clicked()
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);


            for (int i=1;i<tmpSrc.width()-2;i++){
                for (int j=1;j<tmpSrc.height()-2;j++){

                    QColor color = tmpSrc.pixelColor(i,j);
                    int Yar= color.lightness();


                    cout<< Yar<<endl;

                    bool LYar11=false;
                    QColor color11 = tmpSrc.pixelColor(i-1,j-1);
                    int Yar11=color11.lightness();

                    if (Yar<Yar11 ){
                        LYar11=true;
                    }
                    cout<<"Yar11 " <<Yar11<<endl;
                    bool LYar10=false;
                    QColor color10 = tmpSrc.pixelColor(i-1,j);
                    int Yar10=color10.lightness();

                    if (Yar<Yar10 ){
                        LYar10=true;
                    }
                    cout<<"Yar10 " <<Yar10<<endl;
                    bool LYar01=false;
                    QColor color01 = tmpSrc.pixelColor(i-1,j+1);
                    int Yar01=color01.lightness();

                    if (Yar<Yar01 ){
                        LYar01=true;
                    }
                    cout<<"Yar01 " <<Yar01<<endl;
                    bool LYar1=false;
                    QColor color1 = tmpSrc.pixelColor(i-1,j);
                    int Yar1=color1.lightness();

                    if (Yar<Yar1 ){
                        LYar1=true;
                    }
                      cout<<"Yar1 " <<Yar1<<endl;
                    bool LYar2=false;
                    QColor color2 = tmpSrc.pixelColor(i+1,j);
                    int Yar2=color2.lightness();

                    if (Yar<Yar2 ){
                        LYar2=true;
                    }
                      cout<<"Yar2 " <<Yar2<<endl;
                    bool PYar11=false;
                    QColor pcolor11 = tmpSrc.pixelColor(i+1,j-1);
                    int pYar11=pcolor11.lightness();

                    if (Yar<pYar11){
                        PYar11=true;
                    }
                      cout<<"pYar11 " <<pYar11<<endl;
                    bool PYar01=false;
                    QColor pcolor01 = tmpSrc.pixelColor(i+1,j);
                    int pYar01=pcolor01.lightness();

                    if (Yar<pYar01 ){
                        PYar01=true;
                    }
                      cout<<"pYar01 " <<pYar01<<endl;
                    bool PYar10=false;
                    QColor pcolor10 = tmpSrc.pixelColor(i+1,j+1);
                    int pYar10=pcolor10.lightness();

                    if (Yar<pYar10 ){
                        PYar10=true;
                    }
                      cout<<"pYar10 " <<pYar10<<endl;
                    if ((LYar11==true || LYar10==true || LYar01==true || LYar1==true ||
                            LYar2==true || PYar11==true || PYar10==true || PYar01==true)){
                        tmpDst.setPixel(i,j,qRgb(0,255,0));

                }


                }

            }
            gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    using namespace cv;

    //выделяет контуры на изображении методом Canny
    void MainWindow::on_pushButton_30_clicked()
    {
        QString base_folder = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString neg_dir_path = QFileDialog::getOpenFileName(this,
                                                    tr("Выбор Изображения"),
                                                    base_folder,
                                                    tr("Таблицы (*.jpg)"),nullptr, QFileDialog::DontResolveSymlinks);
        ui->lineEdit_12->setText(neg_dir_path);
        Mat src1;
        src1 = imread(neg_dir_path.toStdString(), CV_LOAD_IMAGE_COLOR);
        if (!src1.data)
           {
               std::cout << "Image not loaded";
               exit(0);
           }
        namedWindow("Original image", CV_WINDOW_AUTOSIZE);
        imshow("Original image", src1);

       Mat gray, edge, draw;
        cvtColor(src1, gray, CV_BGR2GRAY);

       Canny(gray, edge, 50, 150, 3);

       edge.convertTo(draw, CV_8U);
        namedWindow("image", CV_WINDOW_AUTOSIZE);
        imshow("image", draw);

       waitKey(0);;
    }

    void MainWindow::on_pushButton_31_clicked()
    {
        QString base_folder_2 = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString Linefound = QFileDialog::getOpenFileName(this,
                                                    tr("Выбор Изображения"),
                                                    base_folder_2,
                                                    tr("Таблицы (*.jpg)"),nullptr, QFileDialog::DontResolveSymlinks);
        IplImage* src = 0;
               IplImage* dst=0;
               IplImage* color_dst=0;
                std::string temp(Linefound.toStdString());
               // имя картинки задаётся первым параметром

              const char* filename = temp.c_str();
               // получаем картинку (в градациях серого)
               src = cvLoadImage(filename, CV_LOAD_IMAGE_GRAYSCALE);

               if( !src ){
                       printf("[!] Error: cant load image: %s \n", filename);

               }

               printf("[i] image: %s\n", filename);


               // хранилище памяти для хранения найденных линий
               CvMemStorage* storage = cvCreateMemStorage(0);
               CvSeq* lines = 0;
               int i = 0;


               dst = cvCreateImage( cvGetSize(src), 8, 1 );
               color_dst = cvCreateImage( cvGetSize(src), 8, 3 );

               // детектирование границ
               cvCanny( src, dst, 50, 200, 3 );

               // конвертируем в цветное изображение
               cvCvtColor( dst, color_dst, CV_GRAY2BGR );

               // нахождение линий
               lines = cvHoughLines2( dst, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/180, 50, 50, 10 );

               // нарисуем найденные линии
               for( i = 0; i < lines->total; i++ ){
                       CvPoint* line = (CvPoint*)cvGetSeqElem(lines,i);
                       cvLine( color_dst, line[0], line[1], CV_RGB(255,0,0), 3, CV_AA, 0 );
               }

               // показываем
               cvNamedWindow( "Source", 1 );
               cvShowImage( "Source", src );

               cvNamedWindow( "Hough", 1 );
               cvShowImage( "Hough", color_dst );

               // ждём нажатия клавиши
               cvWaitKey(0);

               // освобождаем ресурсы
               cvReleaseMemStorage(&storage);
               cvReleaseImage(&src);
               cvReleaseImage(&dst);
               cvReleaseImage(&color_dst);
               cvDestroyAllWindows();
    }

    void MainWindow::on_pushButton_32_clicked()
    {
        QString base_folder_3 = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString neg = QFileDialog::getOpenFileName(this,
                                                    tr("Выбор Изображения"),
                                                    base_folder_3,
                                                    tr("Таблицы (*.jpg)"),nullptr, QFileDialog::DontResolveSymlinks);

        Mat image = imread(neg.toStdString(), IMREAD_GRAYSCALE);

            imshow("Source Image", image);

            //Canny(image, image, 50, 200, 3);

                bool useRefine = false;

            Ptr<LineSegmentDetector> ls = useRefine ? createLineSegmentDetector(LSD_REFINE_STD) : createLineSegmentDetector(LSD_REFINE_STD );

            double start = double(getTickCount());
            vector<Vec4f> lines_std;

            // Detect the lines
            ls->detect(image, lines_std);

            double duration_ms = (double(getTickCount()) - start) * 1000 / getTickFrequency();
            std::cout << "It took " << duration_ms << " ms." << std::endl;

            image = Scalar(0, 0, 0);

            ls->drawSegments(image, lines_std);
            namedWindow("image", CV_WINDOW_AUTOSIZE);
            imshow("image",image);
                imwrite("out.png", image);

                waitKey(0);;
    }

    IplImage* image = 0;
    IplImage* dst = 0;

    // для хранения каналов RGB
    IplImage* rgb = 0;
    IplImage* r_plane = 0;
    IplImage* g_plane = 0;
    IplImage* b_plane = 0;
    // для хранения каналов RGB после преобразования
    IplImage* r_range = 0;
    IplImage* g_range = 0;
    IplImage* b_range = 0;
    // для хранения суммарной картинки
    IplImage* rgb_and = 0;

    int Rmin = 0;
    int Rmax = 256;

    int Gmin = 0;
    int Gmax = 256;

    int Bmin = 0;
    int Bmax = 256;

    int RGBmax = 256;

    void myTrackbarRmin(int pos) {
            Rmin = pos;
            cvInRangeS(r_plane, cvScalar(Rmin), cvScalar(Rmax), r_range);
    }

    void myTrackbarRmax(int pos) {
            Rmax = pos;
            cvInRangeS(r_plane, cvScalar(Rmin), cvScalar(Rmax), r_range);
    }

    void myTrackbarGmin(int pos) {
            Gmin = pos;
            cvInRangeS(g_plane, cvScalar(Gmin), cvScalar(Gmax), g_range);
    }

    void myTrackbarGmax(int pos) {
            Gmax = pos;
            cvInRangeS(g_plane, cvScalar(Gmin), cvScalar(Gmax), g_range);
    }

    void myTrackbarBmin(int pos) {
            Bmin = pos;
            cvInRangeS(b_plane, cvScalar(Bmin), cvScalar(Bmax), b_range);
    }

    void myTrackbarBmax(int pos) {
            Bmax = pos;
            cvInRangeS(b_plane, cvScalar(Bmin), cvScalar(Bmax), b_range);
    }


    void MainWindow::on_pushButton_33_clicked()
    {
        QString base_folder_2 = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString Linefound = QFileDialog::getOpenFileName(this,
                                                    tr("Выбор Изображения"),
                                                    base_folder_2,
                                                    tr("Таблицы (*.jpg)"),nullptr, QFileDialog::DontResolveSymlinks);
        IplImage* src = 0;
               IplImage* dst=0;
               IplImage* color_dst=0;
                std::string temp(Linefound.toStdString());
               // имя картинки задаётся первым параметром

              const char* filename = temp.c_str();
             image = cvLoadImage(filename,1);

             printf("[i] image: %s\n", filename);
             assert( image != 0 );

             // создаём картинки
             rgb = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 3 );
             r_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
             g_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
             b_plane = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
             r_range = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
             g_range = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
             b_range = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
             rgb_and = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
             //  копируем
             cvCopy(image, rgb);
             // разбиваем на отельные каналы
             cvSplit( rgb, b_plane, g_plane, r_plane, 0 );

             //
             // определяем минимальное и максимальное значение
             // у каналов HSV
             double framemin=0;
             double framemax=0;

             cvMinMaxLoc(r_plane, &framemin, &framemax);
             printf("[R] %f x %f\n", framemin, framemax );
             Rmin = framemin;
             Rmax = framemax;
             cvMinMaxLoc(g_plane, &framemin, &framemax);
             printf("[G] %f x %f\n", framemin, framemax );
             Gmin = framemin;
             Gmax = framemax;
             cvMinMaxLoc(b_plane, &framemin, &framemax);
             printf("[B] %f x %f\n", framemin, framemax );
             Bmin = framemin;
             Bmax = framemax;

             // окна для отображения картинки
             cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
             cvNamedWindow("R",CV_WINDOW_AUTOSIZE);
             cvNamedWindow("G",CV_WINDOW_AUTOSIZE);
             cvNamedWindow("B",CV_WINDOW_AUTOSIZE);
             cvNamedWindow("R range",CV_WINDOW_AUTOSIZE);
             cvNamedWindow("G range",CV_WINDOW_AUTOSIZE);
             cvNamedWindow("B range",CV_WINDOW_AUTOSIZE);
             cvNamedWindow("rgb and",CV_WINDOW_AUTOSIZE);

             cvCreateTrackbar("Rmin", "R range", &Rmin, RGBmax, myTrackbarRmin);
             cvCreateTrackbar("Rmax", "R range", &Rmax, RGBmax, myTrackbarRmax);
             cvCreateTrackbar("Gmin", "G range", &Gmin, RGBmax, myTrackbarGmin);
             cvCreateTrackbar("Gmax", "G range", &Gmax, RGBmax, myTrackbarGmax);
             cvCreateTrackbar("Bmin", "B range", &Gmin, RGBmax, myTrackbarBmin);
             cvCreateTrackbar("Bmax", "B range", &Gmax, RGBmax, myTrackbarBmax);

             //
             // разместим окна по рабочему столу
             //
             if(image->width <1920/4 && image->height<1080/2){
                     cvMoveWindow("original", 0, 0);
                     cvMoveWindow("R", image->width+10, 0);
                     cvMoveWindow("G", (image->width+10)*2, 0);
                     cvMoveWindow("B", (image->width+10)*3, 0);
                     cvMoveWindow("rgb and", 0, image->height+30);
                     cvMoveWindow("R range", image->width+10, image->height+30);
                     cvMoveWindow("G range", (image->width+10)*2, image->height+30);
                     cvMoveWindow("B range", (image->width+10)*3, image->height+30);
             }

             while(true){

                     // показываем картинку
                     cvShowImage("original",image);

                     // показываем слои
                     cvShowImage( "R", r_plane );
                     cvShowImage( "G", g_plane );
                     cvShowImage( "B", b_plane );

                     // показываем результат порогового преобразования
                     cvShowImage( "R range", r_range );
                     cvShowImage( "G range", g_range );
                     cvShowImage( "B range", b_range );

                     // складываем
                     cvAnd(r_range, g_range, rgb_and);
                     cvAnd(rgb_and, b_range, rgb_and);

                     // показываем результат
                     cvShowImage( "rgb and", rgb_and );

                     char c = cvWaitKey(33);
                     if (c == 27) { // если нажата ESC - выходим
                             break;
                     }
             }
             printf("\n[i] Results:\n" );
             printf("[i][R] %d : %d\n", Rmin, Rmax );
             printf("[i][G] %d : %d\n", Gmin, Gmax );
             printf("[i][B] %d : %d\n", Bmin, Bmax );

             // освобождаем ресурсы
             cvReleaseImage(& image);
             cvReleaseImage(&rgb);
             cvReleaseImage(&r_plane);
             cvReleaseImage(&g_plane);
             cvReleaseImage(&b_plane);
             cvReleaseImage(&r_range);
             cvReleaseImage(&g_range);
             cvReleaseImage(&b_range);
             cvReleaseImage(&rgb_and);
             // удаляем окна
             cvDestroyAllWindows();
    }

    const int GREEN_MIN = 21;
    const int GREEN_MAX = 110;



    void MainWindow::on_pushButton_34_clicked()
    {
        QString base_folder_3 = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
        QString neg = QFileDialog::getOpenFileName(this,
                                                    tr("Выбор Изображения"),
                                                    base_folder_3,
                                                    tr("Таблицы (*.jpg)"),nullptr, QFileDialog::DontResolveSymlinks);

        cv::Mat img_object=cv::imread(neg.toStdString(), 0);
        std::vector<cv::KeyPoint> keypoints_object, keypoints_scene;
        cv::Mat descriptors_object, descriptors_scene;


        cv::ORB::create(500, 1.2, 4, 31, 0, 2, 0, 31);
        Ptr<FeatureDetector> detector = ORB::create();

        // особые точки объекта
        detector->detect(img_object, keypoints_object);
        detector->compute(img_object, keypoints_object, descriptors_object);

        // особые точки картинки
        cv::Mat img = cv::imread(neg.toStdString(), 1);
        cv::Mat img_scene = cv::Mat(img.size(), CV_8UC1);
        detector->detect(img, keypoints_scene);
        detector->compute(img, keypoints_scene, descriptors_scene);

       // cv::imshow("desrs", descriptors_scene);
       // cvWaitKey();
        int test[10];
        for (int q = 0; q<10 ; q++) test[q]=descriptors_scene.data[q];

        //-- matching descriptor vectors using FLANN matcher
        cv::BFMatcher matcher;
        std::vector<cv::DMatch> matches;
        cv::Mat img_matches;
        if(!descriptors_object.empty() && !descriptors_scene.empty()) {
            matcher.match (descriptors_object, descriptors_scene, matches);
            double max_dist = 0; double min_dist = 100;

            // calculation of max and min idstance between keypoints
            for( int i = 0; i < descriptors_object.rows; i++)
            { double dist = matches[i].distance;
                if( dist < min_dist ) min_dist = dist;
                if( dist > max_dist ) max_dist = dist;
            }

            //-- Draw only good matches (i.e. whose distance is less than 3*min_dist)
            std::vector< cv::DMatch >good_matches;
           for( int i = 0; i < descriptors_object.rows; i++ ) if( matches[i].distance < (max_dist/1.6) ) good_matches.push_back(matches[i]);

            //cv::drawMatches(img_object, keypoints_object, img_scene, keypoints_scene, good_matches, img_matches, cv::Scalar::all(-1), cv::Scalar::all(-1),
            //std::vector<char>(), cv::DrawMatchesFlags::DEFAULT);
           drawKeypoints(img_object, keypoints_object, img_matches, cv::Scalar::all(-1), DrawMatchesFlags::DEFAULT );
        }

        cv::imshow("match result", img_matches );
        cv::waitKey();

    }

    void FASTfeatureTest(uchar * im, uchar * imres, int w, int h, int R, int n, int t)
    {
        // массивы результатов сравнений центральной точки и окружностей радиуса 1 2 3
        int mass1[8]; int mass2[12]; int mass3[16];  // 0 - темнее, 1 - похожий, 2 - светлее

        // смещения от центральной точки к точкам окружности
        int di1[8];
        di1[0]=-1*w; di1[1]=-1*w+1; di1[2]=0*w+1; di1[3]=1*w+1; di1[4]=1*w; di1[5]=1*w-1; di1[6]=0*w-1; di1[7]=-1*w-1;
        int di2[12];
        di2[0]=-2*w; di2[1]=-2*w+1; di2[2]=-1*w+2; di2[3]=0*w+2; di2[4]=1*w+2; di2[5]=2*w+1; di2[6]=2*w; di2[7]=2*w-1;di2[8]=1*w-2;di2[9]=0*w-2;di2[10]=-1*w-2;di2[11]=-2*w-1;
        int di3[16];
        di3[0]= - 3*w;di3[1]= - 3*w+1;	di3[2]= - 2*w+2;di3[3]= - 1*w+3;di3[4]= - 0*w+3;di3[5]= 1*w+3;di3[6]= 2*w+2;di3[7]= 3*w+1;
        di3[8]= 3*w+0;di3[9]= 3*w-1;di3[10]= 2*w-2;di3[11]= 1*w-3;di3[12]= 0*w-3;di3[13]= - 1*w-3;di3[14]= - 2*w-2;di3[15]= - 3*w-1;

        // проходимся по всем точкам изображения
        for (int y = 4; y < h - 4; y++) for (int x = 4; x < w - 4; x++)
        {
            // номер центральной точки
            int i0 = y*w + x;
            int cont0 = 0;int cont2 = 0;// число непрерывно идущих пикселов темнее и светлее соответственно
            bool isfeature = false;  // особая точка или нет

            // круг радиуса 1
            if (R==1)
            {
                for (int m=0; m<8; m++)
                {
                    if (im[i0+di1[m]] > im[i0]+t) mass1[m]=2;
                    else if (im[i0+di1[m]] < im[i0]-t) mass1[m]=0;
                    else mass1[m]=1;
                }
                cont0=0; cont2=0;
                for (int i =0; i<8; i++)
                {
                    if (mass1[i]==0) cont0++; else cont0=0; if (cont0==n) {isfeature=true; break;}
                    if (mass1[i]==2) cont2++; else cont2=0; if (cont2==n) {isfeature=true; break;}
                }
                for (int i =0; i<8; i++)
                {
                    if (mass1[i]==0) cont0++; else cont0=0; if (cont0==n) {isfeature=true; break;}
                    if (mass1[i]==2) cont2++; else cont2=0; if (cont2==n) {isfeature=true; break;}
                }
            }

            // радиус 2
            if (R==2)
            {
                // вычисляем соотношения для всех точек круга
                for (int m=0; m<12; m++)
                {
                    if (im[i0+di2[m]] > im[i0]+t) mass2[m]=2;
                    else if (im[i0+di2[m]] < im[i0]-t) mass2[m]=0;
                    else mass2[m]=1;
                }
                // анализируем и выносим решение
                cont0=0; cont2=0;
                for (int i =0; i<12; i++)
                {
                    if (mass2[i]==0) cont0++; else cont0=0; if (cont0==n) {isfeature=true; break;}
                    if (mass2[i]==2) cont2++; else cont2=0; if (cont2==n) {isfeature=true; break;}
                }
                for (int i =0; i<12; i++)
                {
                    if (mass2[i]==0) cont0++; else cont0=0; if (cont0==n) {isfeature=true; break;}
                    if (mass2[i]==2) cont2++; else cont2=0; if (cont2==n) {isfeature=true; break;}
                }
            }

            if (R==3)
            {
                for (int m=0; m<16; m++)
                {
                    if (im[i0+di3[m]] > im[i0]+t) mass3[m]=2;
                    else if (im[i0+di3[m]] < im[i0]-t) mass3[m]=0;
                    else mass3[m]=1;
                }
                cont0=0; cont2=0;
                for (int i =0; i<16; i++)
                {
                    if (mass3[i]==0) cont0++; else cont0=0; if (cont0==n) {isfeature=true; break;}
                    if (mass3[i]==2) cont2++; else cont2=0; if (cont2==n) {isfeature=true; break;}
                }
                for (int i =0; i<16; i++)
                {
                    if (mass3[i]==0) cont0++; else cont0=0; if (cont0==n) {isfeature=true; break;}
                    if (mass3[i]==2) cont2++; else cont2=0; if (cont2==n) {isfeature=true; break;}
                }
            }

            // закрашиваем особые
            if (isfeature) imres[i0]=255;
            else imres[i0]=im[i0]*0.5;
        }

    }

    void SaveBMP(char * filename, uchar * ptr, int width, int height)
    {
        FILE * vstream = fopen(filename,"wb");
        unsigned short FileType = 19778;
        unsigned long  FileSize;
        unsigned short Reserv1  =     0;
        unsigned short Reserv2  =     0;
        unsigned long  Offset   =  54;
        unsigned long  InfSize  =    40;
        unsigned long  BmWidht = width;
        unsigned long  BmHight = height;
        unsigned short Planes   =     1;
        unsigned short Bits     =     24;
        unsigned long  Compress =     0 ;
        unsigned long  SizeImage;
        unsigned long  XperMetr =     0;
        unsigned long  YperMetr =     0;
        unsigned long  UsColors =   256;
        unsigned long  ImpColors=     0;
        // запись шапки выходного файла
        SizeImage = width*height*3;
        FileSize = SizeImage+Offset;
        fseek(vstream,0,SEEK_SET);
        fwrite(&FileType, 2,1,vstream);fwrite(&FileSize, 4,1,vstream);
        fwrite(&Reserv1 , 2,1,vstream);fwrite(&Reserv2 , 2,1,vstream);
        fwrite(&Offset  , 4,1,vstream);fwrite(&InfSize,  4,1,vstream);
        fwrite(&width,  4,1,vstream);fwrite(&height,  4,1,vstream);
        fwrite(&Planes ,  2,1,vstream);fwrite(&Bits   ,  2,1,vstream);
        fwrite(&Compress, 4,1,vstream);fwrite(&SizeImage,4,1,vstream);
        fwrite(&XperMetr, 4,1,vstream);fwrite(&YperMetr, 4,1,vstream);
        fwrite(&UsColors, 4,1,vstream);fwrite(&ImpColors,4,1,vstream);

        // запись файла в массив по строкам
        for (int j=0; j<height; j++)
        {
            double Seek_Position = Offset + 3*(height-j)*width;
            fseek(vstream,Seek_Position,SEEK_SET);
            fwrite(ptr+j*3*width, 1, width*3, vstream);
        }
        fclose(vstream);
    }

    uchar * LoadBMP(char * filename, int * width, int * height)
    {
        // считываем шапку
        FILE * istream = fopen(filename,"rb");
        unsigned short FileType = 19778;
        unsigned long  FileSize;
        unsigned short Reserv1  =     0;
        unsigned short Reserv2  =     0;
        unsigned long  Offset   =  1078;
        unsigned long  InfSize  =    40;
        unsigned long  BmWidht;
        unsigned long  BmHight;
        unsigned short Planes   =     1;
        unsigned short Bits     =     8;
        unsigned long  Compress =     0 ;
        unsigned long  SizeImage;
        unsigned long  XperMetr =     0;
        unsigned long  YperMetr =     0;
        unsigned long  UsColors =   256;
        unsigned long  ImpColors=     0;
        fseek(istream,0,SEEK_SET);
        fread(&FileType, 2,1,istream);fread(&FileSize, 4,1,istream);
        fread(&Reserv1 , 2,1,istream);fread(&Reserv2 , 2,1,istream);
        fread(&Offset  , 4,1,istream);fread(&InfSize,  4,1,istream);
        fread(&BmWidht,  4,1,istream);fread(&BmHight,  4,1,istream);
        fread(&Planes ,  2,1,istream);fread(&Bits   ,  2,1,istream);
        fread(&Compress, 4,1,istream);fread(&SizeImage,4,1,istream);
        fread(&XperMetr, 4,1,istream);fread(&YperMetr, 4,1,istream);
        fread(&UsColors, 4,1,istream);fread(&ImpColors,4,1,istream);

        // создаём массив
        uchar * ptr = new uchar[3*BmWidht*BmHight];

        // запись файла в массив по строкам
        for (int j=0; j<BmHight; j++)
        {
            double Seek_Position = Offset + 3*(BmHight-j)*BmWidht;
            fseek(istream,Seek_Position,SEEK_SET);
            fread(ptr+j*3*BmWidht, 1, BmWidht*3, istream);
        }

        *width = BmWidht;
        *height = BmHight;
        return ptr;
    }


    // переходы между 1 и 3 канальными картинками
    void im1to3(uchar * im1, uchar * im3, int size1)
    {
        int i1=0; int i3=0;
        for (int i1=0; i1<size1; i1++){ im3[i3]=im1[i1];im3[i3+1]=im1[i1];im3[i3+2]=im1[i1];i3+=3;}
    }
    void im3to1(uchar * im3, uchar * im1, int size1)
    {
        int i1=0; int i3=0;
        for (int i1=0; i1<size1; i1++){ im1[i1]=(im3[i3]+im3[i3+1]+im3[i3+2])/3; i3+=3;}
    }


    void MainWindow::on_pushButton_35_clicked()
    {
        int w = 0;int h = 0;

            // загрузим картинку
            uchar * ptr = LoadBMP("9.bmp", &w,&h);
            uchar * ptr1 = new uchar[w*h];
            uchar * ptr2 = new uchar[w*h];
            uchar * ptr3 = new uchar[3*w*h];

            // в градации серого
            im3to1(ptr, ptr1, w*h);

            // обрабатываем детектором FAST
            // параметры:
            // w и h - ширина и высота картинки
            // R - радиус окружности (1,2 или 3)
            // n - сколько непрерывных светлых или тёмных пикселов достаточно, чтобы считать точку особой
            // t - порог по яркости
            FASTfeatureTest(ptr1, ptr2, w, h, 3, 9, 50);

            // обратно в цветной
            im1to3(ptr2,ptr3,w*h);

            // сохраним
            SaveBMP("out.bmp", ptr3, w, h);

    }

    void MainWindow::on_pushButton_36_clicked()
    {
        QImage tmpSrc =gpiSrc->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        unsigned int MaskSumm=0;
        char Mask[9][9] = {{0,0,0,1,3,1,0,0,0},
                           {0,0,1,3,3,3,1,0,0},
                           {0,1,-1,-2,-3,-2,-1,1,0},
                           {1,3,-2,-3,-4,-3,-2,3,1},
                           {3,3,-3,-4,-4,-4,-3,3,3},
                           {1,3,-2,-3,-4,-3,-2,3,1},
                           {0,1,-1,-2,-3,-2,-1,1,0},
                           {0,0,1,3,3,3,1,0,0},
                           {0,0,0,1,3,1,0,0,0}};
        for(int i=0;i<8;i++)
            for(int j=0;j<8;j++)
                MaskSumm+=Mask[i][j];

    for(int x=4; x<tmpSrc.width()-4;x++)
        for(int y=4;y<tmpSrc.height()-4;y++)
        {
            int sumR=0;int sumG=0;int sumB=0;
                for(int i=0;i<8;i++)
                    for (int j=0;j<8;j++)
                    {
                        sumR+=(Mask[i][j]*qRed(tmpSrc.pixel(x+i-4,y+j-4)));
                        sumR=sumR/8;
                        //qDebug () <<"SumR="<<sumR<<endl;

                        sumG+=(Mask[i][j]*qRed(tmpSrc.pixel(x+i-4,y+j-4)));
                        sumG=sumG/8;
                        //qDebug ()<<"SumG="<<sumG<<endl;

                        sumB+=(Mask[i][j]*qRed(tmpSrc.pixel(x+i-4,y+j-4)));
                        sumB=sumB/8;
                       // qDebug () <<"SumB="<<sumB<<endl;

                    }
                if(sumR>0)
                {
                    if(sumG>0)
                    {
                        if(sumB>0)
                        {
                            tmpDst.setPixel(x,y,qRgb(sumR/MaskSumm,sumG/MaskSumm,sumB/MaskSumm));
                        }
                        else {
                            tmpDst.setPixel(x,y,qRgb(255,255,255));
                        }
                    }
                    else {
                        tmpDst.setPixel(x,y,qRgb(0,0,0));
                    }
                }
                else {
                    tmpDst.setPixel(x,y,qRgb(0,0,0));
                }
        }
        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::on_horizontalSlider_4_actionTriggered(int action)
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        for(int y=0; y<tmpSrc.height();++y)
            for(int x=0; x<tmpSrc.width();++x)
            {
                if(qGreen(tmpSrc.pixel(x,y))>action)
                    tmpDst.setPixel(x,y,qRgb(255,255,255));
                        else
                    tmpDst.setPixel(x,y,qRgb(0,0,0));
            }
        gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }

    const CvScalar BLACK = CV_RGB(0, 0, 0);                         // чёрный
    const CvScalar WHITE = CV_RGB(255, 255, 255);           // белый

    const CvScalar RED = CV_RGB(255, 0, 0);                         // красный
    const CvScalar ORANGE = CV_RGB(255, 100, 0);            // оранжевый
    const CvScalar YELLOW = CV_RGB(255, 255, 0);            // жёлтый
    const CvScalar GREEN = CV_RGB(0, 255, 0);                       // зелёный
    const CvScalar LIGHTBLUE = CV_RGB(60, 170, 255);        // голубой
    const CvScalar BLUE = CV_RGB(0, 0, 255);                        // синий
    const CvScalar VIOLET = CV_RGB(194, 0, 255);            // фиолетовый

    const CvScalar GINGER = CV_RGB(215, 125, 49);           // рыжий
    const CvScalar PINK = CV_RGB(255, 192, 203);            // розовый
    const CvScalar LIGHTGREEN = CV_RGB(153, 255, 153);      // салатовый
    const CvScalar BROWN = CV_RGB(150, 75, 0);                      // коричневый

    typedef unsigned char uchar;
    typedef unsigned int uint;

    typedef struct ColorCluster {
            CvScalar color;
            CvScalar new_color;
            int count;

            ColorCluster():count(0) {
            }
    } ColorCluster;

    float rgb_euclidean(CvScalar p1, CvScalar p2)
    {
            float val = sqrtf( (p1.val[0]-p2.val[0])*(p1.val[0]-p2.val[0]) +
                    (p1.val[1]-p2.val[1])*(p1.val[1]-p2.val[1]) +
                    (p1.val[2]-p2.val[2])*(p1.val[2]-p2.val[2]) +
                    (p1.val[3]-p2.val[3])*(p1.val[3]-p2.val[3]));

            return val;
    }

    // сортировка цветов по количеству
    bool colors_sort(std::pair< int, uint > a, std::pair< int, uint > b)
    {
            return (a.second > b.second);
    }

    void MainWindow::on_pushButton_37_clicked()
    {
        typedef unsigned char uchar;
        // для хранения изображения
                IplImage* image=0, *src=0, *dst=0, *dst2=0;
                uchar b1,g1,r1;
                //
                // загрузка изображения
                //

                QString base_folder_2 = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
                QString Linefound = QFileDialog::getOpenFileName(this,
                                                            tr("Выбор Изображения"),
                                                            base_folder_2,
                                                            tr("Таблицы (*.jpg)"),nullptr, QFileDialog::DontResolveSymlinks);
                        std::string temp(Linefound.toStdString());
                       // имя картинки задаётся первым параметром

                      const char* image_filename = temp.c_str();
                     image = cvLoadImage(image_filename,1);

                // получаем картинку
                image = cvLoadImage(image_filename, 1);

                printf("[i] image: %s\n", image_filename);
                if(!image){
                        printf("[!] Error: cant load test image: %s\n", image_filename);
                        qDebug()<<"Error";
                }

                // показываем картинку
                cvNamedWindow("image");
                cvShowImage("image", image);

                // ресайзим картинку (для скорости обработки)
                src = cvCreateImage(cvSize(image->width/2, image->height/2), IPL_DEPTH_8U, 3);
                cvResize(image, src, CV_INTER_LINEAR);

                cvNamedWindow("img");
                cvShowImage("img", src);

                // картинка для хранения индексов кластеров
                IplImage* cluster_indexes = cvCreateImage(cvGetSize(src), IPL_DEPTH_8U, 1);
                cvZero(cluster_indexes);

        #define CLUSTER_COUNT 10
                int cluster_count = CLUSTER_COUNT;
                ColorCluster clusters[CLUSTER_COUNT];

                int i=0, j=0, k=0, x=0, y=0;

                // начальные цвета кластеров
        #if 0
                clusters[0].new_color = RED;
                clusters[1].new_color = ORANGE;
                clusters[2].new_color = YELLOW;
                //      clusters[3].new_color = GREEN;
                //      clusters[4].new_color = LIGHTBLUE;
                //      clusters[5].new_color = BLUE;
                //      clusters[6].new_color = VIOLET;
        #elif 0
                clusters[0].new_color = BLACK;
                clusters[1].new_color = GREEN;
                clusters[2].new_color = WHITE;
        #else
                CvRNG rng = cvRNG(-1);
                for(k=0; k<cluster_count; k++)
                        clusters[k].new_color = CV_RGB(cvRandInt(&rng)%255, cvRandInt(&rng)%255, cvRandInt(&rng)%255);
        #endif

                // k-means
                float min_rgb_euclidean = 0, old_rgb_euclidean=0;

                while(1) {
                        for(k=0; k<cluster_count; k++) {
                                clusters[k].count = 0;
                                clusters[k].color = clusters[k].new_color;
                                clusters[k].new_color = cvScalarAll(0);
                        }

                        for (y=0; y<src->height; y++) {
                                for (x=0; x<src->width; x++) {
                                        // получаем RGB-компоненты пикселя
                                        uchar B = CV_PIXEL(uchar, src, x, y)[0];        // B
                                        uchar G = CV_PIXEL(uchar, src, x, y)[1];        // G
                                        uchar R = CV_PIXEL(uchar, src, x, y)[2];        // R

                                        min_rgb_euclidean = 255*255*255;
                                        int cluster_index = -1;
                                        for(k=0; k<cluster_count; k++) {
                                                float euclid = rgb_euclidean(cvScalar(B, G, R, 0), clusters[k].color);
                                                if(  euclid < min_rgb_euclidean ) {
                                                        min_rgb_euclidean = euclid;
                                                        cluster_index = k;
                                                }
                                        }
                                        // устанавливаем индекс кластера
                                        CV_PIXEL(uchar, cluster_indexes, x, y)[0] = cluster_index;

                                        clusters[cluster_index].count++;
                                        clusters[cluster_index].new_color.val[0] += B;
                                        clusters[cluster_index].new_color.val[1] += G;
                                        clusters[cluster_index].new_color.val[2] += R;
                                }
                        }

                        min_rgb_euclidean = 0;
                        for(k=0; k<cluster_count; k++) {
                                // new color
                                clusters[k].new_color.val[0] /= clusters[k].count;
                                clusters[k].new_color.val[1] /= clusters[k].count;
                                clusters[k].new_color.val[2] /= clusters[k].count;
                                float ecli = rgb_euclidean(clusters[k].new_color, clusters[k].color);
                                if(ecli > min_rgb_euclidean)
                                        min_rgb_euclidean = ecli;
                        }

                        //printf("%f\n", min_rgb_euclidean);
                        if( fabs(min_rgb_euclidean - old_rgb_euclidean)<1 )
                                break;

                        old_rgb_euclidean = min_rgb_euclidean;
                }
                //-----------------------------------------------------

                // теперь загоним массив в вектор и отсортируем :)
                std::vector< std::pair< int, uint > > colors;
                colors.reserve(CLUSTER_COUNT);

                int colors_count = 0;
                for(i=0; i<CLUSTER_COUNT; i++){
                        std::pair< int, uint > color;
                        color.first = i;
                        color.second = clusters[i].count;
                        colors.push_back( color );
                        if(clusters[i].count>0)
                                colors_count++;
                }
                // сортируем
                std::sort( colors.begin(), colors.end(), colors_sort );

                // покажем цвета
                dst2 = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 3 );
                cvZero(dst2);
                int h = dst2->height / CLUSTER_COUNT;
                int w = dst2->width;
                for(i=0; i<CLUSTER_COUNT; i++ ){
                        cvRectangle(dst2, cvPoint(0, i*h), cvPoint(w, i*h+h), clusters[colors[i].first].color, -1);
                        printf("[i] Color: %d %d %d (%d)\n", (int)clusters[colors[i].first].color.val[2],
                                (int)clusters[colors[i].first].color.val[1],
                                (int)clusters[colors[i].first].color.val[0],
                                clusters[colors[i].first].count);
                }
                cvNamedWindow("colors");
                cvShowImage("colors", dst2);
                //cvResize(dst2, image, CV_INTER_LINEAR);
                //cvSaveImage("dominate_colors_table.png", image);
                //-----------------------------------------------------

                // покажем картинку в найденных цветах
                dst = cvCloneImage(src);
                for (y=0; y<dst->height; y++) {
                        for (x=0; x<dst->width; x++) {
                                int cluster_index = CV_PIXEL(uchar, cluster_indexes, x, y)[0];

                                CV_PIXEL(uchar, dst, x, y)[0] = clusters[cluster_index].color.val[0];
                                CV_PIXEL(uchar, dst, x, y)[1] = clusters[cluster_index].color.val[1];
                                CV_PIXEL(uchar, dst, x, y)[2] = clusters[cluster_index].color.val[2];
                        }
                }

                cvNamedWindow("dst");
                cvShowImage("dst", dst);
                //cvResize(dst, image, CV_INTER_LINEAR);
                //cvSaveImage("dominate_colors.png", image);
                //-----------------------------------------------------

                // ждём нажатия клавиши
                cvWaitKey(0);

                // освобождаем ресурсы
                cvReleaseImage(&image);
                cvReleaseImage(&src);

                cvReleaseImage(&cluster_indexes);

                cvReleaseImage(&dst);
                cvReleaseImage(&dst2);

                // удаляем окна
                cvDestroyAllWindows();

    }




    void MainWindow::on_pushButton_38_clicked()
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);
        QString after;
        QSet <QString> colorh;

          for (int i=0;i<tmpSrc.width();i++){
              for (int j=0;j<tmpSrc.height();j++){
                  QRgb color =tmpSrc.pixel( i, j );
                  QColor col =QColor(color);

                  QString ng = col.name();
                  QString nb = QColor(color).name();

                  nb.remove(0,1);

                  int d = nb.toInt(nullptr,16);
                  //cout<<"d="<<d<<endl;

                  int redU = qRed(tmpDst.pixel(i,j));
                  int blueU = qBlue(tmpDst.pixel(i,j));
                  int greenU = qGreen(tmpDst.pixel(i,j));
                  if( redU<45 && blueU<20 && greenU<20)
                  {
                      tmpDst.setPixel(i,j,qRgb(255,0,0));
                  }


                  Svet = svetu.size();
                  }
            }
          gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
    }
          //gpiDst->setPixmap(QPixmap::fromImage(tmpDst));




   void MainWindow::on_pushButton_39_clicked()
    {
        QImage tmpSrc =gpiDst->pixmap().toImage();
        QImage tmpDst(tmpSrc);

          for (int i=0;i<tmpSrc.width();i++){
              for (int j=0;j<tmpSrc.height();j++){

                  int red = qRed(tmpSrc.pixel(i,j));
                  int blue = qBlue(tmpSrc.pixel(i,j));
                  int green = qGreen(tmpSrc.pixel(i,j));

                  if(red<10 && blue<10 && green<10){
                      tmpDst.setPixel(i,j,qRgb(255,0,0));
                    peredacha(i,j);
                  }
              }
          }
          Obrabotka();
          //gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
    }

    void MainWindow::Obrabotka()
    {
        QImage tmpSrc =gpiSrc->pixmap().toImage();
        QImage tmpDst(tmpSrc);
        for (int i=0;i<tmpSrc.width();i++){
            for (int j=0;j<tmpSrc.height();j++){
                int red = qRed(tmpSrc.pixel(i,j));
                int blue = qBlue(tmpSrc.pixel(i,j));
                int green = qGreen(tmpSrc.pixel(i,j));

                if (red==255 && blue==0 && green==0)
                {
                    for(int n=-4;n<4;n++){
                        for (int h=-4;h<4;h++){
                            int redU = qRed(tmpDst.pixel(i+n,j+h));
                            int blueU = qBlue(tmpDst.pixel(i+n,j+h));
                            int greenU = qGreen(tmpDst.pixel(i+n,j+h));
                            if( redU<60 && blueU<45 && greenU<45)
                            {
                                tmpDst.setPixel(i+n,j+h,qRgb(0,0,200));
                            }
                            QColor color = tmpSrc.pixelColor(i,j);
                            QColor color1 = tmpSrc.pixelColor(i+n,j+h);
                            int Yar=color.lightness();
                            int Yar1=color1.lightness();

                                 if (Yar<Yar1 || Yar==Yar1 )
                                 {
                                      tmpDst.setPixel(i+n,j+h,qRgb(0,0,200));
                                 }
                                    if(n==0&&h==0)
                                    {
                                        tmpDst.setPixel(i+n,j+h,qRgb(0,0,200));
                                    }
                        }
                    }
                }
            }
        }

        gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));

        Ochistka();
    }

    void MainWindow::Ochistka()
    {
        QImage tmpSrc =gpiSrc->pixmap().toImage();
        QImage tmpDst(tmpSrc);

          for (int i=1;i<tmpSrc.width()-1;i++){
              for (int j=1;j<tmpSrc.height()-1;j++){

      //Block for checking if surrounding pixels is white

                  int red = qRed(tmpSrc.pixel(i,j));
                  int blue = qBlue(tmpSrc.pixel(i,j));
                  int green = qGreen(tmpSrc.pixel(i,j));


                  bool UpPixelIsWhite = true;
                  int redU = qRed(tmpSrc.pixel(i,j+1));
                  int blueU = qBlue(tmpSrc.pixel(i,j+1));
                  int greenU = qGreen(tmpSrc.pixel(i,j+1));
                  if((redU==255 && blueU==0 && greenU==255) || (blueU==255 && redU==0 && greenU==0) ){
                      UpPixelIsWhite=false;
                  }

                  bool RightPixelIsWhite = true;
                  int redR = qRed(tmpSrc.pixel(i+1,j));
                  int blueR = qBlue(tmpSrc.pixel(i+1,j));
                  int greenR = qGreen(tmpSrc.pixel(i+1,j));
                  if((redR==255 && blueR==0 && greenR==255) || (blueR==255 && redR==0 && greenR==0)){
                      RightPixelIsWhite=false;
                  }

                  bool DownPixelIsWhite = true;
                  int redD = qRed(tmpSrc.pixel(i,j-1));
                  int blueD = qBlue(tmpSrc.pixel(i,j-1));
                  int greenD = qGreen(tmpSrc.pixel(i,j-1));
                  if((redD==255 && blueD==0 && greenD==255) || (blueD==255 && redD==0 && greenD==0)){
                      DownPixelIsWhite=false;
                  }

                  bool LeftPixelIsWhite = true;
                  int redL = qRed(tmpSrc.pixel(i-1,j));
                  int blueL = qBlue(tmpSrc.pixel(i-1,j));
                  int greenL = qGreen(tmpSrc.pixel(i-1,j));
                  if((redL==255 && blueL==0 && greenL==255) || (blueL==255 && redL==0 && greenL==0)){
                      LeftPixelIsWhite=false;
                  }

                  bool UpRightPixelIsWhite = true;
                  int redUR = qRed(tmpSrc.pixel(i+1,j+1));
                  int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
                  int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
                  if((redUR==255 && blueUR==0 && greenUR==255) || (blueUR==255 && redUR==0 && greenUR==0)){
                      UpRightPixelIsWhite=false;
                  }

                  bool UpLeftPixelIsWhite = true;
                  int redUL = qRed(tmpSrc.pixel(i-1,j+1));
                  int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
                  int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
                  if((redUL==255 && blueUL==0 && greenUL==255) || (blueUL==255 && redUL==0 && greenUL==0)){
                      UpLeftPixelIsWhite=false;
                  }

                  bool DownRightPixelIsWhite = true;
                  int redDR = qRed(tmpSrc.pixel(i+1,j-1));
                  int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
                  int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
                  if((redDR==255 && blueDR==0 && greenDR==255) || (blueDR==255 && redDR==0 && greenDR==0)){
                      DownRightPixelIsWhite=false;
                  }

                  bool DownLeftPixelIsWhite = true;
                  int redDL = qRed(tmpSrc.pixel(i-1,j-1));
                  int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
                  int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
                  if((redDL==255 && blueDL==0 && greenDL==255) || (blueDL==255 && redDL==0 && greenDL==0)){
                      DownLeftPixelIsWhite=false;
                  }

                  if ((red==255 && blue==0 && green==0) && (UpPixelIsWhite==true || RightPixelIsWhite==true || DownPixelIsWhite==true || LeftPixelIsWhite==true ||
                          UpRightPixelIsWhite==true || UpLeftPixelIsWhite==true || DownLeftPixelIsWhite==true || DownRightPixelIsWhite==true)){
                      tmpDst.setPixel(i,j,qRgb(255,255,255));
                  }
              }
          }
      IplImage * pere;
      pere = QImage2IplImage(&tmpDst);
      CentrMass * CM = new CentrMass();
       gtr = new anizo;

      cv::Point2f center = CM->Simmetr(*pere);
      int Xc = center.x;
      int Yc = center.y;

      tmpDst.setPixel(Xc,Yc,qRgb(255,0,255));
      tmpDst.setPixel(Xc-1,Yc-1,qRgb(0,255,0));
      tmpDst.setPixel(Xc-1,Yc,qRgb(0,255,0));
      tmpDst.setPixel(Xc,Yc-1,qRgb(0,255,0));
      tmpDst.setPixel(Xc,Yc+1,qRgb(0,255,0));
      tmpDst.setPixel(Xc+1,Yc,qRgb(0,255,0));
      tmpDst.setPixel(Xc+1,Yc+1,qRgb(0,255,0));
      tmpDst.setPixel(Xc-1,Yc+1,qRgb(0,255,0));
      tmpDst.setPixel(Xc+1,Yc-1,qRgb(0,255,0));
      for(int b=0;b<tmpDst.width();b++)

      gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));

      gtr->show();
      gtr->Peredaxa(tmpDst);
      //CM->Simmetr(tmpDst);
    }


    void MainWindow::peredacha(int vec,int vecs)
    {
        QImage tmpSrc =gpiSrc->pixmap().toImage();
        QImage tmpDst(tmpSrc);

        for (int i=0;i<tmpSrc.width();i++){
            for (int j=0;j<tmpSrc.height();j++){
                if (i==vec && j==vecs)
                {
                    tmpDst.setPixel(i,j,qRgb(255,0,0));
                }
            }
        }
        gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
    }
/*
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
                    QColor color11 = tmpSrc.pixelColor(i-1,j-1);
                    int Yar11=color11.lightness();

        tmp = tmpDst;

       gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

        //ui->dstGrView->setScene(scTmp);
 */

void MainWindow::on_Osi_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
     gtr = new anizo;

    IplImage * pere;
    pere = QImage2IplImage(&tmpDst);
    CentrMass * CM = new CentrMass();

    cv::Point2f center = CM->Simmetr(*pere);
    int Xc = center.x;
    int Yc = center.y;

    for(int b=0;b<tmpDst.height();b++)
    {
        tmpDst.setPixel(Xc,b,qRgb(255,0,255));
    }
    for(int c=0;c<tmpDst.width();c++)
    {
        tmpDst.setPixel(c,Yc,qRgb(255,0,255));
    }
        tmpDst.setPixel(Xc,Yc,qRgb(255,0,0));

    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));

    gtr->show();
    gtr->Peredaxa(tmpDst);
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

void MainWindow::findCircles(IplImage* _image)
{
        assert(_image!=0);

        IplImage* bin = cvCreateImage( cvGetSize(_image), IPL_DEPTH_8U, 1);

        // конвертируем в градации серого
        cvConvertImage(_image, bin, CV_BGR2GRAY);
        // находим границы
        cvCanny(bin, bin, 50, 200);

        cvNamedWindow( "bin", 1 );
        cvShowImage("bin", bin);

        // хранилище памяти для контуров
        CvMemStorage* storage = cvCreateMemStorage(0);
        CvSeq* contours=0;

        // находим контуры
        int contoursCont = cvFindContours( bin, storage,&contours,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

        assert(contours!=0);

        // обходим все контуры
        for( CvSeq* current = contours; current != NULL; current = current->h_next ){
                // вычисляем площадь и периметр контура
                double area = fabs(cvContourArea(current));
                double perim = cvContourPerimeter(current);

                // 1/4*CV_PI = 0,079577
                if ( area / (perim * perim) > 0.07 && area / (perim * perim)< 0.087 ){ // в 10% интервале
                        // нарисуем контур
                        cvDrawContours(_image, current, cvScalar(0, 0, 255), cvScalar(0, 255, 0), -1, 1, 8);
                }
        }

        // освобождаем ресурсы
        cvReleaseMemStorage(&storage);
        cvReleaseImage(&bin);
}


void MainWindow::on_pushButton_41_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    IplImage * pere,*dst=0;;
    pere = QImage2IplImage(&tmpDst);
    findCircles(pere);

    cvNamedWindow( "original", 1 );
    cvShowImage( "original", pere );

            dst = cvCloneImage(pere);

            // показываем
            EnclosingCircle(dst);

            cvNamedWindow( "circles", 1 );
            cvShowImage( "circles", dst);

            // ждём нажатия клавиши
            cvWaitKey(0);

            // освобождаем ресурсы
            cvReleaseImage(&pere);
            cvReleaseImage(&dst);
            // удаляем окна
            cvDestroyAllWindows();

}

void MainWindow::on_Ochistka_clicked()
{
   // gpiSrc->clear;
    gpiDst->clearFocus();
}

void MainWindow::on_pushButton_40_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    for (int i=15;i<tmpSrc.width()-15;i++){
        for (int j=15;j<tmpSrc.height()-15;j++){
            for (int n=-15;n<15;n++){
                for (int l=-15;l<15;l++){
                     QColor color = tmpSrc.pixelColor(i+n,j+l);
                     int Yar = color.lightness();
                     QColor colorR = tmpSrc.pixelColor(i-n,j-l);
                     for (int g=-15;g<15;g++){
                         for (int y=-15;y<15;y++){
                         QColor Color = tmpSrc.pixelColor(i+g,j+y);
                         int Yar1 = Color.lightness();
                         if (Yar<Yar1 || Yar==Yar1)
                         {
                             tmpDst.setPixel(i+n,j+l,qRgb(255,0,0));
                         }
                         }
                     }
                }
            }
        }
    }
    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::EnclosingCircle(IplImage *_image)
{
    assert(_image!=0);

            IplImage* bin = cvCreateImage( cvGetSize(_image), IPL_DEPTH_8U, 1);

            // конвертируем в градации серого
            cvConvertImage(_image, bin, CV_BGR2GRAY);
            // находим границы
            cvCanny(bin, bin, 50, 200);

            cvNamedWindow( "bin", 1 );
            cvShowImage("bin", bin);

            // хранилище памяти для контуров
            CvMemStorage* storage = cvCreateMemStorage(0);
            CvSeq* contours=0;

            // находим контуры
            int contoursCont = cvFindContours( bin, storage,&contours,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

            assert(contours!=0);

            // обходим все контуры
            for( CvSeq* current = contours; current != NULL; current = current->h_next ){
                    CvPoint2D32f center;
                    float radius=0;
                    // находим параметры окружности
                    cvMinEnclosingCircle(current, & center, &radius);
                    // рисуем
                    cvCircle(_image, cvPointFrom32f(center), radius, CV_RGB(255, 0, 0), 1, 8);
            }

            // освобождаем ресурсы
            cvReleaseMemStorage(&storage);
            cvReleaseImage(&bin);
}


/*
            for (int g=-15;g<15;g++) {
                for (int f=-15;f<15;f++){
                    QColor color = tmpSrc.pixelColor(i=15+g,j=15+f);
                     int Yar=color.lightness();
                             for (int n=-15;n<15;n++) {
                                for (int l=-15;l<15;l++){
                                     QColor color1 = tmpSrc.pixelColor(i+n,j+l);
                                        int Yar1=color1.lightness();

                         if (Yar<Yar1 || Yar==Yar1 )
                         {
                              tmpDst.setPixel(i+n,j+l,qRgb(0,0,255));
                         }
                            if(n==0&&l==0)
                            {
                                tmpDst.setPixel(i+n,j+l,qRgb(255,0,0));
                            }
                }
                             }
            }
            }
  */

void MainWindow::on_Original_clicked()
{
    QImage tmpSrc =gpiOriginal->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
    //ui->srcGrView->setScene(scSrc);
}

void MainWindow::FindAssimtrya()
{

  }

void MainWindow::on_Assimetrya_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    IplImage * pere;
    pere = QImage2IplImage(&tmpDst);
    CentrMass * CM = new CentrMass();

    cv::Point2f center = CM->Simmetr(*pere);
    int Xc = center.x;
    int Yc = center.y;
    qDebug() << "Xc=" <<Xc<<"Yc="<<Yc;
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
    else {

          int red = qRed(tmpSrc.pixel(Xc,Yc));
          int blue = qBlue(tmpSrc.pixel(Xc,Yc));
          int green = qGreen(tmpSrc.pixel(Xc,Yc));

          int redPol = qRed(tmpSrc.pixel(wigthX,YPol));
          int bluePol = qBlue(tmpSrc.pixel(wigthX,YPol));
          int greenPol = qGreen(tmpSrc.pixel(wigthX,YPol));

          int redOtr = qRed(tmpSrc.pixel(wigthX,YOtr));
          int blueOtr = qBlue(tmpSrc.pixel(wigthX,YOtr));
          int greenOtr = qGreen(tmpSrc.pixel(wigthX,YOtr));

          while (red!=0 && green!=255 && blue!=255)
          {
               while(redPol!=0 && bluePol!=255 && greenPol!=255)
               {
                   SchPol++;
                   redPol = qRed(tmpSrc.pixel(wigthX,YPol));
                   greenPol = qGreen(tmpSrc.pixel(wigthX,YPol));
                   bluePol = qBlue(tmpSrc.pixel(wigthX,YPol));
                   YPol++;
               }

               redPol = qRed(tmpSrc.pixel(1,1));
               greenPol = qGreen(tmpSrc.pixel(1,1));
               bluePol = qBlue(tmpSrc.pixel(1,1));

               while(redOtr!=0 && blueOtr != 255 && greenOtr != 255)
               {
                   SchOtr++;
                   redOtr = qRed(tmpSrc.pixel(wigthX,YOtr));
                   blueOtr = qBlue(tmpSrc.pixel(wigthX,YOtr));
                   greenOtr = qGreen(tmpSrc.pixel(wigthX,YOtr));
                   YOtr--;
               }

               redOtr = qRed(tmpSrc.pixel(1,1));
               blueOtr = qBlue(tmpSrc.pixel(1,1));
               greenOtr = qGreen(tmpSrc.pixel(1,1));

               if(SchPol==SchOtr || SchPol== SchOtr+1 || SchPol== SchOtr-1)
               {
                   KoefX++;
                   for (SchOtr;SchOtr>0;SchOtr--)
                   {
                       tmpDst.setPixel(wigthX,Yc-SchOtr,qRgb(0,255,0));
                   }

                   for (SchPol;SchPol>0;SchPol--)
                   {
                       tmpDst.setPixel(wigthX,Yc+SchPol,qRgb(0,255,0));
                   }
               }
               else {
                   for (SchOtr;SchOtr>0;SchOtr--)
                   {
                       tmpDst.setPixel(wigthX,Yc-SchOtr,qRgb(255,0,0));
                   }

                   for (SchPol;SchPol>0;SchPol--)
                   {
                       tmpDst.setPixel(wigthX,Yc+SchPol,qRgb(255,0,0));
                   }
               }
               tmpDst.setPixel(wigthX,Yc,qRgb(255,0,255));
               YPol = Yc;
               YOtr = Yc;
               SchPol = 0;
               SchOtr = 0;
                wigthX++;
               red = qRed(tmpSrc.pixel(wigthX,Yc));
               blue = qBlue(tmpSrc.pixel(wigthX,Yc));
               green = qGreen(tmpSrc.pixel(wigthX,Yc));
               SchYup++;
            }
               red = qRed(tmpSrc.pixel(Xc,Yc));
               blue = qBlue(tmpSrc.pixel(Xc,Yc));
               green = qGreen(tmpSrc.pixel(Xc,Yc));

               redPol = qRed(tmpSrc.pixel(1,1));
               greenPol = qGreen(tmpSrc.pixel(1,1));
               bluePol = qBlue(tmpSrc.pixel(1,1));

               wigthX = Xc;

               while(red!=0 && green!=255 && blue!=255 )
               {
                   while(redPol!=0 && greenPol!=255 && bluePol !=255)
                   {
                       SchPol++;
                       redPol = qRed(tmpSrc.pixel(wigthX,YPol));
                       greenPol = qGreen(tmpSrc.pixel(wigthX,YPol));
                       bluePol = qBlue(tmpSrc.pixel(wigthX,YPol));
                       YPol++;
                   }

                   redPol = qRed(tmpSrc.pixel(1,1));
                   greenPol = qGreen(tmpSrc.pixel(1,1));
                   bluePol = qBlue(tmpSrc.pixel(1,1));

                   while(redOtr!=0 && greenOtr!=255 && blueOtr!=255)
                   {
                       SchOtr++;
                       redOtr = qRed(tmpSrc.pixel(wigthX,YOtr));
                       greenOtr = qGreen(tmpSrc.pixel(wigthX,YOtr));
                       blueOtr = qBlue(tmpSrc.pixel(wigthX,YOtr));
                       YOtr--;
                   }

                   redOtr = qRed(tmpSrc.pixel(1,1));
                   greenOtr = qGreen(tmpSrc.pixel(1,1));
                   blueOtr = qBlue(tmpSrc.pixel(1,1));

                   if(SchPol==SchOtr || SchPol== SchOtr+1 || SchPol== SchOtr-1)
                   {
                       KoefX++;
                       for (SchOtr;SchOtr>0;SchOtr--)
                       {
                           tmpDst.setPixel(wigthX,Yc-SchOtr,qRgb(0,255,0));
                       }

                       for (SchPol;SchPol>0;SchPol--)
                       {
                           tmpDst.setPixel(wigthX,Yc+SchPol,qRgb(0,255,0));
                       }
                   }
                   else {
                       for (SchOtr;SchOtr>0;SchOtr--)
                       {
                           tmpDst.setPixel(wigthX,Yc-SchOtr,qRgb(255,0,0));
                       }

                       for (SchPol;SchPol>0;SchPol--)
                       {
                           tmpDst.setPixel(wigthX,Yc+SchPol,qRgb(255,0,0));
                       }
                   }

                   tmpDst.setPixel(wigthX,Yc,qRgb(255,0,255));
                   SchPol = 0;
                   SchOtr = 0;
                   YPol = Yc;
                   YOtr = Yc;
                   wigthX--;
                   red = qRed(tmpSrc.pixel(wigthX,Yc));
                   green = qGreen(tmpSrc.pixel(wigthX,Yc));
                   blue = qBlue(tmpSrc.pixel(wigthX,Yc));
                   SchYup++;
               }
      }
    ResY=(float) KoefX/SchYup;
    qDebug() << "ResY"<<ResY;
    ui->textBrowser_2->append("Вывод анализа по ассиметрии по верхней части оси у");
    ui->textBrowser_2->append("KoefX = "+ QString::number(KoefX)+" SchYup = "+ QString::number(SchYup));
    ui->textBrowser_2->append("ResY =" + QString::number(ResY));

    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::on_FDM_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    IplImage * pere;
    pere = QImage2IplImage(&tmpDst);
    CentrMass * CM = new CentrMass();

    cv::Point2f center = CM->Simmetr(*pere);
    int Xc = center.x;
    int Yc = center.y;
    Yc = Yc + 30;
    qDebug() << "Xc=" <<Xc<<"Yc="<<Yc;
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
             redPol = qRed(tmpSrc.pixel(wigthX,heightY));
             greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
             bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
              FoundTochka(wigthX,heightY);
             wigthX++;
         }

         redPol = qRed(tmpSrc.pixel(1,1));
         greenPol = qGreen(tmpSrc.pixel(1,1));
         bluePol = qBlue(tmpSrc.pixel(1,1));

         while(redOtr!=0 && blueOtr != 255 && greenOtr != 255)
         {
             redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
             blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
             greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));
              FoundTochka(xOtr,heightY);
             xOtr--;

         }

         redOtr = qRed(tmpSrc.pixel(1,1));
         blueOtr = qBlue(tmpSrc.pixel(1,1));
         greenOtr = qGreen(tmpSrc.pixel(1,1));


         tmpDst.setPixel(Xc,heightY,qRgb(255,0,255));
         xOtr = Xc;
         wigthX = Xc;

          heightY++;
         red = qRed(tmpSrc.pixel(Xc,heightY));
         blue = qBlue(tmpSrc.pixel(Xc,heightY));
         green = qGreen(tmpSrc.pixel(Xc,heightY));

    }

    red = qRed(tmpSrc.pixel(Xc,Yc));
    blue = qBlue(tmpSrc.pixel(Xc,Yc));
    green = qGreen(tmpSrc.pixel(Xc,Yc));

    heightY = Yc;
    while (red!=0 && green!=255 && blue!=255)
    {
         while(redPol!=0 && bluePol!=255 && greenPol!=255)
         {
             redPol = qRed(tmpSrc.pixel(wigthX,heightY));
             greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
             bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
              FoundTochka(wigthX,heightY);
             wigthX++;
         }

         redPol = qRed(tmpSrc.pixel(1,1));
         greenPol = qGreen(tmpSrc.pixel(1,1));
         bluePol = qBlue(tmpSrc.pixel(1,1));

         while(redOtr!=0 && blueOtr != 255 && greenOtr != 255)
         {
             redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
             blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
             greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));
              FoundTochka(xOtr,heightY);
             xOtr--;
         }

         redOtr = qRed(tmpSrc.pixel(1,1));
         blueOtr = qBlue(tmpSrc.pixel(1,1));
         greenOtr = qGreen(tmpSrc.pixel(1,1));


         tmpDst.setPixel(Xc,heightY,qRgb(255,0,255));
         xOtr = Xc;
         wigthX = Xc;
          heightY--;
         red = qRed(tmpSrc.pixel(Xc,heightY));
         blue = qBlue(tmpSrc.pixel(Xc,heightY));
         green = qGreen(tmpSrc.pixel(Xc,heightY));
         //Obrabotka();
         OnrabotkaYellow();
         FoundYellow();

    }
}
void MainWindow::FoundTochka(int i, int j)
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    int fda=ui->lineEdit_10->text().toInt();
    int Stochki =0;
    int RedS = 0; int GreenS = 0; int BlueS=0;
    int Xc=0,Yc=0;
    int Hue =0;  int Saturation = 0;int Value =0;
    int D1=0,D2=0;

            QColor color = tmpSrc.pixelColor(i,j);
            int Yar = color.lightness();
            int red = qRed(tmpSrc.pixel(i,j));
            int green = qGreen(tmpSrc.pixel(i,j));
            int blue = qBlue(tmpSrc.pixel(i,j));

            bool UpPixelIsCol = false;
            int redU = qRed(tmpSrc.pixel(i,j+1));
            int blueU = qBlue(tmpSrc.pixel(i,j+1));
            int greenU = qGreen(tmpSrc.pixel(i,j+1));
            QColor colorUp = tmpSrc.pixelColor(i,j+1);
            int YarUp = colorUp.lightness();
            if((redU>red ) && (blueU>blue ) && (greenU>green ) && (YarUp>Yar+fda)) {
                UpPixelIsCol=true;
            }

            bool RightPixelIsCol = false;
            int redR = qRed(tmpSrc.pixel(i+1,j));
            int blueR = qBlue(tmpSrc.pixel(i+1,j));
            int greenR = qGreen(tmpSrc.pixel(i+1,j));
            QColor colorRight = tmpSrc.pixelColor(i+1,j);
            int YarRight = colorRight.lightness();
            if((redR>red ) && (blueR>blue ) && (greenR>green) && (YarRight>Yar+fda)) {
                RightPixelIsCol=true;
            }

            bool DownPixelIsCol = false;
            int redD = qRed(tmpSrc.pixel(i,j-1));
            int blueD = qBlue(tmpSrc.pixel(i,j-1));
            int greenD = qGreen(tmpSrc.pixel(i,j-1));
            QColor colorDown = tmpSrc.pixelColor(i,j-1);
            int YarDown = colorDown.lightness();
            if(( redD>red) && ( blueD>blue) && ( greenD>green) && (YarDown>Yar+fda)){
                DownPixelIsCol=true;
            }

            bool LeftPixelIsCol = false;
            int redL = qRed(tmpSrc.pixel(i-1,j));
            int blueL = qBlue(tmpSrc.pixel(i-1,j));
            int greenL = qGreen(tmpSrc.pixel(i-1,j));
            QColor colorLeft = tmpSrc.pixelColor(i-1,j);
            int YarLeft = colorLeft.lightness();
            if(( redL>red) && ( blueL>blue) && ( greenL>green) && (YarLeft>Yar+fda)){
                LeftPixelIsCol=true;
            }

            bool UpRightPixelIsCol = false;
            int redUR = qRed(tmpSrc.pixel(i+1,j+1));
            int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
            int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
            QColor colorUR = tmpSrc.pixelColor(i+1,j+1);
            int YarUR = colorUR.lightness();
            if(( redUR>red) && ( blueUR>blue) && ( greenUR>green) && (YarUR>Yar+fda)){
                UpRightPixelIsCol=true;
            }

            bool UpLeftPixelIsCol = false;
            int redUL = qRed(tmpSrc.pixel(i-1,j+1));
            int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
            int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
            QColor colorUL = tmpSrc.pixelColor(i-1,j+1);
            int YarUL = colorUL.lightness();
            if((redUL>red) && ( blueUL>blue) && ( greenUL>green) && (YarUL>Yar+fda)){
                UpLeftPixelIsCol=true;
            }

            bool DownRightPixelIsCol = false;
            int redDR = qRed(tmpSrc.pixel(i+1,j-1));
            int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
            int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
            QColor colorDR = tmpSrc.pixelColor(i+1,j-1);
            int YarDR = colorDR.lightness();
            if(( redDR>red) && ( blueDR>blue) && ( greenDR>green) && (YarDR>Yar+fda)){
                DownRightPixelIsCol=true;
            }

            bool DownLeftPixelIsCol = false;
            int redDL = qRed(tmpSrc.pixel(i-1,j-1));
            int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
            int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
            QColor colorDL = tmpSrc.pixelColor(i-1,j-1);
            int YarDL = colorDL.lightness();
            if(( redDL>red) && ( blueDL>blue) && ( greenDL>green) && (YarDL>Yar+fda)){
                DownLeftPixelIsCol=true;
            }

            if ((UpPixelIsCol==true && RightPixelIsCol==true && DownPixelIsCol==true && LeftPixelIsCol==true &&
                    UpRightPixelIsCol==true && UpLeftPixelIsCol==true && DownLeftPixelIsCol==true && DownRightPixelIsCol==true)){

                tmpDst.setPixel(i,j,qRgb(255,0,0));
                //qDebug()<<"if Verno";

                for(int n=-3;n<3;n++){
                    for (int h=-3;h<3;h++){
                        int redU = qRed(tmpDst.pixel(i+n,j+h));
                        int blueU = qBlue(tmpDst.pixel(i+n,j+h));
                        int greenU = qGreen(tmpDst.pixel(i+n,j+h));
                        QColor color = tmpSrc.pixelColor(i,j);
                        QColor color1 = tmpSrc.pixelColor(i+n,j+h);
                        int Yar=color.lightness();
                        int Yar1=color1.lightness();
                        if( (redU<64 && blueU<32 && greenU<32)||  Yar<Yar1 || Yar==Yar1 && (redU!=0 && greenU!=255 && blueU!=255))
                        {
                            tmpDst.setPixel(i+n,j+h,qRgb(255,255,0));
                            Stochki++;
                            RedS += qRed(tmpDst.pixel(i+n,j+h));
                            GreenS += qGreen(tmpDst.pixel(i+n,j+h));
                            BlueS += qBlue(tmpDst.pixel(i+n,j+h));
                            Hue+=color1.hue();
                            Saturation+=color1.saturation();
                            Value+=color1.value();
                            D1++;
                        }
                                if(n==0&&h==0)
                                {
                                    tmpDst.setPixel(i+n,j+h,qRgb(128,0,128));
                                    Xc=i; Yc=j;
                                }
                            }
                        }
                    }
            //int Red = RedS/Stochki;
            //int Green = GreenS/Stochki;
            //int Blue = BlueS/Stochki;
            // Xc, Yc- центр, Stochki- площадь.
            // int Hs=Hue/Stochki;
            // int Ss=Saturation/Stochki;
            // int Vs=Value/Stochku;
            //  if(D2>D1 D2-Dmax else D1=Dmax) Analod for Dmin D2=D1; D1=0;
            /*for (int n=-3;n<3;n++)
                for (int h=-3;h<3;h++)
            {
                int Sr = qRed(tmpDst.pixel(i+n,j+h));
                int SKOred= Red-Sr;
                SKOred += SKOred^2;
                int SkoRed= SKOred/Stochki;
            }*/ //Аналогично для остальных параметров
            // Вопросы Периметр пунуты 7-9

            gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::FoundAreas(int i, int j)
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
   int fda=+5;

            QColor color = tmpSrc.pixelColor(i,j);
            int Yar = color.lightness();
            int red = qRed(tmpSrc.pixel(i,j));
            int green = qGreen(tmpSrc.pixel(i,j));
            int blue = qBlue(tmpSrc.pixel(i,j));

            bool UpPixelIsCol = false;
            int redU = qRed(tmpSrc.pixel(i,j+1));
            int blueU = qBlue(tmpSrc.pixel(i,j+1));
            int greenU = qGreen(tmpSrc.pixel(i,j+1));
            QColor colorUp = tmpSrc.pixelColor(i,j+1);
            int YarUp = colorUp.lightness();
            if((redU<red ) && (blueU<blue ) && (greenU<green ) && (YarUp<Yar+fda)) {
                UpPixelIsCol=true;
                int Yarup=YarUp-Yar;
            }

            bool RightPixelIsCol = false;
            int redR = qRed(tmpSrc.pixel(i+1,j));
            int blueR = qBlue(tmpSrc.pixel(i+1,j));
            int greenR = qGreen(tmpSrc.pixel(i+1,j));
            QColor colorRight = tmpSrc.pixelColor(i+1,j);
            int YarRight = colorUp.lightness();
            if((redR<red ) && (blueR<blue ) && (greenR<green) && (YarRight<Yar+fda)) {
                RightPixelIsCol=true;
                int Yarright =YarRight-Yar;
            }

            bool DownPixelIsCol = false;
            int redD = qRed(tmpSrc.pixel(i,j-1));
            int blueD = qBlue(tmpSrc.pixel(i,j-1));
            int greenD = qGreen(tmpSrc.pixel(i,j-1));
            QColor colorDown = tmpSrc.pixelColor(i,j-1);
            int YarDown = colorUp.lightness();
            if(( redD<red) && ( blueD<blue) && ( greenD<green) && (YarDown<Yar+fda)){
                DownPixelIsCol=true;
                int Yardown=YarDown-Yar;
            }

            bool LeftPixelIsCol = false;
            int redL = qRed(tmpSrc.pixel(i-1,j));
            int blueL = qBlue(tmpSrc.pixel(i-1,j));
            int greenL = qGreen(tmpSrc.pixel(i-1,j));
            QColor colorLeft = tmpSrc.pixelColor(i-1,j);
            int YarLeft = colorUp.lightness();
            if(( redL<red) && ( blueL<blue) && ( greenL<green) && (YarLeft<Yar+fda)){
                LeftPixelIsCol=true;
                int Yarleft=YarLeft-Yar;
            }

            bool UpRightPixelIsCol = false;
            int redUR = qRed(tmpSrc.pixel(i+1,j+1));
            int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
            int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
            QColor colorUR = tmpSrc.pixelColor(i+1,j+1);
            int YarUR = colorUp.lightness();
            if(( redUR<red) && ( blueUR<blue) && ( greenUR<green) && (YarUR<Yar+fda)){
                UpRightPixelIsCol=true;
                int Yarur=YarUR-Yar;
            }

            bool UpLeftPixelIsCol = false;
            int redUL = qRed(tmpSrc.pixel(i-1,j+1));
            int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
            int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
            QColor colorUL = tmpSrc.pixelColor(i-1,j+1);
            int YarUL = colorUp.lightness();
            if((redUL<red) && ( blueUL<blue) && ( greenUL<green) && (YarUL<Yar+fda)){
                UpLeftPixelIsCol=true;
                int Yarul=YarUL-Yar;
            }

            bool DownRightPixelIsCol = false;
            int redDR = qRed(tmpSrc.pixel(i+1,j-1));
            int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
            int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
            QColor colorDR = tmpSrc.pixelColor(i+1,j-1);
            int YarDR = colorUp.lightness();
            if(( redDR<red) && ( blueDR<blue) && ( greenDR<green) && (YarDR<Yar+fda)){
                DownRightPixelIsCol=true;
                int Yardr=YarDR-Yar;
            }

            bool DownLeftPixelIsCol = false;
            int redDL = qRed(tmpSrc.pixel(i-1,j-1));
            int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
            int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
            QColor colorDL = tmpSrc.pixelColor(i-1,j-1);
            int YarDL = colorUp.lightness();
            if(( redDL<red) && ( blueDL<blue) && ( greenDL<green) && (YarDL<Yar+fda)){
                DownLeftPixelIsCol=true;
                int Yardl=YarDL-Yar;
            }

            if ((UpPixelIsCol==true && RightPixelIsCol==true && DownPixelIsCol==true && LeftPixelIsCol==true &&
                    UpRightPixelIsCol==true && UpLeftPixelIsCol==true && DownLeftPixelIsCol==true && DownRightPixelIsCol==true)){

                tmpDst.setPixel(i,j,qRgb(255,0,0));

                for(int n=-4;n<4;n++){
                    for (int h=-4;h<4;h++){


                        int redU = qRed(tmpDst.pixel(i+n,j+h));
                        int blueU = qBlue(tmpDst.pixel(i+n,j+h));
                        int greenU = qGreen(tmpDst.pixel(i+n,j+h));
                        if( redU>60 && blueU>45 && greenU>45)
                        {
                            tmpDst.setPixel(i+n,j+h,qRgb(0,0,255));
                        }
                        QColor color = tmpSrc.pixelColor(i,j);
                        QColor color1 = tmpSrc.pixelColor(i+n,j+h);
                        int Yar=color.lightness();
                        int Yar1=color1.lightness();

                             if (Yar<Yar1 || Yar==Yar1 )
                             {
                                  tmpDst.setPixel(i+n,j+h,qRgb(0,0,255));
                             }
                                if(n==0&&h==0)
                                {
                                    tmpDst.setPixel(i+n,j+h,qRgb(0,0,255));
                                }
                                if(redU==0 && blueU==255 && greenU==255 )
                                {
                                    tmpDst.setPixel(i+n,j+h,qRgb(0,255,255));
                                }

                            }
                        }
                    }
            gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::ObrabotkaBlye()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
   // on_pushButton_19_clicked();
    set <int> coordinatex;
    set <int> coordinatey;

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
              if(redU==0 && blueU==255 && greenU==0){
                  UpPixelIsWhite=true;
              }

              bool RightPixelIsWhite = false;
              int redR = qRed(tmpSrc.pixel(i+1,j));
              int blueR = qBlue(tmpSrc.pixel(i+1,j));
              int greenR = qGreen(tmpSrc.pixel(i+1,j));
              if(redR==0 && blueR==255 && greenR==0){
                  RightPixelIsWhite=true;
              }

              bool DownPixelIsWhite = false;
              int redD = qRed(tmpSrc.pixel(i,j-1));
              int blueD = qBlue(tmpSrc.pixel(i,j-1));
              int greenD = qGreen(tmpSrc.pixel(i,j-1));
              if(redD==0 && blueD==255 && greenD==0){
                  DownPixelIsWhite=true;
              }

              bool LeftPixelIsWhite = false;
              int redL = qRed(tmpSrc.pixel(i-1,j));
              int blueL = qBlue(tmpSrc.pixel(i-1,j));
              int greenL = qGreen(tmpSrc.pixel(i-1,j));
              if(redL==0 && blueL==255 && greenL==0){
                  LeftPixelIsWhite=true;
              }

              bool UpRightPixelIsWhite = false;
              int redUR = qRed(tmpSrc.pixel(i+1,j+1));
              int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
              int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
              if(redUR==0 && blueUR==255 && greenUR==0){
                  UpRightPixelIsWhite=true;
              }

              bool UpLeftPixelIsWhite = false;
              int redUL = qRed(tmpSrc.pixel(i-1,j+1));
              int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
              int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
              if(redUL==0 && blueUL==255 && greenUL==0){
                  UpLeftPixelIsWhite=true;
              }

              bool DownRightPixelIsWhite = false;
              int redDR = qRed(tmpSrc.pixel(i+1,j-1));
              int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
              int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
              if(redDR==0 && blueDR==255 && greenDR==0){
                  DownRightPixelIsWhite=true;
              }

              bool DownLeftPixelIsWhite = false;
              int redDL = qRed(tmpSrc.pixel(i-1,j-1));
              int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
              int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
              if(redDL==0 && blueDL==255 && greenDL==0){
                  DownLeftPixelIsWhite=true;
              }

              if ((red!=0 && blue!=255 && green!=0) && (red!=0 && blue!=255 && green!=255) && (UpPixelIsWhite==true || RightPixelIsWhite==true || DownPixelIsWhite==true || LeftPixelIsWhite==true ||
                      UpRightPixelIsWhite==true || UpLeftPixelIsWhite==true || DownLeftPixelIsWhite==true || DownRightPixelIsWhite==true)){
                  tmpDst.setPixel(i,j,qRgb(0,255,0));
              }
          }
      }

  gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::OnrabotkaYellow()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
   // on_pushButton_19_clicked();

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
              if(redU==255 && blueU==0 && greenU==255){
                  UpPixelIsWhite=true;
              }

              bool RightPixelIsWhite = false;
              int redR = qRed(tmpSrc.pixel(i+1,j));
              int blueR = qBlue(tmpSrc.pixel(i+1,j));
              int greenR = qGreen(tmpSrc.pixel(i+1,j));
              if(redR==255 && blueR==0 && greenR==255){
                  RightPixelIsWhite=true;
              }

              bool DownPixelIsWhite = false;
              int redD = qRed(tmpSrc.pixel(i,j-1));
              int blueD = qBlue(tmpSrc.pixel(i,j-1));
              int greenD = qGreen(tmpSrc.pixel(i,j-1));
              if(redD==255 && blueD==0 && greenD==255){
                  DownPixelIsWhite=true;
              }

              bool LeftPixelIsWhite = false;
              int redL = qRed(tmpSrc.pixel(i-1,j));
              int blueL = qBlue(tmpSrc.pixel(i-1,j));
              int greenL = qGreen(tmpSrc.pixel(i-1,j));
              if(redL==255 && blueL==0 && greenL==255){
                  LeftPixelIsWhite=true;
              }

              bool UpRightPixelIsWhite = false;
              int redUR = qRed(tmpSrc.pixel(i+1,j+1));
              int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
              int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
              if(redUR==255 && blueUR==0 && greenUR==255){
                  UpRightPixelIsWhite=true;
              }

              bool UpLeftPixelIsWhite = false;
              int redUL = qRed(tmpSrc.pixel(i-1,j+1));
              int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
              int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
              if(redUL==255 && blueUL==0 && greenUL==255){
                  UpLeftPixelIsWhite=true;
              }

              bool DownRightPixelIsWhite = false;
              int redDR = qRed(tmpSrc.pixel(i+1,j-1));
              int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
              int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
              if(redDR==255 && blueDR==0 && greenDR==255){
                  DownRightPixelIsWhite=true;
              }

              bool DownLeftPixelIsWhite = false;
              int redDL = qRed(tmpSrc.pixel(i-1,j-1));
              int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
              int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
              if(redDL==255 && blueDL==0 && greenDL==255){
                  DownLeftPixelIsWhite=true;
              }

              if ((red!=255 && blue!=0 && green!=255) && (red!=0 && blue!=255 && green!=255) && (UpPixelIsWhite==true || RightPixelIsWhite==true || DownPixelIsWhite==true || LeftPixelIsWhite==true ||
                      UpRightPixelIsWhite==true || UpLeftPixelIsWhite==true || DownLeftPixelIsWhite==true || DownRightPixelIsWhite==true)){
                  tmpDst.setPixel(i,j,qRgb(255,0,0));
              }
          }
      }

      gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::ObrabotkaDBlue()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
   // on_pushButton_19_clicked();

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
              if(redU==0 && blueU==200 && greenU==0){
                  UpPixelIsWhite=true;
              }

              bool RightPixelIsWhite = false;
              int redR = qRed(tmpSrc.pixel(i+1,j));
              int blueR = qBlue(tmpSrc.pixel(i+1,j));
              int greenR = qGreen(tmpSrc.pixel(i+1,j));
              if(redR==0 && blueR==200 && greenR==0){
                  RightPixelIsWhite=true;
              }

              bool DownPixelIsWhite = false;
              int redD = qRed(tmpSrc.pixel(i,j-1));
              int blueD = qBlue(tmpSrc.pixel(i,j-1));
              int greenD = qGreen(tmpSrc.pixel(i,j-1));
              if(redD==0 && blueD==200 && greenD==0){
                  DownPixelIsWhite=true;
              }

              bool LeftPixelIsWhite = false;
              int redL = qRed(tmpSrc.pixel(i-1,j));
              int blueL = qBlue(tmpSrc.pixel(i-1,j));
              int greenL = qGreen(tmpSrc.pixel(i-1,j));
              if(redL==0 && blueL==200 && greenL==0){
                  LeftPixelIsWhite=true;
              }

              bool UpRightPixelIsWhite = false;
              int redUR = qRed(tmpSrc.pixel(i+1,j+1));
              int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
              int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
              if(redUR==0 && blueUR==200 && greenUR==0){
                  UpRightPixelIsWhite=true;
              }

              bool UpLeftPixelIsWhite = false;
              int redUL = qRed(tmpSrc.pixel(i-1,j+1));
              int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
              int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
              if(redUL==0 && blueUL==200 && greenUL==0){
                  UpLeftPixelIsWhite=true;
              }

              bool DownRightPixelIsWhite = false;
              int redDR = qRed(tmpSrc.pixel(i+1,j-1));
              int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
              int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
              if(redDR==0 && blueDR==200 && greenDR==0){
                  DownRightPixelIsWhite=true;
              }

              bool DownLeftPixelIsWhite = false;
              int redDL = qRed(tmpSrc.pixel(i-1,j-1));
              int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
              int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
              if(redDL==0 && blueDL==200 && greenDL==0){
                  DownLeftPixelIsWhite=true;
              }

              if ((red!=0 && blue!=200 && green!=0) && (red!=0 && blue!=255 && green!=255) && (UpPixelIsWhite==true || RightPixelIsWhite==true || DownPixelIsWhite==true || LeftPixelIsWhite==true ||
                      UpRightPixelIsWhite==true || UpLeftPixelIsWhite==true || DownLeftPixelIsWhite==true || DownRightPixelIsWhite==true)){
                  tmpDst.setPixel(i,j,qRgb(0,128,0));
              }
          }
      }

      gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}
void MainWindow::ObrabotkaDDBlue()
{
    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);
   // on_pushButton_19_clicked();

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
              if(redU==0 && blueU==128 && greenU==0){
                  UpPixelIsWhite=true;
              }

              bool RightPixelIsWhite = false;
              int redR = qRed(tmpSrc.pixel(i+1,j));
              int blueR = qBlue(tmpSrc.pixel(i+1,j));
              int greenR = qGreen(tmpSrc.pixel(i+1,j));
              if(redR==0 && blueR==128 && greenR==0){
                  RightPixelIsWhite=true;
              }

              bool DownPixelIsWhite = false;
              int redD = qRed(tmpSrc.pixel(i,j-1));
              int blueD = qBlue(tmpSrc.pixel(i,j-1));
              int greenD = qGreen(tmpSrc.pixel(i,j-1));
              if(redD==0 && blueD==128 && greenD==0){
                  DownPixelIsWhite=true;
              }

              bool LeftPixelIsWhite = false;
              int redL = qRed(tmpSrc.pixel(i-1,j));
              int blueL = qBlue(tmpSrc.pixel(i-1,j));
              int greenL = qGreen(tmpSrc.pixel(i-1,j));
              if(redL==0 && blueL==128 && greenL==0){
                  LeftPixelIsWhite=true;
              }

              bool UpRightPixelIsWhite = false;
              int redUR = qRed(tmpSrc.pixel(i+1,j+1));
              int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
              int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
              if(redUR==0 && blueUR==128 && greenUR==0){
                  UpRightPixelIsWhite=true;
              }

              bool UpLeftPixelIsWhite = false;
              int redUL = qRed(tmpSrc.pixel(i-1,j+1));
              int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
              int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
              if(redUL==0 && blueUL==128 && greenUL==0){
                  UpLeftPixelIsWhite=true;
              }

              bool DownRightPixelIsWhite = false;
              int redDR = qRed(tmpSrc.pixel(i+1,j-1));
              int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
              int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
              if(redDR==0 && blueDR==128 && greenDR==0){
                  DownRightPixelIsWhite=true;
              }

              bool DownLeftPixelIsWhite = false;
              int redDL = qRed(tmpSrc.pixel(i-1,j-1));
              int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
              int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
              if(redDL==0 && blueDL==128 && greenDL==0){
                  DownLeftPixelIsWhite=true;
              }

              if ((red!=0 && blue!=128 && green!=0) && (red!=0 && blue!=255 && green!=255) && (UpPixelIsWhite==true || RightPixelIsWhite==true || DownPixelIsWhite==true || LeftPixelIsWhite==true ||
                      UpRightPixelIsWhite==true || UpLeftPixelIsWhite==true || DownLeftPixelIsWhite==true || DownRightPixelIsWhite==true)){
                  tmpDst.setPixel(i,j,qRgb(0,128,0));
              }
          }
      }

      gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::obrabotYel(int i,int j)
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    int fda=ui->lineEdit_10->text().toInt();

    QColor color = tmpSrc.pixelColor(i,j);
    int Yar = color.lightness();
    int red = qRed(tmpSrc.pixel(i,j));
    int green = qGreen(tmpSrc.pixel(i,j));
    int blue = qBlue(tmpSrc.pixel(i,j));

            bool UpPixelIsCol = false;
            int redU = qRed(tmpSrc.pixel(i,j+1));
            int blueU = qBlue(tmpSrc.pixel(i,j+1));
            int greenU = qGreen(tmpSrc.pixel(i,j+1));
            QColor colorUp = tmpSrc.pixelColor(i,j+1);
            int YarUp = colorUp.lightness();
            if((redU>red ) && (blueU>blue ) && (greenU>green ) && (YarUp>Yar+fda)) {
                UpPixelIsCol=true;

            }

            bool RightPixelIsCol = false;
            int redR = qRed(tmpSrc.pixel(i+1,j));
            int blueR = qBlue(tmpSrc.pixel(i+1,j));
            int greenR = qGreen(tmpSrc.pixel(i+1,j));
            QColor colorRight = tmpSrc.pixelColor(i+1,j);
            int YarRight = colorRight.lightness();
            if((redR>red ) && (blueR>blue ) && (greenR>green) && (YarRight>Yar+fda)) {
                RightPixelIsCol=true;

            }

            bool DownPixelIsCol = false;
            int redD = qRed(tmpSrc.pixel(i,j-1));
            int blueD = qBlue(tmpSrc.pixel(i,j-1));
            int greenD = qGreen(tmpSrc.pixel(i,j-1));
            QColor colorDown = tmpSrc.pixelColor(i,j-1);
            int YarDown = colorDown.lightness();
            if(( redD>red) && ( blueD>blue) && ( greenD>green) && (YarDown>Yar+fda)){
                DownPixelIsCol=true;
            }

            bool LeftPixelIsCol = false;
            int redL = qRed(tmpSrc.pixel(i-1,j));
            int blueL = qBlue(tmpSrc.pixel(i-1,j));
            int greenL = qGreen(tmpSrc.pixel(i-1,j));
            QColor colorLeft = tmpSrc.pixelColor(i-1,j);
            int YarLeft = colorLeft.lightness();
            if(( redL>red) && ( blueL>blue) && ( greenL>green) && (YarLeft>Yar+fda)){
                LeftPixelIsCol=true;
            }

            bool UpRightPixelIsCol = false;
            int redUR = qRed(tmpSrc.pixel(i+1,j+1));
            int blueUR = qBlue(tmpSrc.pixel(i+1,j+1));
            int greenUR = qGreen(tmpSrc.pixel(i+1,j+1));
            QColor colorUR = tmpSrc.pixelColor(i+1,j+1);
            int YarUR = colorUR.lightness();
            if(( redUR>red) && ( blueUR>blue) && ( greenUR>green) && (YarUR>Yar+fda)){
                UpRightPixelIsCol=true;
            }

            bool UpLeftPixelIsCol = false;
            int redUL = qRed(tmpSrc.pixel(i-1,j+1));
            int blueUL = qBlue(tmpSrc.pixel(i-1,j+1));
            int greenUL = qGreen(tmpSrc.pixel(i-1,j+1));
            QColor colorUL = tmpSrc.pixelColor(i-1,j+1);
            int YarUL = colorUL.lightness();
            if((redUL>red) && ( blueUL>blue) && ( greenUL>green) && (YarUL>Yar+fda)){
                UpLeftPixelIsCol=true;
            }

            bool DownRightPixelIsCol = false;
            int redDR = qRed(tmpSrc.pixel(i+1,j-1));
            int blueDR = qBlue(tmpSrc.pixel(i+1,j-1));
            int greenDR = qGreen(tmpSrc.pixel(i+1,j-1));
            QColor colorDR = tmpSrc.pixelColor(i+1,j-1);
            int YarDR = colorDR.lightness();
            if(( redDR>red) && ( blueDR>blue) && ( greenDR>green) && (YarDR>Yar+fda)){
                DownRightPixelIsCol=true;
            }

            bool DownLeftPixelIsCol = false;
            int redDL = qRed(tmpSrc.pixel(i-1,j-1));
            int blueDL = qBlue(tmpSrc.pixel(i-1,j-1));
            int greenDL = qGreen(tmpSrc.pixel(i-1,j-1));
            QColor colorDL = tmpSrc.pixelColor(i-1,j-1);
            int YarDL = colorDL.lightness();
            if(( redDL>red) && ( blueDL>blue) && ( greenDL>green) && (YarDL>Yar+fda)){
                DownLeftPixelIsCol=true;
            }

            if ((UpPixelIsCol==true && RightPixelIsCol==true && DownPixelIsCol==true && LeftPixelIsCol==true &&
                    UpRightPixelIsCol==true && UpLeftPixelIsCol==true && DownLeftPixelIsCol==true && DownRightPixelIsCol==true)){

                tmpDst.setPixel(i,j,qRgb(255,255,0)); tmpDst.setPixel(i+1,j,qRgb(255,255,0)); tmpDst.setPixel(i,j+1,qRgb(255,255,0)); tmpDst.setPixel(i-1,j,qRgb(255,255,0));
                tmpDst.setPixel(i,j-1,qRgb(255,255,0)); tmpDst.setPixel(i+1,j+1,qRgb(255,255,0));tmpDst.setPixel(i+1,j-1,qRgb(255,255,0)); tmpDst.setPixel(i-1,j+1,qRgb(255,255,0));
                tmpDst.setPixel(i-1,j-1,qRgb(255,255,0));

            for(int n=0;n<3;n++)
            {
                for (int h=0;h<3;h++)
                {

                    int redU = qRed(tmpSrc.pixel(i,j+h));
                    int blueU = qBlue(tmpSrc.pixel(i,j+h));
                    int greenU = qGreen(tmpSrc.pixel(i,j+h));
                    QColor colorUp = tmpSrc.pixelColor(i,j+h);
                    int YarUp = colorUp.lightness();
                    if((redU>red ) && (blueU>blue ) && (greenU>green ) && (YarUp>Yar+fda)) {
                        tmpDst.setPixel(i,j+h,qRgb(255,255,0));
                    }

                    int redR = qRed(tmpSrc.pixel(i+n,j));
                    int blueR = qBlue(tmpSrc.pixel(i+n,j));
                    int greenR = qGreen(tmpSrc.pixel(i+n,j));
                    QColor colorRight = tmpSrc.pixelColor(i+n,j);
                    int YarRight = colorRight.lightness();
                    if((redR>red ) && (blueR>blue ) && (greenR>green) && (YarRight>Yar+fda)) {
                        tmpDst.setPixel(i+n,j,qRgb(255,255,0));
                    }

                    int redD = qRed(tmpSrc.pixel(i,j-h));
                    int blueD = qBlue(tmpSrc.pixel(i,j-h));
                    int greenD = qGreen(tmpSrc.pixel(i,j-h));
                    QColor colorDown = tmpSrc.pixelColor(i,j-h);
                    int YarDown = colorDown.lightness();
                    if(( redD>red) && ( blueD>blue) && ( greenD>green) && (YarDown>Yar+fda)){
                        tmpDst.setPixel(i,j-h,qRgb(255,255,0));
                    }

                    int redL = qRed(tmpSrc.pixel(i-n,j));
                    int blueL = qBlue(tmpSrc.pixel(i-n,j));
                    int greenL = qGreen(tmpSrc.pixel(i-n,j));
                    QColor colorLeft = tmpSrc.pixelColor(i-n,j);
                    int YarLeft = colorLeft.lightness();
                    if(( redL>red) && ( blueL>blue) && ( greenL>green) && (YarLeft>Yar+fda)){
                        tmpDst.setPixel(i-n,j,qRgb(255,255,0));
                    }

                    int redUR = qRed(tmpSrc.pixel(i+n,j+h));
                    int blueUR = qBlue(tmpSrc.pixel(i+n,j+h));
                    int greenUR = qGreen(tmpSrc.pixel(i+n,j+h));
                    QColor colorUR = tmpSrc.pixelColor(i+n,j+h);
                    int YarUR = colorUR.lightness();
                    if(( redUR>red) && ( blueUR>blue) && ( greenUR>green) && (YarUR>Yar+fda)){
                        tmpDst.setPixel(i+n,j+h,qRgb(255,255,0));
                    }


                    int redUL = qRed(tmpSrc.pixel(i-n,j+h));
                    int blueUL = qBlue(tmpSrc.pixel(i-n,j+h));
                    int greenUL = qGreen(tmpSrc.pixel(i-n,j+h));
                    QColor colorUL = tmpSrc.pixelColor(i-n,j+h);
                    int YarUL = colorUL.lightness();
                    if((redUL>red) && ( blueUL>blue) && ( greenUL>green) && (YarUL>Yar+fda)){
                        tmpDst.setPixel(i-n,j+h,qRgb(255,255,0));
                    }


                    int redDR = qRed(tmpSrc.pixel(i+n,j-h));
                    int blueDR = qBlue(tmpSrc.pixel(i+n,j-h));
                    int greenDR = qGreen(tmpSrc.pixel(i+n,j-h));
                    QColor colorDR = tmpSrc.pixelColor(i+n,j-h);
                    int YarDR = colorDR.lightness();
                    if(( redDR>red) && ( blueDR>blue) && ( greenDR>green) && (YarDR>Yar+fda)){
                        tmpDst.setPixel(i+n,j-h,qRgb(255,255,0));
                    }


                    int redDL = qRed(tmpSrc.pixel(i-n,j-h));
                    int blueDL = qBlue(tmpSrc.pixel(i-n,j-h));
                    int greenDL = qGreen(tmpSrc.pixel(i-n,j-h));
                    QColor colorDL = tmpSrc.pixelColor(i-n,j-h);
                    int YarDL = colorDL.lightness();
                    if(( redDL>red) && ( blueDL>blue) && ( greenDL>green) && (YarDL>Yar+fda)){
                        tmpDst.setPixel(i-n,j-h,qRgb(255,255,0));
                    }
                }
            }

            }


                /*for(int n=-4;n<4;n++){
                    for (int h=-4;h<4;h++){
                        int redU = qRed(tmpDst.pixel(i+n,j+h));
                        int blueU = qBlue(tmpDst.pixel(i+n,j+h));
                        int greenU = qGreen(tmpDst.pixel(i+n,j+h));
                        if( redU<60 && blueU<45 && greenU<45)
                        {
                            tmpDst.setPixel(i+n,j+h,qRgb(255,255,0));
                        }
                        QColor color = tmpSrc.pixelColor(i,j);
                        QColor color1 = tmpSrc.pixelColor(i+n,j+h);
                        int Yar=color.lightness();
                        int Yar1=color1.lightness();

                             if (Yar<Yar1 || Yar==Yar1 )
                             {
                                  tmpDst.setPixel(i+n,j+h,qRgb(255,255,0));
                             }
                                if(n==0&&h==0)
                                {
                                    tmpDst.setPixel(i+n,j+h,qRgb(255,0,0));
                                }
                            }
                        }
                    }*/
            gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::on_findAreas_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    IplImage * pere;
    pere = QImage2IplImage(&tmpDst);
    CentrMass * CM = new CentrMass();

    cv::Point2f center = CM->Simmetr(*pere);
    int Xc = center.x;
    int Yc = center.y;
    Yc = Yc + 30;
    qDebug() << "Xc=" <<Xc<<"Yc="<<Yc;
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
             redPol = qRed(tmpSrc.pixel(wigthX,heightY));
             greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
             bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
              FoundAreas(wigthX,heightY);
             wigthX++;
         }

         redPol = qRed(tmpSrc.pixel(1,1));
         greenPol = qGreen(tmpSrc.pixel(1,1));
         bluePol = qBlue(tmpSrc.pixel(1,1));

         while(redOtr!=0 && blueOtr != 255 && greenOtr != 255)
         {
             redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
             blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
             greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));
              FoundAreas(xOtr,heightY);
             xOtr--;

         }

         redOtr = qRed(tmpSrc.pixel(1,1));
         blueOtr = qBlue(tmpSrc.pixel(1,1));
         greenOtr = qGreen(tmpSrc.pixel(1,1));


         tmpDst.setPixel(Xc,heightY,qRgb(255,0,255));
         xOtr = Xc;
         wigthX = Xc;

          heightY++;
         red = qRed(tmpSrc.pixel(Xc,heightY));
         blue = qBlue(tmpSrc.pixel(Xc,heightY));
         green = qGreen(tmpSrc.pixel(Xc,heightY));

    }

    red = qRed(tmpSrc.pixel(Xc,Yc));
    blue = qBlue(tmpSrc.pixel(Xc,Yc));
    green = qGreen(tmpSrc.pixel(Xc,Yc));

    heightY = Yc;
    while (red!=0 && green!=255 && blue!=255)
    {
         while(redPol!=0 && bluePol!=255 && greenPol!=255)
         {
             redPol = qRed(tmpSrc.pixel(wigthX,heightY));
             greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
             bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
              FoundAreas(wigthX,heightY);
             wigthX++;
         }

         redPol = qRed(tmpSrc.pixel(1,1));
         greenPol = qGreen(tmpSrc.pixel(1,1));
         bluePol = qBlue(tmpSrc.pixel(1,1));

         while(redOtr!=0 && blueOtr != 255 && greenOtr != 255)
         {
             redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
             blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
             greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));
              FoundAreas(xOtr,heightY);
             xOtr--;
         }

         redOtr = qRed(tmpSrc.pixel(1,1));
         blueOtr = qBlue(tmpSrc.pixel(1,1));
         greenOtr = qGreen(tmpSrc.pixel(1,1));


         tmpDst.setPixel(Xc,heightY,qRgb(255,0,255));
         xOtr = Xc;
         wigthX = Xc;
          heightY--;
         red = qRed(tmpSrc.pixel(Xc,heightY));
         blue = qBlue(tmpSrc.pixel(Xc,heightY));
         green = qGreen(tmpSrc.pixel(Xc,heightY));
         //Obrabotka();
         ObrabotkaBlye();
         FoundBlue();
}
}

void MainWindow::on_pushButton_42_clicked()
{
    Original *Oty = new Original;

    QImage tmpSrc = Oty->gpiOriginal->pixmap().toImage();
    gpiDst->setPixmap(QPixmap::fromImage(tmpSrc));
}

void MainWindow::Peredaxa(QImage fga)
{
    itemAnz = new QGraphicsPixmapItem(QPixmap::fromImage(fga));
    sciAnz = new QGraphicsScene();
    sciAnz->addItem(itemAnz);
    ui->dstGrView->setScene(sciAnz);
}

void MainWindow::FoundYellow()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    Original *jgf = new Original;


    for (int i=0; i<tmpSrc.width();i++)
    {
        for(int j=0; j<tmpSrc.height();j++)
        {
            int Red = qRed(tmpSrc.pixel(i,j));
            int Green = qGreen(tmpSrc.pixel(i,j));
            int Blue = qBlue(tmpSrc.pixel(i,j));

            if(Red == 255 && Green==255 && Blue==0)
            {
                tie(Red,Green,Blue)= jgf->GiveOriginal(i,j);
                tmpDst.setPixel(i,j,qRgb(Red,Green,Blue));
            }
        }
    }
    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::FoundBlue()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    Original *jgf = new Original;


    for (int i=0; i<tmpSrc.width();i++)
    {
        for(int j=0; j<tmpSrc.height();j++)
        {
            int Red = qRed(tmpSrc.pixel(i,j));
            int Green = qGreen(tmpSrc.pixel(i,j));
            int Blue = qBlue(tmpSrc.pixel(i,j));

            if(Red == 0 && Green==0 && Blue==255)
            {
                tie(Red,Green,Blue)= jgf->GiveOriginal(i,j);
                tmpDst.setPixel(i,j,qRgb(Red,Green,Blue));
            }
        }
    }
    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::FoundDBlue()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    Original *jgf = new Original;


    for (int i=0; i<tmpSrc.width();i++)
    {
        for(int j=0; j<tmpSrc.height();j++)
        {
            int Red = qRed(tmpSrc.pixel(i,j));
            int Green = qGreen(tmpSrc.pixel(i,j));
            int Blue = qBlue(tmpSrc.pixel(i,j));

            if(Red == 0 && Green==0 && Blue==200)
            {
                tie(Red,Green,Blue)= jgf->GiveOriginal(i,j);
                tmpDst.setPixel(i,j,qRgb(Red,Green,Blue));
            }
        }
    }
    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::FoundDDblue()
{
    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    Original *jgf = new Original;


    for (int i=0; i<tmpSrc.width();i++)
    {
        for(int j=0; j<tmpSrc.height();j++)
        {
            int Red = qRed(tmpSrc.pixel(i,j));
            int Green = qGreen(tmpSrc.pixel(i,j));
            int Blue = qBlue(tmpSrc.pixel(i,j));

            if(Red == 0 && Green==0 && Blue==128)
            {
                tie(Red,Green,Blue)= jgf->GiveOriginal(i,j);
                tmpDst.setPixel(i,j,qRgb(Red,Green,Blue));
            }
        }
    }
    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}

QString MainWindow::Pertfunc()
{
    return pert;
}

void MainWindow::on_FDO_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    for (int i=0; i<tmpSrc.width();i++)
    {
        for(int j=0; j<tmpSrc.height();j++)
        {
            int Red = qRed(tmpSrc.pixel(i,j));
            int Green = qGreen(tmpSrc.pixel(i,j));
            int Blue = qBlue(tmpSrc.pixel(i,j));

            if(Red< 64 && Green<32 && Blue<32)
            {
                tmpDst.setPixel(i,j,qRgb(0,0,128));
            }
        }
    }
    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
   // ObrabotkaDDBlue();
   // FoundDDblue();
}

void MainWindow::on_Nf_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    gtr = new anizo();
    gtr -> Peredaxa(tmpDst);
    gtr->show();
}


void MainWindow::on_testyel_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    IplImage * pere;
    pere = QImage2IplImage(&tmpDst);
    CentrMass * CM = new CentrMass();

    cv::Point2f center = CM->Simmetr(*pere);
    int Xc = center.x;
    int Yc = center.y;
    Yc = Yc + 30;
    qDebug() << "Xc=" <<Xc<<"Yc="<<Yc;
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
             redPol = qRed(tmpSrc.pixel(wigthX,heightY));
             greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
             bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
              obrabotYel(wigthX,heightY);
             wigthX++;
         }

         redPol = qRed(tmpSrc.pixel(1,1));
         greenPol = qGreen(tmpSrc.pixel(1,1));
         bluePol = qBlue(tmpSrc.pixel(1,1));

         while(redOtr!=0 && blueOtr != 255 && greenOtr != 255)
         {
             redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
             blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
             greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));
              obrabotYel(xOtr,heightY);
             xOtr--;

         }

         redOtr = qRed(tmpSrc.pixel(1,1));
         blueOtr = qBlue(tmpSrc.pixel(1,1));
         greenOtr = qGreen(tmpSrc.pixel(1,1));


         tmpDst.setPixel(Xc,heightY,qRgb(255,0,255));
         xOtr = Xc;
         wigthX = Xc;

          heightY++;
         red = qRed(tmpSrc.pixel(Xc,heightY));
         blue = qBlue(tmpSrc.pixel(Xc,heightY));
         green = qGreen(tmpSrc.pixel(Xc,heightY));

    }

    red = qRed(tmpSrc.pixel(Xc,Yc));
    blue = qBlue(tmpSrc.pixel(Xc,Yc));
    green = qGreen(tmpSrc.pixel(Xc,Yc));

    heightY = Yc;
    while (red!=0 && green!=255 && blue!=255)
    {
         while(redPol!=0 && bluePol!=255 && greenPol!=255)
         {
             redPol = qRed(tmpSrc.pixel(wigthX,heightY));
             greenPol = qGreen(tmpSrc.pixel(wigthX,heightY));
             bluePol = qBlue(tmpSrc.pixel(wigthX,heightY));
              obrabotYel(wigthX,heightY);
             wigthX++;
         }

         redPol = qRed(tmpSrc.pixel(1,1));
         greenPol = qGreen(tmpSrc.pixel(1,1));
         bluePol = qBlue(tmpSrc.pixel(1,1));

         while(redOtr!=0 && blueOtr != 255 && greenOtr != 255)
         {
             redOtr = qRed(tmpSrc.pixel(xOtr,heightY));
             blueOtr = qBlue(tmpSrc.pixel(xOtr,heightY));
             greenOtr = qGreen(tmpSrc.pixel(xOtr,heightY));
              obrabotYel(xOtr,heightY);
             xOtr--;
         }

         redOtr = qRed(tmpSrc.pixel(1,1));
         blueOtr = qBlue(tmpSrc.pixel(1,1));
         greenOtr = qGreen(tmpSrc.pixel(1,1));


         tmpDst.setPixel(Xc,heightY,qRgb(255,0,255));
         xOtr = Xc;
         wigthX = Xc;
          heightY--;
         red = qRed(tmpSrc.pixel(Xc,heightY));
         blue = qBlue(tmpSrc.pixel(Xc,heightY));
         green = qGreen(tmpSrc.pixel(Xc,heightY));
         //Obrabotka();
         //OnrabotkaYellow();
         //FoundYellow();
}
}

void MainWindow::on_pushButton_43_clicked()
{
    ObrabotkaDDBlue();
}

void MainWindow::on_pushButton_44_clicked()
{
    FoundDDblue();
}

void MainWindow::on_pushButton_45_clicked()
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    tmpDst.save("fast-but-uncompressed.png", "png", 100);

}

