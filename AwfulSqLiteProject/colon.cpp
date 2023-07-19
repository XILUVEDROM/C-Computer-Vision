#include "colon.h"
#include "ui_colon.h"
#include "mainwindow.h"
#include "QDebug"
#include <QFileDialog>
#include <QDate>

Colon::Colon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Colon)
{
    ui->setupUi(this);
    ui->comboBox->addItems(MainWindow::getDW().fillGystoDiagInInsertForm("Толстая кишка"));
    ui->comboBox_2->addItems(MainWindow::getDW().fillCytoDiagInInsertForm("Толстая кишка"));
    ui->comboBox_3->addItems(MainWindow::getDW().fillColorOfImage("Толстая кишка"));
    ui->comboBox_5->addItems(MainWindow::getDW().fillScope("Толстая кишка"));
}

Colon::~Colon()
{
    delete ui;
}

void Colon::on_pushButton_2_clicked()
{
    this->close();
}

void Colon::on_pushButton_clicked()
{

    QString tmp = "";
    int pat_id = 0;


    //list of all QLabels
    QList<QLabel*> labelList = this->findChildren<QLabel*>() ;
    //list of all CheckBoxes
    QList<QCheckBox*> list = this->findChildren<QCheckBox*>() ;
    foreach(QCheckBox *box, list){
        //for each box we check if its checked
        if(box->isChecked()){


            //checking in which layout checkbox is
            QList<QVBoxLayout*> layoustList = this->findChildren<QVBoxLayout*>();
            foreach(QVBoxLayout *l, layoustList){
                //looping through all layouts


                //Creating var for holding label
                QString tmpLabel="";

                //here we looking for label inside layout to use it later
                foreach (QLabel *label, labelList) {
                    for (int i = 0; i < l->count(); ++i)
                    {
                        QWidget *widget = l->itemAt(i)->widget();
                        if(widget == label){
                            tmpLabel=label->text();
                        }
                    }
                }

                //here we find checked box inside layout
                for (int i = 0; i < l->count(); ++i)
                {
                    QWidget *widget = l->itemAt(i)->widget();
                    if(widget == box){
                        tmp = tmp + tmpLabel + "_" + box->text() + ",";
                    }
                }
            }




        }
    }

    tmp.chop(1);
    qDebug(tmp.toUtf8());


    int imgHashCode = qrand() % ((999999999 + 1) - 100000000) + 100000000;
    qDebug()<<imgHashCode;
    QString typeOfFile = ui->lineEdit_2->text().right(4);
    qDebug()<<typeOfFile;

    QFile::copy(ui->lineEdit_2->text(),"Гистология_Толстая кишка/"+QString::number(imgHashCode)+typeOfFile);

    if(!ui->checkBox_141->isChecked()){
        MainWindow::getDW().insertPatientWhileInsertingImage(ui->lineEdit_6->text(),ui->dateEdit->date(),ui->lineEdit_7->text().toInt(),
                                                             ui->lineEdit_8->text().toInt(),ui->radioButton->text(),
                                                             ui->comboBox->currentText(),ui->comboBox_2->currentText(),
                                                             "Толстая кишка","Толстая кишка");
        pat_id = MainWindow::getDW().getLastInsertedPatient();
    }

    MainWindow::getDW().insertImage(ui->comboBox->currentText(),ui->comboBox_2->currentText(),ui->comboBox_3->currentText(),
                                    ui->comboBox_4->currentText(),ui->comboBox_5->currentText(),QDate::currentDate(),
                                    QString::number(imgHashCode)+typeOfFile,pat_id,"Толстая кишка");

    this->close();
    //MainWindow::getDW.insertImage(tmp.toUtf8(),ui->textEdit_2->toPlainText(),ui->lineEdit_2->text(),ui->textEdit->toPlainText(),ui->lineEdit->text());

}

void Colon::on_pushButton_3_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
    this,
    tr("Open File"),
    "",
    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
    );

    ui->lineEdit_2->setText(imagePath);
}
