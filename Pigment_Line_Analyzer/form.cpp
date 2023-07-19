#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(new QGraphicsScene(this));
    ui->graphicsView_2->setScene(new QGraphicsScene(this));
    //connect(ui->horizontalScrollBar, SIGNAL(valueChanged()), this, SLOT(loadImage()));
}

QString Form::openFromBase()
{
    file_name.clear(); count = 0;
    file_name = QFileDialog::getOpenFileName(this, "Open image", "base/");
    QStringList sl = file_name.split("/");
    file_name = sl.at(sl.size()-1);
    return file_name;
}

void Form::createForm(const std::vector<QString> &results)
{
    if (!file_name.isEmpty())
    {
        res = results;
        ui->graphicsView->scene()->addPixmap(QPixmap("base/"+file_name));
        ui->horizontalScrollBar->setValue(0);
        count = results.size();
        ui->horizontalScrollBar->setMaximum(count-1);
        on_horizontalScrollBar_valueChanged(0);
    }
}

Form::~Form()
{
    delete ui;
}

void Form::on_horizontalScrollBar_valueChanged(int value)
{
    ui->lineEdit_5->setText(QString("%1 из %2").arg(value+1).arg(count));
    QString tmp = res[value].split(".").at(0);
    ui->graphicsView_2->scene()->addPixmap(QPixmap("base/"+file_name.split(".").at(0)+"/"+tmp+"_cont.bmp"));
    ui->lineEdit->setText(tmp.split("_").at(0));
    ui->lineEdit_2->setText(tmp.split("_").at(1));
    ui->lineEdit_3->setText(tmp.split("_").at(2));
    ui->lineEdit_4->setText(tmp.split("_").at(3));
}
