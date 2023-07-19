#include "diagnosisfilter.h"
#include "ui_diagnosisfilter.h"
#include "dbworker.h"
#include "mainwindow.h"
#include <QCheckBox>
#include <QVBoxLayout>

DiagnosisFilter::DiagnosisFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DiagnosisFilter)
{
    ui->setupUi(this);
    QPair<QStringList,QStringList> q = MainWindow::getDW().fillDiagnosisInComboBoxes(MainWindow::getTable());


        for(int i = 0; i < q.first.size(); ++i){

            if(q.first.at(i)!=""){
                QCheckBox *ch1 = new QCheckBox(this);
                ch1->setText(q.first.at(i));
                ui->verticalLayout->addWidget(ch1);
            }
        }

        for(int i = 0; i < q.second.size(); ++i){

            if(q.second.at(i)!=""){
                QCheckBox *ch1 = new QCheckBox(this);
                ch1->setText(q.second.at(i));
                ui->verticalLayout_2->addWidget(ch1);
            }
        }


        if(ui->verticalLayout->count()==0 && ui->verticalLayout_2->count()==0){
            QMessageBox::critical(0,
                                  "Critical", "Нет диагнозов по данному органу(база мала или неверно заполнена)");
        }


}

DiagnosisFilter::~DiagnosisFilter()
{
    delete ui;
}

void DiagnosisFilter::on_pushButton_clicked()
{
    this->close();
}

void DiagnosisFilter::on_pushButton_2_clicked()
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
                        tmp = tmp + box->text() + ",";
                    }
                }

                if(tmpLabel=="Гистологические диагнозы:"){
                    MainWindow::setGystoDiagnosisString(MainWindow::getGystoDiagnosisString() + tmp);
                    tmp = "";
                }

                if(tmpLabel=="Цитологические диагнозы:"){
                    MainWindow::setCytoDiagnosisString(MainWindow::getCytoDiagnosisString() + tmp);
                    tmp = "";
                }


            }
        }
    }


    QString cytFilter = MainWindow::getCytoDiagnosisString();
    cytFilter.chop(1);
    QString gystFilter = MainWindow::getGystoDiagnosisString();
    gystFilter.chop(1);
    MainWindow::setGystoDiagnosisString(gystFilter);
    MainWindow::setCytoDiagnosisString(cytFilter);
    qDebug(tmp.toUtf8());
    this->close();
}
