#include "resultfound.h"
#include "ui_resultfound.h"

ResultFound::ResultFound(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ResultFound)
{
    ui->setupUi(this);
}

ResultFound::~ResultFound()
{
    delete ui;
}
