#include "otchetx3.h"
#include "ui_otchetx3.h"
#include "error.h"
#include "ui_error.h"

Otchetx3::Otchetx3(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Otchetx3)
{
    ui->setupUi(this);
}

Otchetx3::~Otchetx3()
{
    delete ui;
}

void Otchetx3::recieveData(QString str, uint g1, uint g2, float g3,uint g4,uint g5)
{


    QString gen1 = QString::number(g1);
    QString gen2 = QString::number(g2);
    QString gen3 = QString::number(g3);
    QString gen4 = QString::number(g4);
    QString gen5 = QString::number(g5);
    ui->textBrowser->append("Если видно 0 и слово симметрична, то значит все хорошо, если просто 0, значит функция на поиск симметрии не выполнена");
    ui->textBrowser->append("Анализ параметра по ассиметричности объекта происходит по вычисление коэффициента ассиметричности по осям, это деленме количества симметричных пикселей на кол-во всех пикселей находящихся в половине загруженного изображения. В случае если коэффциент отношения кол-ва симметриных точек к кол-ву всех  точек в этой полуобласти более 0.9 то симметрична если нет то ассиметрична");
    ui->textBrowser->append(str);
    ui->textBrowser->append(gen1);
    ui->textBrowser->append(gen2);
    ui->textBrowser->append(gen3);
    if (g5>=3)
    {
        ui->textBrowser->append("Подозрения на меланоному по поиску различных цветов" );
    }
    else {
        ui->textBrowser->append("По ппоиску различных цветов нет подохрения на меланому");
    }
    if(g4>3)
    {

        ui->textBrowser->append("Подозренеи на меланому по пигментной сети " );
    }
    else {
        ui->textBrowser->append("По пигемнтной сети подозрения на меланому нет");
    }
}
