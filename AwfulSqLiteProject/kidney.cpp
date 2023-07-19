#include "kidney.h"
#include "ui_kidney.h"
#include "mainwindow.h"
#include <QFileDialog>

Kidney::Kidney(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kidney)
{
    ui->setupUi(this);

}

Kidney::~Kidney()
{
    delete ui;
}

void Kidney::on_pushButton_2_clicked()
{
    this->close();
}

void Kidney::on_pushButton_clicked()
{
    QString tmp = "";


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

    QFile::copy(ui->lineEdit_2->text(),"Kidney/"+QString::number(imgHashCode)+typeOfFile);

    /*MainWindow::getDW().insertImage(tmp.toUtf8(),ui->textEdit_2->toPlainText(),
                                    "Kidney/"+QString::number(imgHashCode)+typeOfFile,
                                    ui->textEdit->toPlainText(),ui->lineEdit->text(),"Почка"); */

    this->close();
}

void Kidney::on_pushButton_3_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
    this,
    tr("Open File"),
    "",
    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
    );

    ui->lineEdit_2->setText(imagePath);
}
