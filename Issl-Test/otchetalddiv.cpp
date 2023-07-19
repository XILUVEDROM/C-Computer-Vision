#include "otchetalddiv.h"
#include "ui_otchetalddiv.h"

OtchetAldDiv::OtchetAldDiv(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OtchetAldDiv)
{
    ui->setupUi(this);
}

OtchetAldDiv::~OtchetAldDiv()
{
    delete ui;
}

void OtchetAldDiv::recieveData(uint g, uint g1, uint g2, uint g3)
{
    QString gen1= QString::number(g);
    QString gen2=QString::number(g1);
    QString gen3=QString::number(g2);
    QString gen4=QString::number(g3);

    uint DC= (float) g*1.3+g1*0.1+g2*0.5+g3*0.5;

    if(DC<=4.75)
    {
        ui->textBrowser->append("Доброкачественная Опухоль");
    }
    else if (DC>=5.45) {
        ui->textBrowser->append("Злокаечтсвенная");
    }
    else {
        ui->textBrowser->append("Мелацитарное подохрение");
    }
    ui->textBrowser->append(gen1);
    ui->textBrowser->append(gen2);
    ui->textBrowser->append(gen3);
    ui->textBrowser->append(gen4);


}
