#include "original.h"
#include "ui_original.h"
#include "mainwindow.h"

QImage Hgf;

Original::Original(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Original)
{
    ui->setupUi(this);

    ui->Origin->setScene(&scOri);
}

Original::~Original()
{
    delete ui;
}

void Original::PolOriginal(QImage fgaO)
{
    MainWindow *w = new MainWindow;
    gpiOriginal = new MyGraphicsItem(QPixmap(w->Pertfunc()));

    scOri.addItem(gpiOriginal);
    Hgf = gpiOriginal->pixmap().toImage();
    //tmpSrc=fgaO;
    //itemAnzO = new QGraphicsPixmapItem(QPixmap::fromImage(fgaO));
    //sciAnzO = new QGraphicsScene();
    //sciAnzO->addItem(itemAnzO);
    //ui->Origin->setScene(sciAnzO);
}

std::tuple<int, int, int> Original::GiveOriginal(int x, int y)
{
    //QImage Hgf = gpiOriginal->pixmap().toImage();

    int Red=qRed(Hgf.pixel(1,1));
    int Green = qGreen(Hgf.pixel(1,1));
    int Blue = qBlue(Hgf.pixel(1,1));

    for (int i=0;i<Hgf.width();i++)
    {
        for (int j=0;j<Hgf.height();j++)
        {
            if (i==x && j==y)
            {
                 Red=qRed(Hgf.pixel(i,j));
                 Green = qGreen(Hgf.pixel(i,j));
                 Blue = qBlue(Hgf.pixel(i,j));
            }

        }
    }
    return std::make_tuple(Red,Green,Blue);
}

void Original::on_Pribl_clicked()
{
     ui->Origin->scale(1.1,1.1);
}

void Original::on_Otdal_clicked()
{
    ui->Origin->scale(1/1.1,1/1.1);
}

void Original::DiveOriginal()
{
    QImage tmpSrc =gpiOriginal->pixmap().toImage();
    MainWindow * Gmr = new MainWindow ;
    Gmr->Peredaxa(tmpSrc);
}

void Original::on_Original_2_clicked()
{
   QImage tmpSrc =gpiOriginal->pixmap().toImage();
   for(int i=100;i<200;i++)
   {
       for(int j=100;j<200;j++)
       {
           tmpSrc.setPixel(i,j,qRgb(0,0,0));
       }
   }
    gpiOriginal->setPixmap(QPixmap::fromImage(tmpSrc));
}
