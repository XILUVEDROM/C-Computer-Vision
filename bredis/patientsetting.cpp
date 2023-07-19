#include "patientsetting.h"
#include "ui_patientsetting.h"

PatientSetting::PatientSetting(const QString Table_1, const QString Table_2, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientSetting)
{
    ui->setupUi(this);

    model = new QSqlQueryModel(this);
    model->setQuery("SELECT Diagnosis FROM "+Table_1);
    ui->comboBox->setModel(model);
    model->setQuery("SELECT Diagnosis FROM "+Table_2);
    ui->comboBox_2->setModel(model);
}

PatientSetting::PatientSetting(const QString Table_1, const QString Table_2, const QVariantList &dataPatient, QWidget *parent):
    QDialog(parent),
    ui(new Ui::PatientSetting)
{
    ui->setupUi(this);

    model = new QSqlQueryModel(this);
    model->setQuery("SELECT Diagnosis FROM "+Table_1);
    ui->comboBox->setModel(model);

    model->setQuery("SELECT Diagnosis FROM "+Table_2);
    ui->comboBox_2->setModel(model);

    ui->comboBox->setCurrentIndex(dataPatient.at(0).toInt()-1);
    ui->comboBox_2->setCurrentIndex(dataPatient.at(0).toInt()-1);
    ui->dateEdit->setDate(dataPatient.at(1).toDate());
    ui->lineEdit->setText(dataPatient.at(2).toString());
    ui->lineEdit_2->setText(dataPatient.at(3).toString());
    ui->lineEdit_3->setText(dataPatient.at(4).toString());
    ui->lineEdit_4->setText(dataPatient.at(5).toString());
    if(dataPatient.at(6).toString() == "М"){
        ui->radioButton->setChecked(true);
        ui->radioButton_2->setChecked(false);
    } else {
        ui->radioButton->setChecked(false);
        ui->radioButton_2->setChecked(true);
    }
    ui->dateEdit_2->setDate(dataPatient.at(7).toDate());
    ui->lineEdit_5->setText(dataPatient.at(8).toString());
}

PatientSetting::~PatientSetting()
{
    delete ui;
}

void PatientSetting::on_pushButton_2_clicked()
{
    emit closeWindow();
}

void PatientSetting::on_pushButton_clicked()
{
    int age = QDate::currentDate().year()-ui->dateEdit->date().year();
    if(QDate(0, ui->dateEdit->date().month(), ui->dateEdit->date().day()) > QDate(0, QDate::currentDate().month(), QDate::currentDate().day()))
                    --age;

    QVariantList data;
    data.append(ui->comboBox->currentIndex()+1);
    data.append(ui->comboBox_2->currentIndex()+1);
    data.append(ui->dateEdit->date());
    data.append(ui->lineEdit->text());
    data.append(ui->lineEdit_2->text());
    data.append(ui->lineEdit_3->text());
    data.append(ui->lineEdit_4->text());
    if(ui->radioButton->isChecked())
        data.append("М");
    else
        data.append("Ж");
    data.append(age);
    data.append(ui->dateEdit_2->date());
    data.append(ui->lineEdit_5->text());
    emit PatientData(data);
}

void PatientSetting::closeEvent(QCloseEvent *event)
{
    emit closeWindow();
    QDialog::closeEvent(event);
}
