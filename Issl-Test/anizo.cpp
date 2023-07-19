#include "anizo.h"
#include "ui_anizo.h"
#include "mainwindow.h"

QImage Hgyy;

anizo::anizo(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::anizo)
{
    ui->setupUi(this);
}

float X1=0,X2=0,Y1=0,Y2=0,S1=0,S2=0,S3=0;
float AX1=0,AX2=0,AY1=0,AY2=0, AS1=0, AS2=0, AS3=0;
bool d1=false,d2=false,d3=false,d4=false,d5=false,d6=false,d7=false,d8=false,d9=false,d10=false,d11=false,d12=false,d13=false,d14=false,d15=false,d16=false,d17=false,d18=false;

anizo::~anizo()
{
    delete ui;
}

void anizo::Peredaxa(QImage fga)
{
    itemAnz = new QGraphicsPixmapItem(QPixmap::fromImage(fga));
    sciAnz = new QGraphicsScene();
    sciAnz->addItem(itemAnz);
    ui->gpiAnz->setScene(sciAnz);

    if (X1!=0 && X2!=0)
    {
        ui->textBrowser->append("Ассиметрия по оси х");
        ui->textBrowser->append("Сумма значений по икс = " + QString::number(X1));
        ui->textBrowser->append("Значение симметрии = " + QString::number(X2));
    }
    else {
        ui->textBrowser->append("Симметрия по оси х");
        ui->textBrowser->append("Сумма значений по икс = " + QString::number(AX1));
        ui->textBrowser->append("Значение симметрии = " + QString::number(AX2));
    }

    if(Y1!=0 && Y2!=0)
    {
        ui->textBrowser->append("Ассиметрия по оси Y");
        ui->textBrowser->append("Сумма значений по Игрек = " + QString::number(Y1));
        ui->textBrowser->append("Значение симметрии = " + QString::number(Y2));
    }
    else {
        ui->textBrowser->append("Симметрия по оси y");
        ui->textBrowser->append("Сумма значений по игрек = " + QString::number(AY1));
        ui->textBrowser->append("Значение симметрии = " + QString::number(AY2));
    }

    if (S1!=0 && S2!=0 )
    {
        ui->textBrowser->append("Ассимметрия");
        ui->textBrowser->append("Сумма Совпадений = " + QString::number(S1));
        ui->textBrowser->append("Количесство точек = " + QString::number(S2));
    }
    else {
        ui->textBrowser->append("Симметрия");
        ui->textBrowser->append("Сумма Совпадений = " + QString::number(AS1));
        ui->textBrowser->append("Количесство точек = " + QString::number(AS2));
    }

    ui-> textBrowser->append("Вывод анализа цветов. Вывод найденных цветов");
    if(d1 == true)
    {
        ui->textBrowser->append("Белый");
    }
    if(d2 == true)
    {
        ui->textBrowser->append("Темно красный");
    }
    if(d3 == true)
    {
        ui->textBrowser->append("Красный");
    }
    if(d4 == true)
    {
        ui->textBrowser->append("Темно Зеленый");
    }
    if(d5 == true)
    {
        ui->textBrowser->append("Зеленый");
    }
    if(d6 == true)
    {
        ui->textBrowser->append("Темно Синий");
    }
    if(d7 == true)
    {
        ui->textBrowser->append("Синий");
    }
    if(d8 == true)
    {
        ui->textBrowser->append("Темно Голубой");
    }
    if(d9 == true)
    {
        ui->textBrowser->append("Голубой");
    }
    if(d10 == true)
    {
        ui->textBrowser->append("Темно Пурпурный");
    }
    if(d11 == true)
    {
        ui->textBrowser->append("Пурпурный");
    }
    if(d12 == true)
    {
        ui->textBrowser->append("Темно Жёлтый");
    }
    if(d13 == true)
    {
        ui->textBrowser->append("Жёлтый");
    }
    if(d14 == true)
    {
        ui->textBrowser->append("Темно серый");
    }
    if(d15 == true)
    {
        ui->textBrowser->append("Серый");
    }
    if(d16 == true)
    {
        ui->textBrowser->append("Светло серый");
    }
    if(d17 == true)
    {
        ui->textBrowser->append("Черный");
    }
    if(d18 == true)
    {
        ui->textBrowser->append("Розовый");
    }
}

MyGraphicsItem *anizo::PolOrig(MyGraphicsItem *pereg)
{
    return pereg;
}

void anizo::Vivod1st(float st1, float st2)
{
    X1=st1; X2=st2;
}

void anizo::Vivod1stS(float st1, float st2)
{
    AX1=st1; AX2=st2;
}

void anizo::Vivod2nd(float st2, float st1)
{
    Y1=st2; Y2=st1;
}

void anizo::Vivod2ndS(float st2, float st1)
{
    AY1=st2; AY2=st1;
}

void anizo::Vivod3ed(float st3, float st2)
{
    S1=st3; S2=st2;
}

void anizo::Vivod3edS(float st3, float st2)
{
    AS1=st3; AS2=st2;
}

void anizo::Colorper(bool f1, bool f2, bool f3, bool f4, bool f5, bool f6, bool f7, bool f8, bool f9, bool f10, bool f11, bool f12, bool f13, bool f14, bool f15, bool f16, bool f17, bool f18)
{
   d1 = f1; d7 = f7;   d13 = f13;
   d2 = f2; d8 = f8;   d14 = f14;
   d3 = f3; d9 = f9;   d15 = f15;
   d4 = f4; d10 = f10; d16 = f16;
   d5 = f5; d11 = f11; d17 = f17;
   d6 = f6; d12 = f12; d18 = f18;
}

void anizo::on_action_triggered()
{
    exit(0);
}

void anizo::on_Otdal_clicked()
{
    ui->gpiAnz->scale(1/1.1,1/1.1);
}

void anizo::on_Pribl_clicked()
{
    ui->gpiAnz->scale(1.1,1.1);
}

void anizo::on_pushButton_clicked()
{
    //QImage tmpVrem = PolOrig() -> pixmap().toImage();
    //QImage tmpVre(tmpVrem);

    //gpiAnz->setPixmap(QPixmap::fromImage(tmpVre));
}
