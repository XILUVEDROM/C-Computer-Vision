#include "tablet.h"
#include "ui_tablet.h"

#include "QStandardItemModel"
#include <QTableWidgetItem>
#include "QStandardItem"
#include "QPushButton"
#include "QBoxLayout"
#include "QString"

#include <iostream>

tablet::tablet(QWidget *parent) :   //конструктор.
    QMainWindow(parent),
    ui(new Ui::tablet)
{    

    parentw = (MainWindow*)parent; //преобразование типа указателя
    ui->setupUi(this);

    //====================================================================================================
    //  1я таблица

    QStandardItemModel *model1 = new QStandardItemModel;
    QStandardItem *item1;

    //Заголовки столбцов
    QStringList horizontalHeader1;
    horizontalHeader1.append("Всего");
    horizontalHeader1.append("Неопред");
    horizontalHeader1.append("Бластов");
    horizontalHeader1.append("Вероятность");    

    //преобразование числа в строку - string <строка> = std::to_string(<числовая переменная>);

    //Заголовки строк
    QStringList verticalHeader1;
    QStandardItem *item;
    QString str, str1;

    verticalHeader1.append("Итого");

    model1->setHorizontalHeaderLabels(horizontalHeader1);
    model1->setVerticalHeaderLabels(verticalHeader1);

    item = new QStandardItem(str.setNum(parentw->getN()));
    model1->setItem(0, 0, item);

    int undef = parentw->getN()-parentw->getNorm()-parentw->getBlast();
    item = new QStandardItem(str.setNum(undef));
    model1->setItem(0, 1, item);

    item = new QStandardItem(str.setNum(parentw->getBlast()));
    model1->setItem(0, 2, item);

    int norm = parentw->getNorm();
    int blast = parentw->getBlast();
    float ratio = (float)blast/(float)(blast+norm);
    std::cout << "ratio = " << ratio << std::endl;
    str.setNum(ratio*100);
    str += "%";

    item = new QStandardItem(str);
    model1->setItem(0, 3, item);

    ui->tableView->setModel(model1);
    ui->tableView->resizeRowsToContents();
    ui->tableView->resizeColumnsToContents();

    //====================================================================================================
    //  2я таблица

    enum{
        id,
        type,
        size,
        color,
        action
    };

    QStandardItemModel *model2 = new QStandardItemModel;
    QStandardItem *item2;

    //Заголовки столбцов
    QStringList horizontalHeader2;

    horizontalHeader2.append("ID");
    horizontalHeader2.append("Тип");
    horizontalHeader2.append("Размер");
    horizontalHeader2.append("Цвет");
    horizontalHeader2.append("Просмотр");

    //преобразование числа в строку - string <строка> = std::to_string(<числовая переменная>);    

    //Заголовки строк
    QStringList verticalHeader2;       

    model2->setHorizontalHeaderLabels(horizontalHeader2);
    model2->setVerticalHeaderLabels(verticalHeader2);


    for ( int row = 0; row < parentw->getN(); row++) //на каждую клетку по строке
    {
        //================================================================
        //Заполняем таблицу

        verticalHeader2.append(QString::number(row)); //добавляем строку, добавляя соответствующий новый заголовок

        item = new QStandardItem(str.setNum(parentw->getCell(row).getID()));
        model2->setItem(row, id, item);

        if (parentw->getCell(row).getType()==+1)    { item = new QStandardItem(QString("normal")); }
        if (parentw->getCell(row).getType()==0)    { item = new QStandardItem(QString("undefined")); }
        if (parentw->getCell(row).getType()==-1)    { { item = new QStandardItem(QString("blast")); } }
        model2->setItem(row, type, item);

        item = new QStandardItem(str.setNum(parentw->getCell(row).maskSize()));
        model2->setItem(row, size, item);

        str = "";
        str1.setNum(parentw->getCell(row).getAvgRed());
        str += str1;
        str+= ".";
        str1.setNum(parentw->getCell(row).getAvgGreen());
        str+=str1;
        str+= ".";
        str1.setNum(parentw->getCell(row).getAvgBlue());
        str+=str1;

        item = new QStandardItem(str);
        model2->setItem(row, color, item);

        ui->tableView_2->setModel(model2); //во избежания ошибки работы с памятью необходимо сначала
        // применить можель = создать ячейку. а уж потом пихать туда кнопку
        ui->tableView_2->setIndexWidget(ui->tableView_2->model()->index(row, action), createButtonWidget() );   //Мутим кнопку

    }

    model2->setHorizontalHeaderLabels(horizontalHeader2);
    model2->setVerticalHeaderLabels(verticalHeader2);

    ui->tableView_2->setModel(model2);
    ui->tableView_2->resizeRowsToContents();
    ui->tableView_2->resizeColumnsToContents();
}

tablet::~tablet()
{
    delete ui;
}

QWidget* tablet::createButtonWidget() const //кнопка в рамке
{
    QWidget* wgt = new QWidget;
    QBoxLayout* l = new QHBoxLayout;
    QPushButton* btn = new QPushButton( "рис." );

    connect( btn, SIGNAL( clicked(bool) ), SLOT(onBtnClicked()) ); //соединяем сигнал со слотом

    l->setMargin( 0 );
    l->addWidget( btn );
    l->addStretch();
    wgt->setLayout( l );

    return wgt;
}

void tablet::onBtnClicked()
{
    if( QPushButton* btn = qobject_cast<QPushButton*>( sender() ) )
    {
        QModelIndex index = ui->tableView_2->indexAt( btn->parentWidget()->pos() );
        int cellnumber = index.row(); //номер клетки - номер строки, содержащей информацию и соответствующую кнопку

        cell prop = parentw->getCell(cellnumber);
        QRect border = parentw->getCell(cellnumber).getBorder();
        QImage img = parentw->getDst();

        //border = QRect(QPoint(100,100),QPoint(200,200));

        //рисуем на исходном изображении прямоугольник, описывающий заданную клетку
        QPainter paint(&img);
        paint.setPen(Qt::red);  //задаем цвет пера
        paint.fillRect(border, Qt::CrossPattern); //рисуем поверх прямоугольник

        parentw->showSrc(img);  //рисуем результат.
    }
}

