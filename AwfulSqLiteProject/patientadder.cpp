#include "patientadder.h"
#include "ui_patientadder.h"

PatientAdder::PatientAdder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientAdder)
{
    ui->setupUi(this);
}

PatientAdder::~PatientAdder()
{
    delete ui;
}

void PatientAdder::on_pushButton_2_clicked()
{
    this->close();
}
