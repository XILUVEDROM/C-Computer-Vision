#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QtAlgorithms>
#include <QColor>
#include <stdlib.h>
#include <stack>
#include <math.h>
#include <unistd.h>
#include <QMessageBox>

//MainWindow(QWidget *parent)
MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent), //наследование от QMainWindow
    ui(new Ui::MainWindow) //инициализация атрибута
{
    ui->setupUi(this);
    //ui->srcGrView->setScene(&scSrc);
    //ui->srcGrView->setScene(&scDst);

    scSrc = new QGraphicsScene();
    scDst = new QGraphicsScene();
    scTmp = new QGraphicsScene();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scSrc;
    delete scTmp;
    delete scDst;
}

 void MainWindow::on_action_triggered() //открытие файла - загрузка изображения
{
    QString fp = QFileDialog::getOpenFileName(); //получение пути к файлу из диалога

    src.load(fp);                              //загрузили файл
    tmp = src;
    dst = src;

    //gpiSrc = new MyGraphicsItem(QPixmap(fp));
    //gpiDst = gpiSrc;
    //gpiTmp = gpiSrc;

    scSrc->clear();
    scSrc->addPixmap(QPixmap::fromImage(src));//инициализация

    scDst->clear();
    scDst->addPixmap(QPixmap::fromImage(src));

    //scDst = scSrc; я дебил и приравнивал указатели :с

    ui->srcGrView->setScene(scSrc); //выводим в интерфейс
    ui->srcGrView->fitInView(src.rect(),Qt::KeepAspectRatio);//подгонка изображения. масштабируя реально меняем размер изображения

    ui->dstGrView->setScene(scDst); //выводим в интерфейс
    ui->dstGrView->fitInView(src.rect(),Qt::KeepAspectRatio);//подгонка изображения. масштабируя реально меняем размер изображения
}
    //!!!оказывается, сцены жестко привязываются к виду и обновляются постоянно


void MainWindow::on_horizontalSlider_2_valueChanged(int value)//бинаризация
{    
    setTrsBin(value);

    QImage tmpSrc(src);
    QImage tmpDst(tmpSrc);

    for (int y = 0; y < tmpSrc.height(); ++y)
    {
        for (int x = 0; x < tmpSrc.width(); ++x)
        {
            if (qGreen(tmpSrc.pixel(x,y))>value) { tmpDst.setPixel(x,y,qRgb(255,255,255)); }
            else { tmpDst.setPixel(x,y,qRgb(0,0,0)); }
        }
    }

    tmp = tmpDst;   //!!!сохранили промежуточный итог
    bin = tmp;

    scDst->clear();
    scDst->addPixmap(QPixmap::fromImage(tmpDst)); //нельзя добавлять GPI - скажет что уже добавлен

    //======================================================================================
    //рисуем соответствующую красную полосу на гистограмме

    showGreen();
}

void MainWindow::on_horizontalSlider_valueChanged(int value) //ручная установка цветового предела
{
    setTrsGreen(value);
    showGreen();
}

void MainWindow::on_horizontalSlider_3_valueChanged(int value) //ручная установка размерного предела
{
    setTrsSize(value);

    //======================================================================================
    //рисуем соответствующую синюю полосу на гистограмме        

    showSize();
}

void MainWindow::on_horizontalSlider_4_valueChanged(int value) //установка минимального учитываемого размера
{
    setMinSize(value);

    dst = src;
    for (int i = 0; i < cells.size(); i++)
    {
        if ( cells[i].maskSize()<minSize )  { cells[i].paint(dst, qRgb(255,255,255));}
    }

    //======================================================================================
    //рисуем соответствующую синюю полосу на гистограмме
    showSize();

    scSrc->clear();
    scSrc->addPixmap(QPixmap::fromImage(dst));
}

void MainWindow::on_pushButton_2_clicked() //медианный фильтр
{    
    QImage tmpSrc(tmp);
    QImage tmpDst(tmpSrc);

    QVector<QRgb> vec;  //встроенный в QT шаблон-динамический массив

    int n = ui->lineEdit_2->text().toInt();

    for ( int y = 0+(n-1)/2; y < tmpSrc.height()-(n-1)/2; y++ )//
    {
        for ( int x = 0+(n-1)/2; x < tmpSrc.width()-(n-1)/2; x++ )//
        {
            vec.clear();
            //РАБОТАЕМ С ОКНОМ
            for ( int shiftY = -(n-1)/2; shiftY <= (n-1)/2; shiftY++) //shift значит сдвиг
            {
                for ( int shiftX = -(n-1)/2; shiftX <= (n-1)/2; shiftX++)
                {
                    vec.append(QColor(tmpSrc.pixel(x+shiftX,y+shiftY)).rgb());        //закидываем пиксели из окна в массив
                }
            }
            qSort(vec);                     //Встроенная в Qt функция-сортировщик
            tmpDst.setPixel(x,y,vec.at((vec.size()-1)/2)); //вставляем в среднюю клетку окна среднее значение массива
        }
    }

    //gpiTmp->setPixmap(QPixmap::fromImage(tmpDst)); //сохранили промежуточный результат

    tmp = tmpDst;
    scDst->addPixmap(QPixmap::fromImage(tmp)); //нельзя добавлять GPI - скажет что уже добавлен

    //ui->srcGrView->setScene(scSrc); //выводим в интерфейс
    //scSrc->update();
    //ui->srcGrView->updateSceneRect(scSrc->sceneRect());
}

void MainWindow::on_pushButton_4_clicked() //в серые тона
{
    QImage tmpSrc(src);
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

    scSrc->clear();
    scSrc->addPixmap(QPixmap::fromImage(tmp));
    //ui->dstGrView->setScene(scTmp);

}

void MainWindow::on_pushButton_5_clicked() //вернуть оригинал
{    
    tmp = src;

    scSrc->clear();
    scSrc->addPixmap(QPixmap::fromImage(tmp));

    //scDst->clear();
    //scDst->addPixmap(QPixmap::fromImage(tmp));
    //ui->srcGrView->setScene(scSrc);

}

void MainWindow::on_pushButton_6_clicked() //зеленый фильтр
{
    QImage tmpSrc(src);
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

    scSrc->clear();
    scSrc->addPixmap(QPixmap::fromImage(tmp));

    //ui->srcGrView->setScene(scSrc);

    //QImage tmpDst1(tmpDst);
    //scDst->addPixmap(QPixmap::fromImage(tmpDst1));
    //ui->dstGrView->setScene(scDst);
}

void MainWindow::on_pushButton_3_clicked() //гистограмма яркости
{    
    gGreen();
    ui->horizontalSlider_2->setValue(trsBin);
    ui->horizontalSlider->setValue(trsGreen);
}

void MainWindow::on_pushButton_7_clicked()//градиент
{
    QImage tmpSrc(tmp);
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

    scSrc->clear();
    scSrc->addPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::on_pushButton_8_clicked() //Заполнение
{
    tmp=innerFill(tmp);
    bin = tmp;
    //Fill(0,0,qRgb(255,255,255),qRgb(255,0,0));
    scDst->clear();
    scDst->addPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::on_pushButton_9_clicked()//К средних
{
    int integer_value = ui->lineEdit->text().toInt();
    tmp = kmean(integer_value, tmp, bin);
    scSrc->clear();
    scSrc->addPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::on_pushButton_10_clicked() //Преобразование расстояний
{
    tmp = distance(bin, qRgb(0,0,0), qRgb(255,255,255) ); //внутри - черное. темное - глубоко, светлое - мелко.
    flood = tmp; //запомнили
    scDst->clear();
    scDst->addPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::on_pushButton_clicked() //водораздел
{
    tmp = watershed1(tmp);
    flood = tmp;
    scDst->clear();
    scDst->addPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::on_pushButton_11_clicked() //инверсия
{
    tmp = invert(tmp);
    scDst->clear();
    scDst->addPixmap(QPixmap::fromImage(tmp));
}

void MainWindow::on_pushButton_12_clicked() //Статистика - открытие нового окна
{
    defineCellsType();
    tablet *tbl = new tablet(this); //указываем на экзепляр класса mainwindow (this) как на родительский
    tbl -> show();
}

void MainWindow::on_pushButton_13_clicked() //гистограмма площадей
{
    gSize();
}

void MainWindow::on_pushButton_14_clicked() //Анализ
{
    cellScan();
    if (ui->checkBox_2->checkState()==Qt::Unchecked)  { edgeFilter(); }
    sizeFilter();

    //defineCellsType();

    //Для оповещения об окончании работы выводим messageBox

    QMessageBox msgBox;
    msgBox.setText("Анализ завершен");
    msgBox.exec();
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

QImage MainWindow::invert(QImage bin)
{
    int r, g, b;

    for (int x = 0; x < bin.width(); x++)
    {
        for (int y = 0; y < bin.height(); y++)
        {
            r = qRed(bin.pixel(x,y));
            g = qGreen(bin.pixel(x,y));
            b = qBlue(bin.pixel(x,y));
            bin.setPixel(x,y,qRgb(255-r,255-g,255-b));
        }
    }
    return bin;
}

QImage MainWindow::innerFill(QImage img)
{
    QImage mask(img);
    QImage mask1(img);

    //просто зальем внешнюю область. она ОДНА. ОДНА. ОДНА, КАРЛ. ВСЕ ОСТАЛЬНЫЕ ВНУТРЕННИЕ.

    //находим у края пустое место и заливаем зеленым на маске
    for ( int y = 0; y < mask.height(); y++ )
    {
        if ( mask.pixel(0, y)!=qRgb(0,0,0) ) { mask = Fill( mask, 0, y, qRgb(255, 255, 255), qRgb(0, 255, 0)); }
        if ( mask.pixel(mask.width()-1, y)!=qRgb(0,0,0) ) { mask = Fill(mask, mask.width()-1, y, qRgb(255, 255, 255), qRgb(0, 255, 0)); }
    }
    for ( int x = 0; x < mask.width(); x++ )
    {
        if ( mask.pixel(x, 0)!=qRgb(0,0,0) ) { mask = Fill(mask, x, 0, qRgb(255, 255, 255), qRgb(0, 255, 0)); }
        if ( mask.pixel(x, mask.height()-1)!=qRgb(0,0,0) ) { mask = Fill(mask, x, mask.height()-1, qRgb(255, 255, 255), qRgb(0, 255, 0)); }
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

QImage MainWindow::distance(QImage bin, QRgb in, QRgb out)
{
    QImage src(bin);
    QImage distance(bin);

    //int maxdistance=0;
    int w = src.width();
    int h = src.height();
    // объявление двумерного динамического массива:
    float **dcs = new float* [w]; // h строк в массиве
        for (int count = 0; count < w; count++)
            dcs[count] = new float [h]; // и w столбцов

    //определим начальные расстояния для внутренних точек, расстояния для внешних
    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < h; y++)
        {
            if (src.pixel(x,y)==out) { dcs[x][y]=0; }
            else { dcs[x][y]=(float)100000; }
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
            temp = dcs[x][y]/(maxdistance+1)*255;
            distance.setPixel(x,y, qRgb(temp,temp,temp));
        }
    }

    distance = invert( distance );

    // высвобождение памяти отводимой под двумерный динамический массив:
        for (int count = 0; count < w; count++)
            delete [] dcs[count];
    //      где h – количество строк в массиве

    return distance;
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
                if (tmpSrc.pixel(x1,y1) == colorFrom)
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

QImage MainWindow::addBorders(QImage img, QRgb bordersColor)
{
    QImage ltl(img);
    QImage big(img.width()+2, img.height()+2, img.format());
    //переносим исходное изображение на расширенное
    for (int i = 0; i < ltl.width(); i++)
    {
        for (int j = 0; j < ltl.height(); j++)
        {
            big.setPixel(i+1, j+1, ltl.pixel(i,j));
        }
    }
    //границы перезаливаем белым на всякий случай
    for (int i = 0; i < big.width(); i++)
    {
        big.setPixel(i, 0, bordersColor);
        big.setPixel(i, big.height()-1, bordersColor);
    }
    for (int i = 0; i < big.height(); i++)
    {
        big.setPixel(0, i, bordersColor);
        big.setPixel(big.width()-1, i, bordersColor);
    }
    //img = big;
    scDst->clear();
    scDst->addPixmap(QPixmap::fromImage(big));
    return big;
}

QImage MainWindow::delBorders(QImage img)
{
    QImage big(img);
    QImage ltl(img.width()-2, img.height()-2, img.format());
    for (int i = 0; i < ltl.width(); i++)
    {
        for (int j = 0; j < ltl.height(); j++)
        {
            ltl.setPixel(i, j, big.pixel(i+1,j+1));
        }
    }

    //img = ltl;
    //tmp = tmpSrc;
    scDst->clear();
    scDst->addPixmap(QPixmap::fromImage(ltl));
    return ltl;
}

QImage MainWindow::watershed1(QImage dist)
{
    // на входе - изображение после преобразования расстояний и К-средних
    dist = addBorders(dist, qRgb(255,255,255));

    QImage label(dist);
    label.fill(Qt::white);
    QImage temp(label);
    QImage mask(label);

    int w = label.width();
    int h = label.height();

    N = 0; //число клеток
    QRgb fill; //цвет заливки новой клетки

    std::stack<std::pair<int,int>> stack; //для работы заливки

    int x1, y1;

    bool change;
    for (int dpt=0; dpt<255; dpt++) //наращиваем уровень воды.
    {

        change = true; //чтобы войти первый раз

        while (change)
        {            
            //====================================================================================================
            // сначала пытаемся нарастить то, что есть

            change = false;
            temp = label; //промежуточное изображение вводится для того чтобы все наращивалось постепенно - по одному слою.

            //первый проход - снизу вверх
            for (int y = 0; y < h-1; y++)
            {
                for (int x = 0; x < w-1; x++)
                {
                    if ( label.pixel(x,y)!=qRgb(255,255,255) && //точка, принадлежащая клетке
                         mask.pixel(x,y)!=qRgb(255,0,0) )      //но не установленной границе между клетками
                    {
                        for (int i = 0; i<2; i++)
                        {
                            if (i==0) { x1=x+1; y1=y; }
                            if (i==1) { x1=x; y1=y+1; }

                            if ( dist.pixel(x1,y1)==qRgb(dpt,dpt,dpt) &&    //доступная высота
                                 mask.pixel(x1,y1)!=qRgb(255,0,0) )         //незамаскированный сосед
                            {
                                if ( label.pixel(x1,y1)==qRgb(255,255,255)) //сосед свободен
                                {
                                    temp.setPixel(x1,y1, label.pixel(x,y)); //добавляем
                                    change = true;
                                }
                                else if ( label.pixel(x1,y1)!=label.pixel(x,y) )//сосед занят другой клеткой
                                {
                                    temp.setPixel(x,y,qRgb(255,255,255)); //устанавливаем белую границу
                                    mask.setPixel(x,y,qRgb(255,0,0));     //маскируем
                                    change = true;
                                }
                            }
                        }
                    }
                }
            }
            //второй проход - сверху вниз
            for (int y = h-1; y > 0; y--)
            {
                for (int x = w-1; x > 0; x--)
                {
                    if ( label.pixel(x,y)!=qRgb(255,255,255) && //точка, принадлежащая клетке
                         mask.pixel(x,y)!=qRgb(255,0,0) )      //но не установленной границе между клетками
                    {
                        for (int i = 0; i<2; i++)
                        {
                            if (i==0) { x1=x-1; y1=y; }
                            if (i==1) { x1=x; y1=y-1; }

                            if ( dist.pixel(x1,y1)==qRgb(dpt,dpt,dpt) &&    //доступная высота
                                 mask.pixel(x1,y1)!=qRgb(255,0,0) )         //незамаскированный сосед
                            {
                                if ( label.pixel(x1,y1)==qRgb(255,255,255)) //сосед свободен
                                {
                                    temp.setPixel(x1,y1, label.pixel(x,y)); //добавляем
                                    change = true;
                                }
                                else if ( label.pixel(x1,y1)!=label.pixel(x,y) )//сосед занят другой клеткой
                                {
                                    temp.setPixel(x,y,qRgb(255,255,255)); //устанавливаем белую границу
                                    mask.setPixel(x,y,qRgb(255,0,0));     //маскируем
                                    change = true;
                                }
                            }
                        }
                    }
                }
            }
            label = temp;
        }


        //============================================================================================================================
        // все что было связно уже нарастили. теперь найдем все изолированное и зальем. если заливается слитно - оно, очевидно, связно.
        //!! надо переделать чтоб рассматривало qGreen()==dpt

        if ( dpt!=255 )
        {
            for (int y = 0; y < h; y++)
            {
                for (int x = 0; x < w; x++)
                {
                    if ( dist.pixel(x,y)==qRgb(dpt,dpt,dpt) && //соответствует высоте
                         label.pixel(x,y)==qRgb(255,255,255) && //не принадлежит клетке
                         mask.pixel(x,y)==qRgb(255,255,255)) //не принадлежит границе
                    {

                        fill = generateColor(N);

                        //помним, что на входе - серое изображение после преобразования расстояний.
                        //label = Fill(dist, x, y, qRgb(dpt,dpt,dpt), fill);
                        //!!просто fill - не подойдет. нужно смотреть пиксели на одном изображении и заливать на другом.

                        temp = dist;//перекинули во временный чтоб нечаянно не залить исходник

                        //===========================================================================================================
                        //просто немного переработанный метод Fill

                        stack.push(std::make_pair(x,y)); //закидываем координаты в стек, одновременно преобразуя их в пару

                        while (!stack.empty())
                        {
                            x = stack.top().first;
                            y = stack.top().second;
                            stack.pop();

                            if ( x>=0 && x<temp.width() && y>=0 && y<temp.height())
                            {
                                if ( temp.pixel(x,y)==qRgb(dpt,dpt,dpt) && //соответствует высоте
                                     label.pixel(x,y)==qRgb(255,255,255) && //не принадлежит клетке
                                     mask.pixel(x,y)==qRgb(255,255,255)) //не принадлежит границе
                                {
                                    temp.setPixel(x,y,fill); //для работы алгоритма заливки
                                    label.setPixel(x,y,fill); //для водораздела
                                }
                            }
                            for (int i = 0; i < 4; i++)
                            {
                                if (i == 0) { x1 = x+1; y1 = y; }
                                if (i == 1) { x1 = x; y1 = y+1; }
                                if (i == 2) { x1 = x-1; y1 = y; }
                                if (i == 3) { x1 = x; y1 = y-1; }

                                if ( x1>=0 && x1<temp.width() && y1>=0 && y1<temp.height())
                                {
                                    if ( temp.pixel(x1,y1)==qRgb(dpt,dpt,dpt) && //соответствует высоте
                                         label.pixel(x1,y1)==qRgb(255,255,255) && //не принадлежит клетке
                                         mask.pixel(x1,y1)==qRgb(255,255,255)) //не принадлежит границе
                                    {
                                        stack.push(std::make_pair(x1,y1));
                                        temp.setPixel(x,y,fill); //для работы алгоритма заливки
                                        label.setPixel(x,y,fill); //для водораздела
                                    }
                                }
                            }
                        }
                        N++;
                    }
                }
            }
        }
    }

    //отобразим границы
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {

        }
    }


    delBorders(label);
    return label;
}

QRgb MainWindow::generateColor(int N) //из номера клетки генерирует цвет.
{
    //N++;
    //int t = N*16+N/(16*16*16); //прыгает через 16 уровней и смещается на 1

    int t = N;//*64;

    int r = t/(256*256);
    int g = (t%(256*256))/256;
    int b = (t%(256*256))%256;

    //r = 0;
    //g = 0;

    return qRgb(r, g, b);
}

int MainWindow::degenerateColor(QRgb color) //из цвета клетки генерирует номер
{
    int r = qRed(color);
    int g = qGreen(color);
    int b = qBlue(color);

    return b + 256*g + 256*256*r;
}

void MainWindow::cellScan()
{

    int cellNumber;

    flood = delBorders(flood);

    int w = flood.width();
    int h = flood.height();

    cells.resize(N); //приводим размер хранилища клеток к размеру соотв-му числу цветов, использованных в водоразделе

    for ( int x = 0; x < w; x++ )
    {
        for (int y = 0; y < h; y++)
        {
            if ( flood.pixel(x,y)!=qRgb(255,255,255) )
            {
                cellNumber=degenerateColor(flood.pixel(x,y));
                cells[cellNumber].setID(cellNumber);
                cells[cellNumber].addEdge(x,y);                
            }
        }
    }    

    int x, y;

    for ( int i = 0; i < cells.size(); i++ )
    {
        //сначала проинициализируем какой-то опорной точкой внутри клетки
        x = cells[i].getMask(0).first;
        y = cells[i].getMask(0).second;

        cells[i].setXmin(x);
        cells[i].setXmax(x);
        cells[i].setYmin(y);
        cells[i].setYmax(y);

        int avgR = 0, avgG = 0, avgB = 0;

        for ( int j = 0; j < cells[i].maskSize(); j++ )
        {
            x = cells[i].getMask(j).first;
            y = cells[i].getMask(j).second;
            //будем задавать рамку
            if ( cells[i].getBorder().left() > x )     { cells[i].setXmin(x); }
            if ( cells[i].getBorder().right() < x )    { cells[i].setXmax(x); }
            if ( cells[i].getBorder().top() < y )      { cells[i].setYmax(y); }
            if ( cells[i].getBorder().bottom() > y )   { cells[i].setYmin(y); }
        }

        //считаем средние цвета
        for (int j = 0; j < cells[i].maskSize(); j++)
        {
            avgR+=qRed(src.pixel(cells[i].getMask(j).first, cells[i].getMask(j).second));
            avgG+=qGreen(src.pixel(cells[i].getMask(j).first, cells[i].getMask(j).second));
            avgB+=qBlue(src.pixel(cells[i].getMask(j).first, cells[i].getMask(j).second));
        }
        cells[i].setAvgRed(avgR/cells[i].maskSize());
        cells[i].setAvgGreen(avgG/cells[i].maskSize());
        cells[i].setAvgBlue(avgB/cells[i].maskSize());

    }
    //строим гистограмму размеров и первично определяем пороги
    gSize();
    /*
    for ( cellNumber = 0; cellNumber < N; cellNumber++ ) //!!! А НАДО ЛИ? и так все выделено и понятно.
    {
        cells[cellNumber].checkEdge(flood); //выкидываем из границы нактуальные клетки
    }
    */
}

void MainWindow::sizeFilter()
{    
    std::stack<int> IDs;
    for (int i = 0; i < cells.size(); i++)
    {
        if (cells[i].maskSize() < minSize)
        {
            //cells[i].paint(flood, qRgb(255,255,255));
            IDs.push(cells[i].getID()); //заполняем стек претендентами на удаление (если удалять сразу - вылезем за пределы массива. он ведь меняет размер)
        }
    }

    //удаляем претендентов
    while (!IDs.empty())
    {
        cells.removeAt(cellNumberFromId(IDs.top()));
        IDs.pop();
    }
}

void MainWindow::edgeFilter()
{
    QImage mask = flood;   //анализируем изображение, которое возвращает водораздел
    int n;
    for ( int y = 0; y < mask.height(); y++ )
    {
        if ( mask.pixel(0, y)!=qRgb(255,255,255) )
        {
            n = cellNumberFromColor(mask.pixel(0,y));
            if (n>=0)
            {
                cells[n].paint(mask, qRgb(255,255,255)); //flood
                cells.remove(n);
            }
        }
        if ( mask.pixel(mask.width()-1, y)!=qRgb(255,255,255) )
        {            
            n = cellNumberFromColor(mask.pixel(mask.width()-1, y));
            if (n>=0)
            {
                cells[n].paint(mask, qRgb(255,255,255));
                cells.remove(n);
            }
        }
    }
    for ( int x = 0; x < mask.width(); x++ )
    {
        if ( mask.pixel(x, 0)!=qRgb(255,255,255) )
        {
            n = cellNumberFromColor(mask.pixel(x, 0));
            if (n>=0)
            {
                cells[n].paint(mask, qRgb(255,255,255));
                cells.remove(n);
            }
        }
        if ( mask.pixel(x, mask.height()-1)!=qRgb(255,255,255) )
        {
            n = cellNumberFromColor(mask.pixel(x, mask.height()-1));
            if (n>=0)
            {
                cells[n].paint(mask, qRgb(255,255,255));
                cells.remove(n);
            }
        }
    }
}

int MainWindow::getAvgColor(cell temp, int mode) //вычисление средних цветов.
{
    int avg = 0;
    for (int i = 0; i < temp.maskSize(); i++)
    {
        if (mode == 1) { avg+=qRed(src.pixel(temp.getMask(i).first,temp.getMask(i).second)); }
        if (mode == 2) { avg+=qGreen(src.pixel(temp.getMask(i).first,temp.getMask(i).second)); }
        if (mode == 3) { avg+=qBlue(src.pixel(temp.getMask(i).first,temp.getMask(i).second)); }
        else { avg = 0; }
    }
    avg = avg/temp.maskSize();
    return avg;
}

void MainWindow::setAvgColor(int number, int mode, int color)
{
    if ( mode==1 )  { cells[number].setAvgRed(color); }
    if ( mode==2 )  { cells[number].setAvgGreen(color); }
    if ( mode==3 )  { cells[number].setAvgRed(color); }
}

void MainWindow::gSize()    //первичное построение гистограммы площадей
{        
    int max = 0;
    for (int n = 0; n < cells.size(); n++)
    {
        if ( cells[n].maskSize()>max ) { max = cells[n].maskSize(); }
    }    

    int mk=0, m = 0, d = 0;
    gistoSize.clear();
    gistoSize.resize(100); //пусть будет 100 бинов

    for ( int i=0; i<gistoSize.size(); i++) { gistoSize[i]=0; }

    int binNumber;
    maxSize = max;

    ui->horizontalSlider_4->setMaximum(maxSize/5); //устанавливаем предел для ползунка "минимальный размер"
    ui->horizontalSlider_3->setMaximum(maxSize); //устанавливаем предел для ползунка "предельный размер"
    //значения меняются и нас тащит в gShow, которая указана в слоте value_changed

    for (int n = 0; n < cells.size(); n++)
    {
        //определяем в который бин попадает
        for (int b = 0; b < 100; b++)
        {
            if ( cells[n].maskSize() > maxSize - maxSize*(b+1)/gistoSize.size() )
            {
                gistoSize[gistoSize.size()-1-b]++;
                break;
            }
        }
    }

    max = 0;  //ищем максимальную высоту гистограммы.
    for ( int i=0; i<gistoSize.size(); i++ )    { if ( gistoSize[i]>max ) { max=gistoSize[i]; } }

    //нормируем
    for ( int i=0; i<gistoSize.size(); i++ )    { gistoSize[i] = 100*gistoSize[i]/max; }

    //=============================================================================================
    //Поиск экстремумов

    int size=gistoSize.size(); //число бинов
    int left = 0, right = 0; //состояние функции в данной точке (растет/убывает/полка)
    int j, flag;
    QVector<int> extr;
    for (int i = 1; i < size-1; i++)
    {
        if (gistoSize[i+1]!=gistoSize[i]) //ориентируемся всегда на правую границу
        {
            j = i;
            flag=0;
            left=0;
            while (j!=0 && flag==0)
            {
                j--;
                if (gistoSize[j]<gistoSize[i])  { left = -1; flag=1; }
                if (gistoSize[j]>gistoSize[i])  { left = +1; flag=1; }
            }
            j = i;
            flag=0;
            right=0;
            while (j!=size-2 && flag==0)
            {
                j++;
                if (gistoSize[i]<gistoSize[j])  { right = +1; flag=1; }
                if (gistoSize[i]>gistoSize[j])  { right = -1; flag=1; }
            }

            if ( left == right) { extr.push_front(-left/abs(left)*i); }   //максимумы со знаком плюс, минимумы со знаком минус
        }        
    }    

    j = extr.size()-1;
    if (extr[j]<0)  { j--; } //если справа минимум - сдвигаемся налево к максимуму. перешли к фоновому пику
    j = j - 1;     //перешли к минимуму слева от пика нормальных клеток.
    trsSize = abs(extr[j]);

    showSize();
}

void MainWindow::showSize()
{
    //============================================================================================
    //отрисовка

    QImage gis(gistoSize.size(),100,QImage::Format_RGB888);
    gis.fill(Qt::white);
    QPainter p(&gis);

    for ( int i=0; i<gistoSize.size(); i++ )
    {
        p.drawRect(QRect(QPoint(i,100),QPoint(i,100 - gistoSize[i]))); //рисуем бин
    }

    int binNumber;

    //определяем в который бин попадает
    for (int b = 0; b < 100; b++)
    {
        if ( trsSize > maxSize - maxSize*(b+1)/gistoSize.size() )
        {
            binNumber = gistoSize.size()-1-b;
            break;
        }
    }
    p.setPen(Qt::blue); //установить синий цвет кисти    
    p.drawRect(QRect(QPoint(binNumber,100),QPoint(binNumber,0))); //рисуем бин размерного предела

    for (int b = 0; b < 100; b++)
    {
        if ( minSize > maxSize - maxSize*(b+1)/gistoSize.size() )
        {
            binNumber = gistoSize.size()-1-b;
            break;
        }
    }
    p.setPen(Qt::red); //установить синий цвет кисти
    p.drawRect(QRect(QPoint(binNumber,100),QPoint(binNumber,0))); //рисуем бин размерного минимума

    ui->label_5->setPixmap(QPixmap::fromImage(gis));
}

void MainWindow::gGreen()
{
    //============================================================================================
    //Первичное построение гистограммы

    gistoGreen.clear();
    gistoGreen.resize(256);
    for ( int i=0; i<256; i++) { gistoGreen[i]=0; }
    QImage tmpSrc(src);

    int mk;
    for ( int y=0; y<tmpSrc.height(); y++)
    {
        for (int x=0; x<tmpSrc.width(); x++)
        {
            mk = qGreen(tmpSrc.pixel(x,y));
            gistoGreen[mk]++;
        }
    }

    int max = 0;  //ищем максимальную высоту гистограммы.
    for ( int i=0; i<gistoGreen.size(); i++ )
    {
        if ( gistoGreen[i]>max ) { max=gistoGreen[i]; }
    }    

    for ( int i=0; i<gistoGreen.size(); i++ )
    {
        gistoGreen[i] = 100*gistoGreen[i]/max;    //нормируем
    }

    //=============================================================================================
    //Поиск экстремумов

    int size=gistoGreen.size(); //число бинов
    int left = 0, right = 0; //состояние функции в данной точке (растет/убывает/полка)
    int j, flag;
    QVector<int> extr;
    for (int i = 1; i < size-1; i++)
    {
        if (gistoGreen[i+1]!=gistoGreen[i]) //'ориентируемся всегда на правую границу
        {
            j = i;
            flag=0;
            left=0;
            while (j!=0 && flag==0)
            {
                j--;
                if (gistoGreen[j]<gistoGreen[i])  { left = -1; flag=1; }
                if (gistoGreen[j]>gistoGreen[i])  { left = +1; flag=1; }
            }
            j = i;
            flag=0;
            right=0;
            while (j!=size-2 && flag==0)
            {
                j++;
                if (gistoGreen[i]<gistoGreen[j])  { right = +1; flag=1; }
                if (gistoGreen[i]>gistoGreen[j])  { right = -1; flag=1; }
            }
            if ( left == right) { extr.push_back(-left/abs(left)*i); }   //максимумы со знаком плюс, минимумы со знаком минус
        }
    }

    j = extr.size()-1;
    if (extr[j]<0)  { j--; } //если справа минимум - сдвигаемся налево к максимуму. перешли к фоновому пику
    //j = j - 3;
    trsGreen = abs(extr[j-3]); //перешли к минимуму слева от пика нормальных клеток.
    trsBin = abs(extr[j-1]);
    showGreen();
}

void MainWindow::showGreen()
{
    //===========================================================================================
    //Отрисовка

    QImage gis(gistoGreen.size(),100,QImage::Format_RGB888);
    gis.fill(Qt::white);
    QPainter p(&gis);

    for ( int i=0; i<gistoGreen.size(); i++ )
    {
        p.drawRect(QRect(QPoint(i,100),QPoint(i,100 - gistoGreen[i]))); //рисуем бин
    }

    p.setPen(Qt::blue); //установить синий цвет кисти
    p.drawRect(QRect(QPoint(trsGreen,100),QPoint(trsGreen,0))); //рисуем бин зеленого предела

    p.setPen(Qt::red); //установить синий цвет кисти
    p.drawRect(QRect(QPoint(trsBin,100),QPoint(trsBin,0))); //рисуем бин порога бинаризации
    ui->label_3->setPixmap(QPixmap::fromImage(gis));
}

void MainWindow::defineCellsType()
{
    normal = 0;
    blast = 0;
    int j;
    for (int n = 0; n < cells.size(); n++)
    {
        j = 0;
        if (cells[n].getAvgGreen()<trsGreen)    { j++; } //темная зеленая компонента характерна для бластов
        if (cells[n].maskSize()>trsSize)    { j++; }
        //=================================================
        if (j == 0) { cells[n].setType(+1); normal++; } //норм
        if (j == 1) { cells[n].setType(0); }  //хз
        if (j == 2) { cells[n].setType(-1); blast++; } //бласт
        //=================================================
    }
    ratio = (float)blast/(float)(blast+normal);
}

int MainWindow::cellNumberFromId(int id) //определяем номер в массиве по id
{
    for (int i = 0; i<cells.size();i++)
    {
        if (cells[i].getID()==id)   { return i; }
    }
}

int MainWindow::cellNumberFromColor(QRgb color) //определяем номер в массиве по цвету
{
    for (int i = 0; i<cells.size();i++)
    {
        if (cells[i].getColor()==color)   { return i; }
    }
    return -1;
}

