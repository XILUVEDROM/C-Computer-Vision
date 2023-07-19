#include "markupadder.h"
#include "ui_markupadder.h"
#include "mainwindow.h"
#include <QFileDialog>

MarkUpAdder::MarkUpAdder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MarkUpAdder)
{
    ui->setupUi(this);
}

MarkUpAdder::~MarkUpAdder()
{
    delete ui;
}

void MarkUpAdder::on_pushButton_2_clicked()
{
    this->close();
}

void MarkUpAdder::on_pushButton_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
    this,
    tr("Open File"),
    "",
    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
    );

    ui->lineEdit->setText(imagePath);
}

void MarkUpAdder::on_pushButton_3_clicked()
{
    MainWindow::getDW().insertMarkUp(ui->lineEdit_2->text(),ui->lineEdit_3->text(),ui->lineEdit_4->text());
    MainWindow::setPathForInsertingMarkUp(ui->lineEdit->text());
    MainWindow::copyAndCreateDirIfNessesaryForMarkupInsertion();
    this->close();
}
