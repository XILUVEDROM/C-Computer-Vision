#include "colonfilter.h"
#include "ui_colonfilter.h"
#include "mainwindow.h"

ColonFilter::ColonFilter(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColonFilter)
{
    ui->setupUi(this);
}

ColonFilter::~ColonFilter()
{
    delete ui;
}

void ColonFilter::on_pushButton_2_clicked()
{
    this->close();
}

void ColonFilter::on_pushButton_clicked()
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
    MainWindow::setFilterString(tmp.toUtf8());

    this->close();

}
