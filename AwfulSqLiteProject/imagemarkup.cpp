#include "imagemarkup.h"
#include "ui_imagemarkup.h"
#include "mainwindow.h"
#include <QMouseEvent>
#include <QPainter>

ImageMarkUp::ImageMarkUp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageMarkUp)
{
    ui->setupUi(this);


    QImage *imageObject = new QImage();
    imageObject->load(MainWindow::getImagePathForMarkup());

    QPixmap image = QPixmap::fromImage(*imageObject);

    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);

    ui->graphicsView->viewport()->installEventFilter(this);


    mousePressed = false;
    drawStarted = false;

}

ImageMarkUp::~ImageMarkUp()
{
    delete ui;
}

bool ImageMarkUp::eventFilter(QObject *watched, QEvent *event)
{
    QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
    if (event->type() == QEvent::MouseButtonPress){

        if (watched == ui->graphicsView->viewport()){
                mRect.setTopLeft(mouseEvent->pos());
                mRect.setBottomRight(mouseEvent->pos());
                mousePressed = true;
                qDebug()<<mouseEvent->pos();
        }
        update();
    }

    if(event->type() == QEvent::MouseMove){
        if (watched == ui->graphicsView->viewport()){
                mRect.setBottomRight(mouseEvent->pos());
        }
        update();
    }


    if(event->type() == QEvent::MouseButtonRelease){
        if (watched == ui->graphicsView->viewport()){
        }
        update();
    }


    //return ImageMarkUp::eventFilter(watched, event); why wont you work!!????
    return false;
}

void ImageMarkUp::paintEvent(QPaintEvent *event)
{
          //When the mouse is pressed
        if(mousePressed){
            ui->graphicsView->scene()->clear();
            ui->graphicsView->scene()->addRect(mRect);
            drawStarted = true;
        }
}




void ImageMarkUp::on_pushButton_2_clicked()
{
    this->close();
}
