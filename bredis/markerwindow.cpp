#include "markerwindow.h"
#include "ui_markerwindow.h"
#include <QDebug>

MarkerWindow::MarkerWindow(QString fileName, int idSymptom, QWidget *parent) : id(idSymptom),
    QMainWindow(parent),
    ui(new Ui::MarkerWindow)
{
    ui->setupUi(this);
    scene = new PaintScene();                                               // Инициализируем графическую сцену
    pmItem = new QGraphicsPixmapItem(fileName);
    QPixmap pm(fileName);
    scene->setSceneRect(0, 0, pm.width(), pm.height());
    scene->addItem(pmItem);
    QPixmap marker;
    if(marker.load("MarkersDir/" + QString::number(id) + ".png"))
        scene->addPixmap(marker);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);                // Устанавливаем сглаживание
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    initialColor();
    ui->comboBox_2->setCurrentIndex(2);
}

MarkerWindow::~MarkerWindow()
{
    delete ui;
}

void MarkerWindow::initialColor()
{
    QPixmap pm(20,20);
    pm.fill(Qt::black);
    ui->comboBox_3->addItem(QIcon(pm), QString("Черный"));
    pm.fill(Qt::white);
    ui->comboBox_3->addItem(QIcon(pm), QString("Белый"));
    pm.fill(Qt::red);
    ui->comboBox_3->addItem(QIcon(pm), QString("Красный"));
    pm.fill(Qt::green);
    ui->comboBox_3->addItem(QIcon(pm), QString("Зеленный"));
    pm.fill(Qt::blue);
    ui->comboBox_3->addItem(QIcon(pm), QString("Синий"));
    pm.fill(Qt::cyan);
    ui->comboBox_3->addItem(QIcon(pm), QString("Голубой"));
    pm.fill(Qt::magenta);
    ui->comboBox_3->addItem(QIcon(pm), QString("Фиолетовый"));
    pm.fill(Qt::yellow);
    ui->comboBox_3->addItem(QIcon(pm), QString("Желтый"));
    pm.fill(Qt::gray);
    ui->comboBox_3->addItem(QIcon(pm), QString("Серый"));
}

void MarkerWindow::on_comboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        scene->setTypeFigure(PaintScene::SquareType);
        break;
    case 1:
        scene->setTypeFigure(PaintScene::TriangleType);
        break;
    case 2:
        scene->setTypeFigure(PaintScene::RombType);
        break;
    case 3:
        scene->setTypeFigure(PaintScene::LineType);
        break;
    default:
        break;
    }
}

void MarkerWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    scene->setDepthFigure(arg1.toInt());
}

void MarkerWindow::on_comboBox_3_currentIndexChanged(int index)
{
    switch(index){
    case 0:
        scene->setColorFigure(Qt::black);
        break;
    case 1:
        scene->setColorFigure(Qt::white);
        break;
    case 2:
        scene->setColorFigure(Qt::red);
        break;
    case 3:
        scene->setColorFigure(Qt::green);
        break;
    case 4:
        scene->setColorFigure(Qt::blue);
        break;
    case 5:
        scene->setColorFigure(Qt::cyan);
        break;
    case 6:
        scene->setColorFigure(Qt::magenta);
        break;
    case 7:
        scene->setColorFigure(Qt::yellow);
        break;
    case 8:
        scene->setColorFigure(Qt::gray);
        break;
    default:
        break;
    }
}

void MarkerWindow::closeEvent(QCloseEvent *event)
{
    emit closeWindow();
    QMainWindow::closeEvent(event);
}

void MarkerWindow::on_pushButton_5_clicked()
{
    emit closeWindow();
}

void MarkerWindow::on_pushButton_clicked()
{
    scene->removeLastFigure();
}

void MarkerWindow::on_pushButton_2_clicked()
{
    scene->removeALlFigure();
}

void MarkerWindow::on_pushButton_3_clicked()
{
    scene->restoreFigure();
}

void MarkerWindow::on_pushButton_4_clicked()
{
    scene->removeItem(pmItem);
    QImage image(scene->width(), scene->height(), QImage::Format_ARGB32_Premultiplied);
    image.fill(0x00FFFFFF);
    QPainter painter(&image);
    scene->render(&painter);
    QDir dir;
    if(!dir.exists("MarkersDir"))
        dir.mkdir("MarkersDir");
    image.save("MarkersDir/" + QString::number(id) + ".png");
    QSqlQuery query;
    qDebug() << query.exec("UPDATE SymptomsOfTheImage SET MarkerName = 'true' WHERE id = " + QString::number(id));
    emit closeWindow();
}
