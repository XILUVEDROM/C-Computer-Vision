#include "addimage.h"
#include "ui_addimage.h"
#include "mainwindow.h"
#include "colon.h"
#include "kidney.h"
#include "mammalgland.h"

AddImage::AddImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddImage)
{
    ui->setupUi(this);
}

AddImage::~AddImage()
{
    delete ui;
}

void AddImage::on_pushButton_clicked()
{
    Kidney *kd = new Kidney();
    kd->showMaximized();
}

void AddImage::on_pushButton_3_clicked()
{
    Colon *cl = new Colon();
    cl->showMaximized();
}

void AddImage::on_pushButton_2_clicked()
{
    MammalGland *mg = new MammalGland();
    mg->showMaximized();
}
