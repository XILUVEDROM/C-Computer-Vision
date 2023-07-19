#include "centrmass.h"

CentrMass::CentrMass()
{

}

cv::Point2f CentrMass::Simmetr(IplImage s3)
{

    //
           // находим центр масс
           //

           int Xc = 0;
           int Yc = 0;
           int counter = 0; // счётчик числа белых пикселей
           cv::Point2f center(0,0);

           // пробегаемся по пикселям изображения
           for(int y=0; y<s3.height; y++)
           {
                   uchar* ptr = (uchar*) (s3.imageData + y * s3.widthStep);
                   for(int x=0; x<s3.width; x++)
                   {
                           if( ptr[x]>0 )
                           {
                                   Xc += x;
                                   Yc += y;
                                   counter++;
                           }
                   }
           }

           if(counter!=0)
           {
                   center.x = float(Xc)/counter;
                   center.y = float(Yc)/counter;
           }

           return center;
    /*
     s1 =f1->pixmap().toImage();
     QImage fg(s1);

     for(int y=0; y<s1.height();++y)
         for(int x=0; x<s1.width();++x)
         {
             int r = qRed(s1.pixel(x,y));
             int g = qGreen(s1.pixel(x,y));
             int b = qBlue(s1.pixel(x,y));

             uint i = 0.298*r+0.587*g+0.114*b;

             int newr= 0.298*r;
             int newg= 0.587*g;
             int newb= 0.114*b;

             fg.setPixel(x,y,qRgb(newr,newg,newb));

         }
     f2->setPixmap(QPixmap::fromImage(fg));


     float H=0;
     float G=0;
     float Hg=0;
     int value;
     uint xc,yc;

     unsigned int MaskSumm=0;
     char Mask[5][5] = {{1,1,1,1,1},
                        {1,1,1,1,1},
                        {1,1,1,1,1},
                        {1,1,1,1,1},
                        {1,1,1,1,1}};
     for(int i=0;i<5;i++)
         for(int j=0;j<5;j++)
             MaskSumm+=Mask[i][j];

 for(int x=2; x<s1.width()-2;x++)
     for(int y=2;y<s1.height()-2;y++)
     {
         int sumR=0;int sumG=0;int sumB=0;
             for(int i=0;i<5;i++)
                 for (int j=0;j<5;j++)
                 {
                     sumR+=(Mask[i][j]*qRed(s1.pixel(x+i-2,y+j-2)));
                     sumG+=(Mask[i][j]*qRed(s1.pixel(x+i-2,y+j-2)));
                     sumB+=(Mask[i][j]*qRed(s1.pixel(x+i-2,y+j-2)));
                 }
             fg.setPixel(x,y,qRgb(sumR/MaskSumm,sumG/MaskSumm,sumB/MaskSumm));
     }
     f2->setPixmap(QPixmap::fromImage(fg));

     Hg=pow(2.7,-(5*5+5*5)/2*G);

     H= (5*5+5*5-2*G*G)*Hg/2*3.14*G*G*G*G*G*G*Hg;

     value = sqrt(G*G/2*Hg);

     for(int y=0; y<s1.height();++y)
         for(int x=0; x<s1.width();++x)
         {
             if(H>value)
                 fg.setPixel(x,y,qRgb(255,255,255));
                     else
                 fg.setPixel(x,y,qRgb(0,0,0));
         }
     f2->setPixmap(QPixmap::fromImage(fg));

     for (int i=0;i<s1.width();i++)
     {
         xc+=xc/s1.width();
     }
     for (int i=0;i<s1.height();i++)
     {
         yc+=yc/s1.height();
     }

*/
}






/*
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
                  //for_each(colorh.begin(),colorh.end(),[](const auto& t){/*cout<<t.toStdString().c_str()<<' ';});
                  //for_each(colorh.begin(),colorh.end(),[](const auto& t){if(t=="White"|| t=="Red"||t="Korichevyi"||t=="blue"||t=="black"||t=="s_grey") {Svet++;}});

                  //Svet = svetu.size();
                  //gpiDst->setPixmap(QPixmap::fromImage(tmpDst));
*/
