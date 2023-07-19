#include "diagramma.h"
#include "ui_diagramma.h"
#include "global.h"
#include "mainwindow.h"


#include <QPainter>
#include <QColor>
#include <QRectF>
#include <QPen>
//#include "global.h"

#include <QBrush>
#include <QVariant>
#include <cmath>

#include <algorithm>
#include <vector>

#include <QDebug>

#include <QStringList>
#include <QLineEdit>
#include <QFileDialog>
#include <QImage>
#include <QDesktopWidget>
#include <QString>
#include <QFileInfo>
#include <QDir>

#include <QTableView>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtSql/QSqlError>
#include <QMessageBox>


#include <QMainWindow>
#include <QImage>
Diagramma::Diagramma(QWidget *parent) :
    QWidget(parent),
    di(new Ui::Diagramma)
{
    di->setupUi(this);
    db = QSqlDatabase::database("Con");

    di->graphicsView->setScene(new QGraphicsScene);


/*
 *   //работает
    QPixmap *pix = new QPixmap(400,400);
    QPainter *paint = new QPainter(pix);// QPainter paint(&pix);
    paint->setPen(*(new QColor(255,34,255,255)));
    paint->drawRect(15,15,180,180);
    di->graphicsView->scene()->addPixmap(*pix);//di->graphicsView->scene()->addPixmap(pix);
    di->graphicsView->scene()->update();
    delete paint;
    */ //работает

    qDebug() << "narisovano3";

     QPixmap *pix = new QPixmap(400,400);
     pix->fill(Qt::white);

    QSize labSize = di->graphicsView->size()-QSize(250,320);

    qDebug() <<"Diagramma starts";

    QPainter painter (pix);
    QPen pen;
    QRectF size;
    QColor qvColors[26];
    QColor q2Colors[2];
    qvColors[0].setRgb(255, 200, 30, 200); qvColors[1].setRgb(10, 20, 50, 200); qvColors[2].setRgb(10, 20, 70, 200);
    qvColors[3].setRgb(100, 0, 90, 200); qvColors[4].setRgb(10, 20, 110, 200); qvColors[5].setRgb(10, 20, 130, 200);
    qvColors[6].setRgb(10, 20, 150, 200); qvColors[7].setRgb(100, 100, 70, 200); qvColors[8].setRgb(100, 20, 190, 200);
    qvColors[9].setRgb(10, 20, 210, 200); qvColors[10].setRgb(10, 40, 30, 200); qvColors[11].setRgb(10, 60, 30, 200);
    qvColors[12].setRgb(190, 20, 0, 200); qvColors[13].setRgb(10, 100, 30, 200); qvColors[14].setRgb(10, 120, 30, 200);
    qvColors[15].setRgb(10, 140, 30, 200); qvColors[16].setRgb(10, 160, 30, 200); qvColors[17].setRgb(10, 180, 30, 200);
    qvColors[18].setRgb(10, 200, 30, 200); qvColors[19].setRgb(30, 30, 30, 200); qvColors[20].setRgb(50, 30, 30, 200);
    qvColors[21].setRgb(70, 30, 30, 200); qvColors[22].setRgb(90, 30, 30, 200); qvColors[23].setRgb(110, 30, 30, 200);
    qvColors[24].setRgb(130, 30, 30, 200); qvColors[25].setRgb(150, 30, 30, 200);
    //qvColors[26].setRgb(180, 30, 30, 200); qvColors[27].setRgb(200, 30, 30, 200);
    qDebug() <<qvColors[0];
    qDebug() <<qvColors[25];

    q2Colors[0].setRgb(10, 80, 10, 200);
    q2Colors[1].setRgb(10, 100, 100, 200);
    qDebug() <<q2Colors[0];
     qDebug() <<q2Colors[1];

    pen.setColor(Qt::white);
    pen.setWidth(0);
    painter.setPen(pen);


    size = QRectF(20,0,labSize.height(), labSize.width());

    double sum = 0.0, startAngle = 0.0;
    double angle = 0.0, endAngle, percent;
    //Diag_Sum
    qDebug() << "Rows_amount[i]";
     for (int i = 0; i < 25; i++)
    { qDebug() << Rows_amount[i];
    }
    sum = Rows_amount[0]+Rows_amount[1]+ Rows_amount[2]+ Rows_amount[3]+ Rows_amount[4]+ Rows_amount[5]+Rows_amount[6]+Rows_amount[7]+Rows_amount[8]+ Rows_amount[9]+ Rows_amount[10]+ Rows_amount[11]+Rows_amount[12]+ Rows_amount[13]+ Rows_amount[14]+ Rows_amount[15]+ Rows_amount[16]+Rows_amount[17]+Rows_amount[18]+Rows_amount[19]+ Rows_amount[20]+ Rows_amount[21]+ Rows_amount[22]+Rows_amount[23]+ Rows_amount[24];
    qDebug() << "stroka109 diagramm";
     qDebug() << sum;
    //for (int g = 0; g < 25; g++){
    if (sum==0)
    { //sum = 25;
        // Rows_amount[g]=1;
        QMessageBox::information(this, "Bнимание", "Нет примеров с таким признаком в базе вероятность диагнозов не известна") ;
        di->label->setText("      ПО ВЫБРАННОМУ ДЛЯ ПОИСКА СОЧЕТАНИЮ ПРИЗНАКОВ \n        В ЭТАЛОННОЙ БАЗЕ НЕТ ПРИМЕРОВ,\n\n       ЗАКРОЙТЕ ЭТО ОКНО КРЕСТИКОМ В ПРАВОМ ВЕРХНЕМ УГЛУ\n      И ВЕРНИТЕСЬ НА ВКЛАДКУ ПОИСК \n         ДЛЯ ИЗМЕНЕНИЯ ПАРАМЕТРОВ ЗАПРОСА.\n\n       СПАСИБО");
    }
    // }
    else
    {
        qDebug() <<" SUM vsego strok";
        qDebug() <<sum;
        Flag_poisk_without = true;// dly etoi versii vsegda
        if (Flag_poisk_without == true)
        {
            QString percent_text = "";
            // qDebug() <<" Квадратик должен быть нари сован для каждого i";
            qDebug() <<"Poisk bez nosology s diagrammoi";
            for (int i = 0; i < 25; i++)
            {
                percent = double(Rows_amount[i]/sum);//строк с определенным значением нозологии
                //   qDebug() << i+ "kolichestvo ctrok s nosologiei";
                qDebug() << Rows_amount[i];
                angle = percent *360.0;
                qDebug() << angle;
                endAngle = startAngle + angle;
                painter.setBrush(qvColors[i]);
                painter.drawPie(size, startAngle*16, angle*16);
                startAngle = endAngle;

                // QPainter rect();
                int Y;
                Y=13*i+4;
                painter.setBrush(qvColors[i]);
                painter.drawRect(378, -0+Y, 15, 8);

                percent_text += (QString::number(i+1))+" "+(QString::number(Rows_amount[i]/sum*100))+"%\n";
                QRectF size1 = QRectF(396,-100+Y,15, 20);

                painter.setBrush(qvColors[i]);
                painter.drawText(size1,percent_text);
                qDebug() << "percent_text";
                qDebug() << percent_text;
                //qDebug() <<" Квадратик должен быть нари сован для каждого i";
            }

            di->label_5->setText(percent_text);
            di->label_4->setText("1.Мастит\n2.Туберкулез\n3.Липогранулема (жировой некроз)\n4.Фиброзно-кистозная болезнь\n5.Фиброзно-кистозная болезнь(Киста)\n6.Гинекомастия\n7.Фиброаденома\n8.Атипическая гиперплазия \n9.Атипическая гиперплазия (по типу тяжелой дисплазии)\n10.Внутрипротоковая папиллома\n11.Протоковый рак\n12.Дольковый рак\n13.Тубулярный рак\n14.Медуллярный рак\n15.Рак Педжета\n16.Слизистый (муцинозный, колоидный)рак\n17.Рак с апокриновой дифференцировкой клеток\n18.Плоскоклеточный рак\n19.Рак с высокой дифференцировкой клеток\n20.Рак с низкой дифференцировкой клеток\n21.Рак без уточнения степени дифференцировки (БДУ)\n22.Эпидермоидная киста\n23.Листовидная опухоль доброкачественная\n24.Листовидная опухоль злокачественная\n25.Внутрипротоковый папиллярный рак");
        }
     }
    /* else
    {
        // kogda mi zadali poisk po nozjlogii pokazet ukazanuiynozologiu i vse drugie vmeste
        qDebug() <<" Diagramma s 2 sectorami";
        if(Flag_poisk_without == false)
        //{
        qDebug() <<one_nosology[0];
         qDebug() <<one_nosology[1];
         qDebug() << sum;
        one_nosology[1]= sum-one_nosology[0];//если здесь возникнет ошибка, искать nosology[0]
        qDebug() <<one_nosology[1];
        QString nos ="";
        for (int j = 0; j < 2; j++)
        {
            percent = double(one_nosology[j]/sum);//строк с определенным значением нозологии
            angle = percent *360.0;
            qDebug() << angle;
            endAngle = startAngle + angle;
            painter.setBrush(qvColors[j]);
            painter.drawPie(size, startAngle*16, angle*16);
            //painter.drawText(size,"text");
            startAngle = endAngle;
            //QPainter rect2(di->graphicsView->scene());
            int Y2;
            Y2=15*j+0;
            painter.setBrush(qvColors[j]);
            painter.drawRect(348, 288+Y2, 15, 8);
             nos += (QString::number(one_nosology[j]/sum*100))+"%\n";
        }
        di->label_3->setText("1 "+Name_ng+"\nДругие нозологии");

        di->label_7->setText(nos);
    }
*/
    di->graphicsView->scene()->addPixmap(*pix);
    di->graphicsView->scene()->update();

    Diag_Sum =0;
}


Diagramma::~Diagramma()
{
    delete di;
}
