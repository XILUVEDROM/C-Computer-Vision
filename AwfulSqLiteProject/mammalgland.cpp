#include "mammalgland.h"
#include "ui_mammalgland.h"
#include "mainwindow.h"
#include <QFileDialog>

MammalGland::MammalGland(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MammalGland)
{
    ui->setupUi(this);
}

MammalGland::~MammalGland()
{
    delete ui;
}

void MammalGland::on_pushButton_2_clicked()
{
    this->close();
}

void MammalGland::on_pushButton_clicked()
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

    QFile::copy(ui->lineEdit_2->text(),"MammalGland/"+QString::number(imgHashCode)+typeOfFile);

    /*MainWindow::getDW().insertImage(tmp.toUtf8(),ui->textEdit_2->toPlainText(),
                                    "MammalGland/"+QString::number(imgHashCode)+typeOfFile,
                                    ui->textEdit->toPlainText(),ui->lineEdit->text(),"Молочная железа");*/
    this->close();
}

void MammalGland::on_pushButton_3_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
    this,
    tr("Open File"),
    "",
    tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
    );

    ui->lineEdit_2->setText(imagePath);
}
