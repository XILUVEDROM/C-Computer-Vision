#include "error.h"
#include "ui_error.h"

Error::Error(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Error)
{
    ui->setupUi(this);
}

Error::~Error()
{
    delete ui;
}

void Error::recieveData(QString Str)
{
    ui->lineEdit->setText(Str);
}
