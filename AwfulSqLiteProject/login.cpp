#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    if(MainWindow::getDW().checkIfAuthCorrect(ui->lineEdit->text(),ui->lineEdit_2->text()))
    {
        auto win = new MainWindow();
        win->setAttribute( Qt::WA_DeleteOnClose );
        win->show();
        this->close();
    }

}
