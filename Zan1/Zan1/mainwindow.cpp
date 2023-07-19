#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->ScrView->setScene(&scSrc);
    ui->DstView->setScene(&scDst);
}

MainWindow::~MainWindow()
{
    delete ui;

}


void MainWindow::on_MedianFilt_clicked()
{
    QImage tmpSrc = gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    int znachbin = ui->lineEdit->text().toInt();
    int znb = znachbin+1;

    QVector <QRgb> vec;
    int h,w;

    for(h= znb; h<tmpSrc.height()-znb;h++)
    {
        for (w=znb; w<tmpSrc.width()-znb;w++)
        {
            for(int i=-znachbin;i<znb;i++){
                for(int j=-znachbin;j<znb;j++){
                    vec.append(tmpSrc.pixel(w+i,h+j));
                }
           }
            std::sort(vec.begin(),vec.end());
            tmpDst.setPixel(w,h,vec.at(znachbin*znachbin*4));
            vec.clear();
        }
    }
    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    for(int y=0; y<tmpSrc.height();++y){
        for(int x=0; x<tmpSrc.width();++x)
        {
            if(qRed(tmpSrc.pixel(x,y))>value)
                tmpDst.setPixel(x,y,qRgb(255,255,255));
                else
                tmpDst.setPixel(x,y,qRgb(0,0,0));
        }
    }
    ui->lineEdit_2->setText(QString::number(value));
    gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
}

void MainWindow::on_actionOpenImage_triggered()
{
    QString fp = QFileDialog::getOpenFileName();

    gpiSrc = new mygraphicsitem(QPixmap(fp));
    gpiDst = new mygraphicsitem(QPixmap(fp));

    scSrc.addItem(gpiSrc);
    scDst.addItem(gpiDst);

    QStringList filenames=QFileDialog::getOpenFileNames(this,tr("Open image"),"/home",tr("*.*"));//последняя переменная обозначает фильтр форматов. Здесь можно вручную сузить поиск только по файлам с определенным расширением.
    for(QStringList::Iterator it=filenames.begin();
                it!=filenames.end();
                it++)
          {
               ui->listWidget->addItem(*it);//загрузка элемента в listWidget главной формы
          }
}


void MainWindow::on_WorkImage_clicked()
{
    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);
   // on_pushButton_19_clicked();
;

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
                    peredacha(i,j);
              }
          }
      }


  gpiDst->setPixmap(QPixmap::fromImage(tmpDst));

}

void MainWindow::peredacha(int width, int height)
{
    QImage tmpSrc =gpiSrc->pixmap().toImage();
    QImage tmpDst(tmpSrc);

    for (int i=0;i<tmpSrc.width();i++){
                 for (int j=0;j<tmpSrc.height();j++){
                     if(i==width && j==height)
                     {
                         tmpDst.setPixel(i,j,qRgb(0,255,9));
                     }
                 }
    }
    gpiSrc->setPixmap(QPixmap::fromImage(tmpDst));
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

void MainWindow::on_pushButton_clicked()
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


void MainWindow::on_pushButton_2_clicked()
{
    ui->ScrView->scale(1.1,1.1);
    ui->DstView->scale(1.1,1.1);
}


void MainWindow::on_pushButton_3_clicked()
{
    ui->ScrView->scale(1/1.1,1/1.1);
    ui->DstView->scale(1/1.1,1/1.1);
}


void MainWindow::on_pushButton_4_clicked()
{
    int schS=0,schN=0,SchSb=0,SchNb=0;

    QImage tmpSrc =gpiDst->pixmap().toImage();
    QImage tmpDst(tmpSrc);
    //connect(sliderA, SIGNAL(valueChanged(int)), lcdAL, SLOT(display(int)));
    float znachsim = 0.9;
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

    }
    else {
        ui->textBrowser->append("Симметрия по оси х");
        ui->textBrowser->append("Симметрия по икс = " + QString::number(Simmx));
        ui->textBrowser->append("Значение симметрии = " + QString::number(znachsim));

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

    }
    else {
        ui->textBrowser->append("Симметрия по оси у");
        ui->textBrowser->append("Сумма значений по у = "+ QString::number(Simmy));
        ui->textBrowser->append("Значение симметрии = " +QString::number(znachsim) );
         ui->textBrowser->append("Вывод данных по расчету SchN = " + QString::number(schN) + "Количество точек У = " + QString::number(KolvoTochY));

    }
    float Srsim = (Simmy+Simmx)/2;
    if(Srsim<0.9)
    {
        ui->textBrowser->append("Ассиметрия");
        ui->textBrowser->append("Сумма Совпадений = " + QString::number(Srsim));
        ui->textBrowser->append("Количество точек = " + QString::number(KolvoTochS));


    }
    else
    {

        ui->textBrowser->append("Симметрия");
        ui->textBrowser->append("Сумма Совпадений = " + QString::number(Srsim));
        ui->textBrowser->append("Количесство точек = " + QString::number(KolvoToch));

    }
}



void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString fp = item->text();

    qDebug() << "FP" << fp;

    gpiSrc = new mygraphicsitem(QPixmap(fp));
    gpiDst = new mygraphicsitem(QPixmap(fp));

    scSrc.addItem(gpiSrc);
    scDst.addItem(gpiDst);
}


void MainWindow::on_pushButton_5_clicked()
{
    QString filename = QFileDialog::getOpenFileName();

    std::string temp(filename.toStdString());

    const char* fn = temp.c_str();

    image = cvLoadImage(fn,1);

    assert (image !=0);

    gray = cvCreateImage(cvGetSize(image), IPL_DEPTH_8U, 1);
    bin = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );

    dsp = cvCloneImage(image);

    cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("binary",CV_WINDOW_AUTOSIZE);
    cvNamedWindow("contours",CV_WINDOW_AUTOSIZE);

    cvCvtColor(image,gray, CV_RGB2GRAY);

    cvInRangeS(gray, cvScalar(40), cvScalar(150), bin);

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours =0;

    int contoursCont = cvFindContours(bin, storage, &contours, sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

    for(CvSeq* seq0=contours; seq0!=0;seq0 = seq0->h_next)
        cvDrawContours(dsp,seq0,CV_RGB(255,0,0), CV_RGB(0,250,0),0,1,8);

    cvShowImage("original",image);
    cvShowImage("binary",bin);
    cvShowImage("contours",dsp);

    cvWaitKey(0);

    cvReleaseImage(&image);
    cvReleaseImage(&gray);
    cvReleaseImage(&bin);
    cvReleaseImage(&dsp);

    cvDestroyAllWindows();



}

