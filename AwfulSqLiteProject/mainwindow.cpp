#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dbworker.h"
#include "addimage.h"
#include "colonfilter.h"
#include "diagnosisfilter.h"
#include "kidneyfilter.h"
#include "mammalglandfilter.h"
#include "mycustomview.h"
#include "imagemarkup.h"
#include "analytics.h"
#include "analyticforcyto.h"
#include "analyticforgysto.h"
#include <QFileDialog>
#include <QMouseEvent>
#include <iostream>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QtGui>
#include <QThread>
#include <QWheelEvent>
#include <markupadder.h>
#include <QDir>
#include <QImage>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // прячем всю хуйню связанную с разметкой
    ui->lineEdit_4->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->textBrowser_6->setVisible(false);
    ui->pushButton_14->setVisible(false);
    ui->pushButton_15->setVisible(false);



    //for eventFilter
    ui->graphicsView->viewport()->installEventFilter(this);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView_2->viewport()->installEventFilter(this);
    ui->graphicsView_2->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView_3->viewport()->installEventFilter(this);
    ui->graphicsView_4->viewport()->installEventFilter(this);
    ui->graphicsView_5->viewport()->installEventFilter(this);
    ui->graphicsView_6->viewport()->installEventFilter(this);
    ui->graphicsView_7->viewport()->installEventFilter(this);
    ui->graphicsView_3->setStyleSheet("border: 3px solid blue;");
    ui->graphicsView_4->setStyleSheet("border: 3px solid blue;");
    ui->graphicsView_5->setStyleSheet("border: 3px solid blue;");
    ui->graphicsView_6->setStyleSheet("border: 3px solid blue;");
    ui->graphicsView_7->setStyleSheet("border: 3px solid blue;");
    if(dbworker::getUserRole()!="admin"){
        ui->action->setVisible(false);
        ui->action_4->setVisible(false);
    }
}



MainWindow::~MainWindow()
{
    delete ui;
}



int selectionIndex = 0;
int globalAllRows = 0;
bool MarkUpExists = false;
QString pathForInsertingMarkUp="";
QString tableName="Kidney";
QString typeOfImages = "Гистология";
QString selectionString = "";
QString filterString = "";
QString gystoDiagnosisString = "";
QString cytoDiagnosisString = "";
QString pathString="";
QStringList FeatureForAnalytics;
QStringList GystoDiagnosisForAnalytics;
QStringList CytoDiagnosisForAnalytics;
QVector<double> percentageFeatureForAnalytics;
QVector<double> GystoPercentageDiagnosisForAnalytics;
QVector<double> CytoPercentageDiagnosisForAnalytics;







bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{

    if (event->type() == QEvent::MouseButtonPress)
    {
        //QGraphicsView *edit = qobject_cast< QGraphicsView* >(watched); i guess a cant change connected widget

        if (watched == ui->graphicsView_3->viewport()){

            ui->graphicsView_3->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_4->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_5->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_6->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_7->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_4->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_5->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_6->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_7->setStyleSheet("border: 3px solid blue;");



            setCurrentId(getCurrentIdImageList()-5);
            QString currentPath = typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString();
            //костыли для возврата картинки в рисовалку
            pathString = currentPath;

            //проверка на существование маркеров!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //но сперва сбрасываем состояние флага в ноль just to be sure и скрываем всю хуйню повторно
            setCurrentMarkUpId(0);
            allFilesInsideMarkupFolder.clear();
            gystoDiagForMarkUp.clear();
            cytoDiagForMarkUp.clear();
            featuresForMarkUp.clear();
            ui->lineEdit_4->setVisible(false);
            ui->checkBox_2->setVisible(false);
            ui->textBrowser_6->setVisible(false);
            ui->pushButton_14->setVisible(false);
            ui->pushButton_15->setVisible(false);
            MarkUpExists=false;

            int pos = getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
            if(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)).exists()){
                MarkUpExists=true;
            }

            // если существует разметка, то 1) кладем в лист имена всех разметок внутри папки
            // 2) тащим в два листа соответствующие диагнозы из базы

            //1)


            if(MarkUpExists){

                //показываем всю хуйню
                ui->lineEdit_4->setVisible(true);
                ui->checkBox_2->setVisible(true);
                ui->textBrowser_6->setVisible(true);
                ui->pushButton_14->setVisible(true);
                ui->pushButton_15->setVisible(true);
                //--------------

                QDirIterator it(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                                     "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)));
                while (it.hasNext()){
                    QFile f(it.next());
                    allFilesInsideMarkupFolder.push_back(it.fileName().left(it.fileName().size()-4).toInt());
                    qDebug()<<it.fileName().left(it.fileName().size()-4).toInt();
                    qDebug()<<allFilesInsideMarkupFolder;
                }

                allFilesInsideMarkupFolder.removeFirst();
                allFilesInsideMarkupFolder.removeFirst();
                 //2)


                for(int i=0;i<allFilesInsideMarkupFolder.size();i++){
                    QPair<QString,QString> tmp = DW.selectCytoAndGystoDiagForMarkUp(allFilesInsideMarkupFolder.at(i));
                    QStringList tmp2 = DW.selectFeatureForMarkUp(allFilesInsideMarkupFolder.at(i));
                    featuresForMarkUp.push_back(tmp2.at(0));
                    gystoDiagForMarkUp.push_back(tmp.first);
                    cytoDiagForMarkUp.push_back(tmp.second);
                }

                setCurrentMarkUpId(0);
                // сначала загружаем первую в списке хуету
                ui->textBrowser_6->clear();
                ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
                ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                                  "Цитологический диагноз - %2"
                                                  "Признак на разметке - %3")
                                          .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(featuresForMarkUp.at(getCurrentMarkUpId())));



            } else {
                //если ничего, то сразу очищаем just to be sure
                setCurrentMarkUpId(0);
                allFilesInsideMarkupFolder.clear();
                gystoDiagForMarkUp.clear();
                cytoDiagForMarkUp.clear();
                featuresForMarkUp.clear();
            }





            //---------------------------------------------------------------------------------------------------------------------


            //getting zoom
            QString currentZoom = getTmpmodel()->record(getCurrentId()).value("zoomOfImage").toString();
            //getting colorOfProbe
            QString currentColorOfImage = getTmpmodel()->record(getCurrentId()).value("colorOfImage").toString();
            //getting all diagnosis
            QString currentCytoDiag = getTmpmodel()->record(getCurrentId()).value("cytDiag").toString();
            QString currentGystoDiag = getTmpmodel()->record(getCurrentId()).value("gystoDiag").toString();
            //getting features
            QString currentFirstFeature = getTmpmodel()->record(getCurrentId()).value("Feature").toString();

            QSqlTableModel *patModel = DW.selectPatientById((MainWindow*)parent(), getTmpmodel()->record(getCurrentId()).value("Patient_Id").toInt());
            ui->textBrowser_5->clear();
            ui->textBrowser_5->append(QString("Данные по пациенту: \n"
                                              "Номер истории - %1 \n"
                                              "Дата рождения - %2 \n"
                                              "Год заболевания - %3 \n"
                                              "Возраст - %4 \n"
                                              "Пол - %5 \n"
                                              "Гистологический диагноз - %6 \n"
                                              "Цитологический диагноз - %7 \n")
                                      .arg(patModel->record(0).value("historyNum").toString())
                                      .arg(patModel->record(0).value("dateofBirth").toString())
                                      .arg(patModel->record(0).value("dateOfFallingIll").toString())
                                      .arg(patModel->record(0).value("age").toString())
                                      .arg(patModel->record(0).value("sex").toString())
                                      .arg(patModel->record(0).value("gystoDiagn").toString())
                                      .arg(patModel->record(0).value("cytoDiagn").toString()));

            ui->lineEdit_3->setText(currentZoom);
            ui->lineEdit_7->setText(currentColorOfImage);
            ui->lineEdit->setText(currentCytoDiag);
            ui->lineEdit_2->setText(currentGystoDiag);
            ui->textBrowser->setText(currentFirstFeature);
            ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(globalAllRows));

            imageObject = new QImage();
            imageObject->load(currentPath);

            image = QPixmap::fromImage(*imageObject);

            scene = new QGraphicsScene(this);
            scene->addPixmap(image);
            scene->setSceneRect(image.rect());
            ui->graphicsView_2->setScene(scene);
            return false;
        }
        else if (watched == ui->graphicsView_4->viewport()){

            ui->graphicsView_3->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_4->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_5->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_6->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_7->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_3->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_5->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_6->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_7->setStyleSheet("border: 3px solid blue;");



            setCurrentId(getCurrentIdImageList()-4);
            QString currentPath = typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString();
            //костыли для возврата картинки в рисовалку
            pathString = currentPath;


            //проверка на существование маркеров!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //но сперва сбрасываем состояние флага в ноль just to be sure и скрываем всю хуйню повторно
            setCurrentMarkUpId(0);
            allFilesInsideMarkupFolder.clear();
            gystoDiagForMarkUp.clear();
            cytoDiagForMarkUp.clear();
            featuresForMarkUp.clear();
            ui->lineEdit_4->setVisible(false);
            ui->checkBox_2->setVisible(false);
            ui->textBrowser_6->setVisible(false);
            ui->pushButton_14->setVisible(false);
            ui->pushButton_15->setVisible(false);
            MarkUpExists=false;

            int pos = getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
            if(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)).exists()){
                MarkUpExists=true;
            }

            // если существует разметка, то 1) кладем в лист имена всех разметок внутри папки
            // 2) тащим в два листа соответствующие диагнозы из базы

            //1)


            if(MarkUpExists){

                //показываем всю хуйню
                ui->lineEdit_4->setVisible(true);
                ui->checkBox_2->setVisible(true);
                ui->textBrowser_6->setVisible(true);
                ui->pushButton_14->setVisible(true);
                ui->pushButton_15->setVisible(true);
                //--------------

                QDirIterator it(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                                     "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)));
                while (it.hasNext()){
                    QFile f(it.next());
                    allFilesInsideMarkupFolder.push_back(it.fileName().left(it.fileName().size()-4).toInt());
                    qDebug()<<it.fileName().left(it.fileName().size()-4).toInt();
                    qDebug()<<allFilesInsideMarkupFolder;
                }

                allFilesInsideMarkupFolder.removeFirst();
                allFilesInsideMarkupFolder.removeFirst();
                 //2)


                for(int i=0;i<allFilesInsideMarkupFolder.size();i++){
                    QPair<QString,QString> tmp = DW.selectCytoAndGystoDiagForMarkUp(allFilesInsideMarkupFolder.at(i));
                    QStringList tmp2 = DW.selectFeatureForMarkUp(allFilesInsideMarkupFolder.at(i));
                    featuresForMarkUp.push_back(tmp2.at(0));
                    gystoDiagForMarkUp.push_back(tmp.first);
                    cytoDiagForMarkUp.push_back(tmp.second);
                }

                setCurrentMarkUpId(0);
                // сначала загружаем первую в списке хуету
                ui->textBrowser_6->clear();
                ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
                ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                                  "Цитологический диагноз - %2"
                                                  "Признак на разметке - %3")
                                          .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(featuresForMarkUp.at(getCurrentMarkUpId())));



            } else {
                //если ничего, то сразу очищаем just to be sure
                setCurrentMarkUpId(0);
                allFilesInsideMarkupFolder.clear();
                gystoDiagForMarkUp.clear();
                cytoDiagForMarkUp.clear();
                featuresForMarkUp.clear();
            }





            //---------------------------------------------------------------------------------------------------------------------

            //getting zoom
            QString currentZoom = getTmpmodel()->record(getCurrentId()).value("zoomOfImage").toString();
            //getting colorOfProbe
            QString currentColorOfImage = getTmpmodel()->record(getCurrentId()).value("colorOfImage").toString();
            //getting all diagnosis
            QString currentCytoDiag = getTmpmodel()->record(getCurrentId()).value("cytDiag").toString();
            QString currentGystoDiag = getTmpmodel()->record(getCurrentId()).value("gystoDiag").toString();
            //getting features
            QString currentFirstFeature = getTmpmodel()->record(getCurrentId()).value("Feature").toString();

            QSqlTableModel *patModel = DW.selectPatientById((MainWindow*)parent(), getTmpmodel()->record(getCurrentId()).value("Patient_Id").toInt());
            ui->textBrowser_5->clear();
            ui->textBrowser_5->append(QString("Данные по пациенту: \n"
                                              "Номер истории - %1 \n"
                                              "Дата рождения - %2 \n"
                                              "Год заболевания - %3 \n"
                                              "Возраст - %4 \n"
                                              "Пол - %5 \n"
                                              "Гистологический диагноз - %6 \n"
                                              "Цитологический диагноз - %7 \n")
                                      .arg(patModel->record(0).value("historyNum").toString())
                                      .arg(patModel->record(0).value("dateofBirth").toString())
                                      .arg(patModel->record(0).value("dateOfFallingIll").toString())
                                      .arg(patModel->record(0).value("age").toString())
                                      .arg(patModel->record(0).value("sex").toString())
                                      .arg(patModel->record(0).value("gystoDiagn").toString())
                                      .arg(patModel->record(0).value("cytoDiagn").toString()));

            ui->lineEdit_3->setText(currentZoom);
            ui->lineEdit_7->setText(currentColorOfImage);
            ui->lineEdit->setText(currentCytoDiag);
            ui->lineEdit_2->setText(currentGystoDiag);
            ui->textBrowser->setText(currentFirstFeature);
            ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(globalAllRows));


            imageObject = new QImage();
            imageObject->load(currentPath);

            image = QPixmap::fromImage(*imageObject);

            scene = new QGraphicsScene(this);
            scene->addPixmap(image);
            scene->setSceneRect(image.rect());
            ui->graphicsView_2->setScene(scene);
            return false;
        }
        else if (watched == ui->graphicsView_5->viewport()){

            ui->graphicsView_3->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_4->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_5->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_6->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_7->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_3->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_4->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_6->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_7->setStyleSheet("border: 3px solid blue;");


            setCurrentId(getCurrentIdImageList()-3);
            QString currentPath = typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString();
            //костыли для возврата картинки в рисовалку
            pathString = currentPath;


            //проверка на существование маркеров!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //но сперва сбрасываем состояние флага в ноль just to be sure и скрываем всю хуйню повторно
            setCurrentMarkUpId(0);
            allFilesInsideMarkupFolder.clear();
            gystoDiagForMarkUp.clear();
            cytoDiagForMarkUp.clear();
            featuresForMarkUp.clear();
            ui->lineEdit_4->setVisible(false);
            ui->checkBox_2->setVisible(false);
            ui->textBrowser_6->setVisible(false);
            ui->pushButton_14->setVisible(false);
            ui->pushButton_15->setVisible(false);
            MarkUpExists=false;

            int pos = getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
            if(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)).exists()){
                MarkUpExists=true;
            }

            // если существует разметка, то 1) кладем в лист имена всех разметок внутри папки
            // 2) тащим в два листа соответствующие диагнозы из базы

            //1)


            if(MarkUpExists){

                //показываем всю хуйню
                ui->lineEdit_4->setVisible(true);
                ui->checkBox_2->setVisible(true);
                ui->textBrowser_6->setVisible(true);
                ui->pushButton_14->setVisible(true);
                ui->pushButton_15->setVisible(true);
                //--------------

                QDirIterator it(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                                     "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)));
                while (it.hasNext()){
                    QFile f(it.next());
                    allFilesInsideMarkupFolder.push_back(it.fileName().left(it.fileName().size()-4).toInt());
                    qDebug()<<it.fileName().left(it.fileName().size()-4).toInt();
                    qDebug()<<allFilesInsideMarkupFolder;
                }

                allFilesInsideMarkupFolder.removeFirst();
                allFilesInsideMarkupFolder.removeFirst();
                 //2)


                for(int i=0;i<allFilesInsideMarkupFolder.size();i++){
                    QPair<QString,QString> tmp = DW.selectCytoAndGystoDiagForMarkUp(allFilesInsideMarkupFolder.at(i));
                    QStringList tmp2 = DW.selectFeatureForMarkUp(allFilesInsideMarkupFolder.at(i));
                    featuresForMarkUp.push_back(tmp2.at(0));
                    gystoDiagForMarkUp.push_back(tmp.first);
                    cytoDiagForMarkUp.push_back(tmp.second);
                }

                setCurrentMarkUpId(0);
                // сначала загружаем первую в списке хуету
                ui->textBrowser_6->clear();
                ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
                ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                                  "Цитологический диагноз - %2"
                                                  "Признак на разметке - %3")
                                          .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(featuresForMarkUp.at(getCurrentMarkUpId())));



            } else {
                //если ничего, то сразу очищаем just to be sure
                setCurrentMarkUpId(0);
                allFilesInsideMarkupFolder.clear();
                gystoDiagForMarkUp.clear();
                cytoDiagForMarkUp.clear();
                featuresForMarkUp.clear();
            }





            //---------------------------------------------------------------------------------------------------------------------

            //getting zoom
            QString currentZoom = getTmpmodel()->record(getCurrentId()).value("zoomOfImage").toString();
            //getting colorOfProbe
            QString currentColorOfImage = getTmpmodel()->record(getCurrentId()).value("colorOfImage").toString();
            //getting all diagnosis
            QString currentCytoDiag = getTmpmodel()->record(getCurrentId()).value("cytDiag").toString();
            QString currentGystoDiag = getTmpmodel()->record(getCurrentId()).value("gystoDiag").toString();
            //getting features
            QString currentFirstFeature = getTmpmodel()->record(getCurrentId()).value("Feature").toString();

            QSqlTableModel *patModel = DW.selectPatientById((MainWindow*)parent(), getTmpmodel()->record(getCurrentId()).value("Patient_Id").toInt());
            ui->textBrowser_5->clear();
            ui->textBrowser_5->append(QString("Данные по пациенту: \n"
                                              "Номер истории - %1 \n"
                                              "Дата рождения - %2 \n"
                                              "Год заболевания - %3 \n"
                                              "Возраст - %4 \n"
                                              "Пол - %5 \n"
                                              "Гистологический диагноз - %6 \n"
                                              "Цитологический диагноз - %7 \n")
                                      .arg(patModel->record(0).value("historyNum").toString())
                                      .arg(patModel->record(0).value("dateofBirth").toString())
                                      .arg(patModel->record(0).value("dateOfFallingIll").toString())
                                      .arg(patModel->record(0).value("age").toString())
                                      .arg(patModel->record(0).value("sex").toString())
                                      .arg(patModel->record(0).value("gystoDiagn").toString())
                                      .arg(patModel->record(0).value("cytoDiagn").toString()));

            ui->lineEdit_3->setText(currentZoom);
            ui->lineEdit_7->setText(currentColorOfImage);
            ui->lineEdit->setText(currentCytoDiag);
            ui->lineEdit_2->setText(currentGystoDiag);
            ui->textBrowser->setText(currentFirstFeature);
            ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(globalAllRows));


            imageObject = new QImage();
            imageObject->load(currentPath);

            image = QPixmap::fromImage(*imageObject);

            scene = new QGraphicsScene(this);
            scene->addPixmap(image);
            scene->setSceneRect(image.rect());
            ui->graphicsView_2->setScene(scene);
            return false;
        }
        else if (watched == ui->graphicsView_6->viewport()){

            ui->graphicsView_3->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_4->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_5->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_6->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_7->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_3->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_4->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_5->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_7->setStyleSheet("border: 3px solid blue;");


            setCurrentId(getCurrentIdImageList()-2);
            QString currentPath = typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString();
            //костыли для возврата картинки в рисовалку
            pathString = currentPath;


            //проверка на существование маркеров!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //но сперва сбрасываем состояние флага в ноль just to be sure и скрываем всю хуйню повторно
            setCurrentMarkUpId(0);
            allFilesInsideMarkupFolder.clear();
            gystoDiagForMarkUp.clear();
            cytoDiagForMarkUp.clear();
            featuresForMarkUp.clear();
            ui->lineEdit_4->setVisible(false);
            ui->checkBox_2->setVisible(false);
            ui->textBrowser_6->setVisible(false);
            ui->pushButton_14->setVisible(false);
            ui->pushButton_15->setVisible(false);
            MarkUpExists=false;

            int pos = getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
            if(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)).exists()){
                MarkUpExists=true;
            }

            // если существует разметка, то 1) кладем в лист имена всех разметок внутри папки
            // 2) тащим в два листа соответствующие диагнозы из базы

            //1)


            if(MarkUpExists){

                //показываем всю хуйню
                ui->lineEdit_4->setVisible(true);
                ui->checkBox_2->setVisible(true);
                ui->textBrowser_6->setVisible(true);
                ui->pushButton_14->setVisible(true);
                ui->pushButton_15->setVisible(true);
                //--------------

                QDirIterator it(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                                     "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)));
                while (it.hasNext()){
                    QFile f(it.next());
                    allFilesInsideMarkupFolder.push_back(it.fileName().left(it.fileName().size()-4).toInt());
                    qDebug()<<it.fileName().left(it.fileName().size()-4).toInt();
                    qDebug()<<allFilesInsideMarkupFolder;
                }

                allFilesInsideMarkupFolder.removeFirst();
                allFilesInsideMarkupFolder.removeFirst();
                 //2)


                for(int i=0;i<allFilesInsideMarkupFolder.size();i++){
                    QPair<QString,QString> tmp = DW.selectCytoAndGystoDiagForMarkUp(allFilesInsideMarkupFolder.at(i));
                    QStringList tmp2 = DW.selectFeatureForMarkUp(allFilesInsideMarkupFolder.at(i));
                    featuresForMarkUp.push_back(tmp2.at(0));
                    gystoDiagForMarkUp.push_back(tmp.first);
                    cytoDiagForMarkUp.push_back(tmp.second);
                }

                setCurrentMarkUpId(0);
                // сначала загружаем первую в списке хуету
                ui->textBrowser_6->clear();
                ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
                ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                                  "Цитологический диагноз - %2"
                                                  "Признак на разметке - %3")
                                          .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(featuresForMarkUp.at(getCurrentMarkUpId())));



            } else {
                //если ничего, то сразу очищаем just to be sure
                setCurrentMarkUpId(0);
                allFilesInsideMarkupFolder.clear();
                gystoDiagForMarkUp.clear();
                cytoDiagForMarkUp.clear();
                featuresForMarkUp.clear();
            }





            //---------------------------------------------------------------------------------------------------------------------
            //getting zoom
            QString currentZoom = getTmpmodel()->record(getCurrentId()).value("zoomOfImage").toString();
            //getting colorOfProbe
            QString currentColorOfImage = getTmpmodel()->record(getCurrentId()).value("colorOfImage").toString();
            //getting all diagnosis
            QString currentCytoDiag = getTmpmodel()->record(getCurrentId()).value("cytDiag").toString();
            QString currentGystoDiag = getTmpmodel()->record(getCurrentId()).value("gystoDiag").toString();
            //getting features
            QString currentFirstFeature = getTmpmodel()->record(getCurrentId()).value("Feature").toString();

            QSqlTableModel *patModel = DW.selectPatientById((MainWindow*)parent(), getTmpmodel()->record(getCurrentId()).value("Patient_Id").toInt());
            ui->textBrowser_5->clear();
            ui->textBrowser_5->append(QString("Данные по пациенту: \n"
                                              "Номер истории - %1 \n"
                                              "Дата рождения - %2 \n"
                                              "Год заболевания - %3 \n"
                                              "Возраст - %4 \n"
                                              "Пол - %5 \n"
                                              "Гистологический диагноз - %6 \n"
                                              "Цитологический диагноз - %7 \n")
                                      .arg(patModel->record(0).value("historyNum").toString())
                                      .arg(patModel->record(0).value("dateofBirth").toString())
                                      .arg(patModel->record(0).value("dateOfFallingIll").toString())
                                      .arg(patModel->record(0).value("age").toString())
                                      .arg(patModel->record(0).value("sex").toString())
                                      .arg(patModel->record(0).value("gystoDiagn").toString())
                                      .arg(patModel->record(0).value("cytoDiagn").toString()));

            ui->lineEdit_3->setText(currentZoom);
            ui->lineEdit_7->setText(currentColorOfImage);
            ui->lineEdit->setText(currentCytoDiag);
            ui->lineEdit_2->setText(currentGystoDiag);
            ui->textBrowser->setText(currentFirstFeature);
            ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(globalAllRows));


            imageObject = new QImage();
            imageObject->load(currentPath);

            image = QPixmap::fromImage(*imageObject);

            scene = new QGraphicsScene(this);
            scene->addPixmap(image);
            scene->setSceneRect(image.rect());
            ui->graphicsView_2->setScene(scene);
            return false;
        }
        else if (watched == ui->graphicsView_7->viewport()){

            ui->graphicsView_3->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_4->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_5->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_6->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_7->setStyleSheet("border: 3px solid black;");
            ui->graphicsView_3->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_4->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_5->setStyleSheet("border: 3px solid blue;");
            ui->graphicsView_6->setStyleSheet("border: 3px solid blue;");


            setCurrentId(getCurrentIdImageList()-1);
            QString currentPath = typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString();
            //костыли для возврата картинки в рисовалку
            pathString = currentPath;

            //проверка на существование маркеров!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //но сперва сбрасываем состояние флага в ноль just to be sure и скрываем всю хуйню повторно
            setCurrentMarkUpId(0);
            allFilesInsideMarkupFolder.clear();
            gystoDiagForMarkUp.clear();
            cytoDiagForMarkUp.clear();
            featuresForMarkUp.clear();
            ui->lineEdit_4->setVisible(false);
            ui->checkBox_2->setVisible(false);
            ui->textBrowser_6->setVisible(false);
            ui->pushButton_14->setVisible(false);
            ui->pushButton_15->setVisible(false);
            MarkUpExists=false;

            int pos = getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
            if(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                    "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)).exists()){
                MarkUpExists=true;
            }

            // если существует разметка, то 1) кладем в лист имена всех разметок внутри папки
            // 2) тащим в два листа соответствующие диагнозы из базы

            //1)


            if(MarkUpExists){

                //показываем всю хуйню
                ui->lineEdit_4->setVisible(true);
                ui->checkBox_2->setVisible(true);
                ui->textBrowser_6->setVisible(true);
                ui->pushButton_14->setVisible(true);
                ui->pushButton_15->setVisible(true);
                //--------------

                QDirIterator it(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                                     "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)));
                while (it.hasNext()){
                    QFile f(it.next());
                    allFilesInsideMarkupFolder.push_back(it.fileName().left(it.fileName().size()-4).toInt());
                    qDebug()<<it.fileName().left(it.fileName().size()-4).toInt();
                    qDebug()<<allFilesInsideMarkupFolder;
                }

                allFilesInsideMarkupFolder.removeFirst();
                allFilesInsideMarkupFolder.removeFirst();
                 //2)


                for(int i=0;i<allFilesInsideMarkupFolder.size();i++){
                    QPair<QString,QString> tmp = DW.selectCytoAndGystoDiagForMarkUp(allFilesInsideMarkupFolder.at(i));
                    QStringList tmp2 = DW.selectFeatureForMarkUp(allFilesInsideMarkupFolder.at(i));
                    featuresForMarkUp.push_back(tmp2.at(0));
                    gystoDiagForMarkUp.push_back(tmp.first);
                    cytoDiagForMarkUp.push_back(tmp.second);
                }

                setCurrentMarkUpId(0);
                // сначала загружаем первую в списке хуету
                ui->textBrowser_6->clear();
                ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
                ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                                  "Цитологический диагноз - %2"
                                                  "Признак на разметке - %3")
                                          .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                                          .arg(featuresForMarkUp.at(getCurrentMarkUpId())));



            } else {
                //если ничего, то сразу очищаем just to be sure
                setCurrentMarkUpId(0);
                allFilesInsideMarkupFolder.clear();
                gystoDiagForMarkUp.clear();
                cytoDiagForMarkUp.clear();
                featuresForMarkUp.clear();
            }





            //---------------------------------------------------------------------------------------------------------------------

            //getting zoom
            QString currentZoom = getTmpmodel()->record(getCurrentId()).value("zoomOfImage").toString();
            //getting colorOfProbe
            QString currentColorOfImage = getTmpmodel()->record(getCurrentId()).value("colorOfImage").toString();
            //getting all diagnosis
            QString currentCytoDiag = getTmpmodel()->record(getCurrentId()).value("cytDiag").toString();
            QString currentGystoDiag = getTmpmodel()->record(getCurrentId()).value("gystoDiag").toString();
            //getting features
            QString currentFirstFeature = getTmpmodel()->record(getCurrentId()).value("Feature").toString();

            QSqlTableModel *patModel = DW.selectPatientById((MainWindow*)parent(), getTmpmodel()->record(getCurrentId()).value("Patient_Id").toInt());
            ui->textBrowser_5->clear();
            ui->textBrowser_5->append(QString("Данные по пациенту: \n"
                                              "Номер истории - %1 \n"
                                              "Дата рождения - %2 \n"
                                              "Год заболевания - %3 \n"
                                              "Возраст - %4 \n"
                                              "Пол - %5 \n"
                                              "Гистологический диагноз - %6 \n"
                                              "Цитологический диагноз - %7 \n")
                                      .arg(patModel->record(0).value("historyNum").toString())
                                      .arg(patModel->record(0).value("dateofBirth").toString())
                                      .arg(patModel->record(0).value("dateOfFallingIll").toString())
                                      .arg(patModel->record(0).value("age").toString())
                                      .arg(patModel->record(0).value("sex").toString())
                                      .arg(patModel->record(0).value("gystoDiagn").toString())
                                      .arg(patModel->record(0).value("cytoDiagn").toString()));

            ui->lineEdit_3->setText(currentZoom);
            ui->lineEdit_7->setText(currentColorOfImage);
            ui->lineEdit->setText(currentCytoDiag);
            ui->lineEdit_2->setText(currentGystoDiag);
            ui->textBrowser->setText(currentFirstFeature);
            ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(globalAllRows));


            imageObject = new QImage();
            imageObject->load(currentPath);

            image = QPixmap::fromImage(*imageObject);

            scene = new QGraphicsScene(this);
            scene->addPixmap(image);
            scene->setSceneRect(image.rect());
            ui->graphicsView_2->setScene(scene);
            return false;
        }
    }
    if (event->type() == QEvent::Wheel){
        if (watched == ui->graphicsView->viewport()){

            QWheelEvent *wEvent = static_cast<QWheelEvent*>(event);
            if(wEvent->delta()>0){
                ui->graphicsView->scale(1.1,1.1);
            } else {
                ui->graphicsView->scale(0.9,0.9);
            }


        }
        if (watched == ui->graphicsView_2->viewport()){

            QWheelEvent *wEvent = static_cast<QWheelEvent*>(event);
            if(wEvent->delta()>0){
                ui->graphicsView_2->scale(1.1,1.1);
            } else {
                ui->graphicsView_2->scale(0.9,0.9);
            }

        }
    }


    return QMainWindow::eventFilter(watched, event);
}

QString MainWindow::getTable()
{
    return tableName;
}





QString MainWindow::getImagePathForMarkup()
{
    return pathString;
}

void MainWindow::setPathForInsertingMarkUp(const QString &value)
{
    pathForInsertingMarkUp = value;
}

QStringList MainWindow::getFeatureForAnalytics()
{
    return FeatureForAnalytics;
}

QStringList MainWindow::getGystoDiagnosisForAnalytic()
{
    return GystoDiagnosisForAnalytics;
}

QStringList MainWindow::getCytoDiagnosisForAnalytic()
{
    return CytoDiagnosisForAnalytics;
}

QVector<double> MainWindow::getPercentageFeatureForAnalytics()
{
    return percentageFeatureForAnalytics;
}

QVector<double> MainWindow::getGystoPercentageDiagnosisForAnalytics()
{
    return GystoPercentageDiagnosisForAnalytics;
}

QVector<double> MainWindow::getCytoPercentageDiagnosisForAnalytics()
{
    return CytoPercentageDiagnosisForAnalytics;
}





QString MainWindow::getFilterString()
{
    return filterString;
}

void MainWindow::setFilterString(const QString &value)
{
    filterString = value;
}

void MainWindow::setGystoDiagnosisString(const QString &value)
{
    gystoDiagnosisString = value;
}

QString MainWindow::getGystoDiagnosisString()
{
    return gystoDiagnosisString;
}

void MainWindow::setCytoDiagnosisString(const QString &value)
{
    cytoDiagnosisString = value;
}

QString MainWindow::getCytoDiagnosisString()
{
    return cytoDiagnosisString;
}


// Форма для добавления изображений
void MainWindow::on_action_triggered()
{
    AddImage *ad = new AddImage();
    ad->show();
}

void MainWindow::on_pushButton_clicked()
{
    QString imagePath = QFileDialog::getOpenFileName(
                this,
                tr("Open File"),
                "",
                tr("JPEG (*.jpg *.jpeg);;PNG (*.png)" )
                );

    imageObject = new QImage();
    imageObject->load(imagePath);

    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView->setScene(scene);
}

void MainWindow::on_pushButton_6_clicked()
{
    ui->graphicsView->scale(1.1,1.1);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->graphicsView->scale(0.9,0.9);
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->graphicsView_2->scale(1.1,1.1);
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->graphicsView_2->scale(0.9,0.9);
}

int MainWindow::getCurrentId() const
{
    return currentId;
}

void MainWindow::setCurrentId(int value)
{
    currentId = value;
}

QSqlTableModel *MainWindow::getTmpmodel() const
{
    return tmpmodel;
}

void MainWindow::setTmpmodel(QSqlTableModel *value)
{
    tmpmodel = value;
}


void MainWindow::on_pushButton_4_clicked()
{

    ui->textBrowser_3->setText(filterString);

    QSqlTableModel *model = DW.selectImage((MainWindow*)parent(), filterString, tableName);
    setTmpmodel(model);
    setCurrentId(0);
    setCurrentIdImageList(0);

    QSqlTableModel *patModel = DW.selectPatientById((MainWindow*)parent(), model->record(getCurrentId()).value("Patient_Id").toInt());
    ui->textBrowser_5->clear();
    ui->textBrowser_5->append(QString("Данные по пациенту: \n"
                                      "Номер истории - %1 \n"
                                      "Дата рождения - %2 \n"
                                      "Год заболевания - %3 \n"
                                      "Возраст - %4 \n"
                                      "Пол - %5 \n"
                                      "Гистологический диагноз - %6 \n"
                                      "Цитологический диагноз - %7 \n")
                              .arg(patModel->record(0).value("historyNum").toString())
                              .arg(patModel->record(0).value("dateofBirth").toString())
                              .arg(patModel->record(0).value("dateOfFallingIll").toString())
                              .arg(patModel->record(0).value("age").toString())
                              .arg(patModel->record(0).value("sex").toString())
                              .arg(patModel->record(0).value("gystoDiagn").toString())
                              .arg(patModel->record(0).value("cytoDiagn").toString()));


    //проверка на существование маркеров!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //но сперва сбрасываем состояние флага в ноль just to be sure и скрываем всю хуйню повторно
    setCurrentMarkUpId(0);
    allFilesInsideMarkupFolder.clear();
    gystoDiagForMarkUp.clear();
    cytoDiagForMarkUp.clear();
    featuresForMarkUp.clear();
    ui->lineEdit_4->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->textBrowser_6->setVisible(false);
    ui->pushButton_14->setVisible(false);
    ui->pushButton_15->setVisible(false);
    MarkUpExists=false;

    int pos = model->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
    if(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
            "\\TImages\\" + model->record(getCurrentId()).value("linkToImage").toString().left(pos)).exists()){
        MarkUpExists=true;
    }

    // если существует разметка, то 1) кладем в лист имена всех разметок внутри папки
    // 2) тащим в два листа соответствующие диагнозы из базы

    //1)


    if(MarkUpExists){

        //показываем всю хуйню
        ui->lineEdit_4->setVisible(true);
        ui->checkBox_2->setVisible(true);
        ui->textBrowser_6->setVisible(true);
        ui->pushButton_14->setVisible(true);
        ui->pushButton_15->setVisible(true);
        //--------------

        QDirIterator it(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                             "\\TImages\\" + model->record(getCurrentId()).value("linkToImage").toString().left(pos)));
        while (it.hasNext()){
            QFile f(it.next());
            allFilesInsideMarkupFolder.push_back(it.fileName().left(it.fileName().size()-4).toInt());
            qDebug()<<it.fileName().left(it.fileName().size()-4).toInt();
            qDebug()<<allFilesInsideMarkupFolder;
        }

        allFilesInsideMarkupFolder.removeFirst();
        allFilesInsideMarkupFolder.removeFirst();
         //2)


        for(int i=0;i<allFilesInsideMarkupFolder.size();i++){
            QPair<QString,QString> tmp = DW.selectCytoAndGystoDiagForMarkUp(allFilesInsideMarkupFolder.at(i));
            QStringList tmp2 = DW.selectFeatureForMarkUp(allFilesInsideMarkupFolder.at(i));
            featuresForMarkUp.push_back(tmp2.at(0));
            gystoDiagForMarkUp.push_back(tmp.first);
            cytoDiagForMarkUp.push_back(tmp.second);
        }

        setCurrentMarkUpId(0);
        // сначала загружаем первую в списке хуету
        ui->textBrowser_6->clear();
        ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
        ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                          "Цитологический диагноз - %2"
                                          "Признак на разметке - %3")
                                  .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                                  .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                                  .arg(featuresForMarkUp.at(getCurrentMarkUpId())));



    } else {
        //если ничего, то сразу очищаем just to be sure
        setCurrentMarkUpId(0);
        allFilesInsideMarkupFolder.clear();
        gystoDiagForMarkUp.clear();
        cytoDiagForMarkUp.clear();
        featuresForMarkUp.clear();
    }





    //---------------------------------------------------------------------------------------------------------------------




    QString firstPath = typeOfImages + "_" + ui->comboBox_4->currentText() +
            "\\TImages\\" + model->record(getCurrentId()).value("linkToImage").toString();
    //костыли для возврата картинки в рисовалку
    pathString = firstPath;


    //getting zoom
    QString zoom = model->record(getCurrentId()).value("zoomOfImage").toString();
    //getting colorOfProbe
    QString colorOfImage = model->record(getCurrentId()).value("colorOfImage").toString();
    //getting all diagnosis
    QString cytoDiag = model->record(getCurrentId()).value("cytDiag").toString();
    QString gystoDiag = model->record(getCurrentId()).value("gystoDiag").toString();
    //getting features
    QString firstFeature = model->record(getCurrentId()).value("Feature").toString();

    //showing params to doctor
    ui->lineEdit_3->setText(zoom);
    ui->lineEdit_7->setText(colorOfImage);
    ui->lineEdit->setText(cytoDiag);
    ui->lineEdit_2->setText(gystoDiag);
    //lets make form beautiful

    QStringList beatyForFeature = firstFeature.split(",");
    for(int i=0;i<beatyForFeature.size();i++){
        ui->textBrowser->append(beatyForFeature.at(i));
    }

    //loadin image
    imageObject = new QImage();
    imageObject->load(firstPath);

    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView_2->setScene(scene);

    //Statistics for Gystodiagnos selection
    ui->textBrowser_2->clear();
    int allRows = model->rowCount();
    globalAllRows = allRows;
    //ui->lineEdit_4->setText(QString::number(allRows));
    double dAllRows = (double) allRows;


    QVector<double> tmpvect;

    for(int i=0;i<allRows;i++){
        int counter = 0;
        for(int j=0;j<allRows;j++){
            if (model->record(i).value("gystoDiag").toString() == model->record(j).value("gystoDiag").toString()){
                counter++;
            }
        }

        tmpvect.append(100*counter/dAllRows);
    }


    QVector<QString> diagNames;
    for(int i=0;i<allRows;i++){
        diagNames.append(model->record(i).value("gystoDiag").toString());
    }

    QList<QPair<QString,double>> mightySelector;

    for(int i=0;i<allRows;i++){
        mightySelector.append(qMakePair(diagNames.at(i),tmpvect.at(i)));
    }

    //toSet then toList doesnt work apparently so
    qSort(mightySelector.begin(),mightySelector.end());
    mightySelector.erase(std::unique(mightySelector.begin(),mightySelector.end()), mightySelector.end());


    ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(allRows));
    ui->textBrowser_2->append("Статистика выборки по гистологическим диагнозам:");
    ui->textBrowser_2->append(QString("Всего диагнозов - %1 \n").arg(mightySelector.size()));

    for(int i=0;i<mightySelector.size();i++){
        ui->textBrowser_2->append(QString("Изображений с диагнозом %1 - %2% процентов \n")
                                  .arg(mightySelector.at(i).first).arg(mightySelector.at(i).second));
        //filling up list for analytic
        GystoDiagnosisForAnalytics.push_back(mightySelector.at(i).first);
        GystoPercentageDiagnosisForAnalytics.push_back(mightySelector.at(i).second);
    }

    tmpvect.clear();


    //Statistics for Cytodiagnos selection


    for(int i=0;i<allRows;i++){
        int counter = 0;
        for(int j=0;j<allRows;j++){
            if (model->record(i).value("cytDiag").toString() == model->record(j).value("cytDiag").toString()){
                counter++;
            }
        }

        tmpvect.append(100*counter/dAllRows);
    }

    QVector<QString> cytoDiagNames;
    for(int i=0;i<allRows;i++){
        cytoDiagNames.append(model->record(i).value("cytDiag").toString());
    }


    QList<QPair<QString,double>> mightierSelector;

    for(int i=0;i<allRows;i++){
        mightierSelector.append(qMakePair(cytoDiagNames.at(i),tmpvect.at(i)));
    }

    //toSet then toList doesnt work apparently so
    qSort(mightierSelector.begin(),mightierSelector.end());
    mightierSelector.erase(std::unique(mightierSelector.begin(),mightierSelector.end()), mightierSelector.end());


    ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(allRows));
    ui->textBrowser_2->append("________________________________________");
    ui->textBrowser_2->append("Статистика выборки по цитологическим диагнозам:");
    ui->textBrowser_2->append(QString("Всего диагнозов - %1 \n").arg(mightierSelector.size()));

    for(int i=0;i<mightierSelector.size();i++){
        ui->textBrowser_2->append(QString("Изображений с диагнозом %1 - %2% процентов \n")
                                  .arg(mightierSelector.at(i).first).arg(mightierSelector.at(i).second));
        //filling up list for analytic
        CytoDiagnosisForAnalytics.push_back(mightierSelector.at(i).first);
        CytoPercentageDiagnosisForAnalytics.push_back(mightierSelector.at(i).second);
    }


    tmpvect.clear();



    //Statistics for Feature selection

    QString tmpFeature;
    for (int i=0;i<allRows;i++){
        tmpFeature.append(model->record(i).value("Feature").toString() + ",");
    }
    tmpFeature.chop(1);

    QStringList q = tmpFeature.split(',');
    q = q.toSet().toList(); //getting rid of duplicates

    for(int i=0;i<q.size();i++){
        int counter = 0;
        for(int j=0;j<allRows;j++){
            if (model->record(j).value("Feature").toString().contains(q.value(i))){
                counter++;
            }
        }

        tmpvect.append(100*counter/dAllRows);
    }


    ui->textBrowser_2->append("___________________");
    ui->textBrowser_2->append("Статистика выборки по признакам: \n");
    ui->textBrowser_2->append(QString("Всего признаков - %1 \n").arg(q.size()));

    for(int i=0;i<tmpvect.size();i++){
        ui->textBrowser_2->append(QString("Изображений с признаком %1 - %2% процентов \n")
                                  .arg(q.value(i)).arg(tmpvect.value(i)));
    }


    //block for analytics
    FeatureForAnalytics = q;
    percentageFeatureForAnalytics = tmpvect;

    // Loading images in the bottom

    for(int i=getCurrentIdImageList();i<getCurrentIdImageList()+5;i++){


        imageObject = new QImage();
        imageObject->load(typeOfImages + "_" + ui->comboBox_4->currentText() +
                          "\\TImages\\" + model->record(i).value("linkToImage").toString());

        image = QPixmap::fromImage(*imageObject);

        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());


        // i dont know how to do this properly
        if(i==getCurrentIdImageList()){ui->graphicsView_3->setScene(scene);
                                       ui->graphicsView_3->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+1){ui->graphicsView_4->setScene(scene);
                                         ui->graphicsView_4->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+2){ui->graphicsView_5->setScene(scene);
                                         ui->graphicsView_5->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+3){ui->graphicsView_6->setScene(scene);
                                         ui->graphicsView_6->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+4){ui->graphicsView_7->setScene(scene);
                                         ui->graphicsView_7->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}




    }

    setCurrentIdImageList(getCurrentIdImageList()+5);

}

void MainWindow::on_pushButton_3_clicked()
{
    if(getTmpmodel()->record(getCurrentId()+1).value("linkToImage").toString() == ""){
        setCurrentId(0);
    } else {
        setCurrentId(getCurrentId()+1);
    }

    //проверка на существование маркеров!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //но сперва сбрасываем состояние флага в ноль just to be sure и скрываем всю хуйню повторно
    setCurrentMarkUpId(0);
    allFilesInsideMarkupFolder.clear();
    gystoDiagForMarkUp.clear();
    cytoDiagForMarkUp.clear();
    featuresForMarkUp.clear();
    ui->lineEdit_4->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->textBrowser_6->setVisible(false);
    ui->pushButton_14->setVisible(false);
    ui->pushButton_15->setVisible(false);
    MarkUpExists=false;

    int pos = getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
    if(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
            "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)).exists()){
        MarkUpExists=true;
    }

    // если существует разметка, то 1) кладем в лист имена всех разметок внутри папки
    // 2) тащим в два листа соответствующие диагнозы из базы

    //1)


    if(MarkUpExists){

        //показываем всю хуйню
        ui->lineEdit_4->setVisible(true);
        ui->checkBox_2->setVisible(true);
        ui->textBrowser_6->setVisible(true);
        ui->pushButton_14->setVisible(true);
        ui->pushButton_15->setVisible(true);
        //--------------

        QDirIterator it(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                             "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)));
        while (it.hasNext()){
            QFile f(it.next());
            allFilesInsideMarkupFolder.push_back(it.fileName().left(it.fileName().size()-4).toInt());
            qDebug()<<it.fileName().left(it.fileName().size()-4).toInt();
            qDebug()<<allFilesInsideMarkupFolder;
        }

        allFilesInsideMarkupFolder.removeFirst();
        allFilesInsideMarkupFolder.removeFirst();
         //2)


        for(int i=0;i<allFilesInsideMarkupFolder.size();i++){
            QPair<QString,QString> tmp = DW.selectCytoAndGystoDiagForMarkUp(allFilesInsideMarkupFolder.at(i));
            QStringList tmp2 = DW.selectFeatureForMarkUp(allFilesInsideMarkupFolder.at(i));
            featuresForMarkUp.push_back(tmp2.at(0));
            gystoDiagForMarkUp.push_back(tmp.first);
            cytoDiagForMarkUp.push_back(tmp.second);
        }

        setCurrentMarkUpId(0);
        // сначала загружаем первую в списке хуету
        ui->textBrowser_6->clear();
        ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
        ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                          "Цитологический диагноз - %2"
                                          "Признак на разметке - %3")
                                  .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                                  .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                                  .arg(featuresForMarkUp.at(getCurrentMarkUpId())));



    } else {
        //если ничего, то сразу очищаем just to be sure
        setCurrentMarkUpId(0);
        allFilesInsideMarkupFolder.clear();
        gystoDiagForMarkUp.clear();
        cytoDiagForMarkUp.clear();
        featuresForMarkUp.clear();
    }





    //---------------------------------------------------------------------------------------------------------------------






    QString currentPath = typeOfImages + "_" + ui->comboBox_4->currentText() +
            "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString();
    //костыли для возврата картинки в рисовалку
    pathString = currentPath;


    //getting zoom
    QString currentZoom = getTmpmodel()->record(getCurrentId()).value("zoomOfImage").toString();
    //getting colorOfProbe
    QString currentColorOfImage = getTmpmodel()->record(getCurrentId()).value("colorOfImage").toString();
    //getting all diagnosis
    QString currentCytoDiag = getTmpmodel()->record(getCurrentId()).value("cytDiag").toString();
    QString currentGystoDiag = getTmpmodel()->record(getCurrentId()).value("gystoDiag").toString();
    //getting features
    QString currentFirstFeature = getTmpmodel()->record(getCurrentId()).value("Feature").toString();

    QSqlTableModel *patModel = DW.selectPatientById((MainWindow*)parent(), getTmpmodel()->record(getCurrentId()).value("Patient_Id").toInt());
    ui->textBrowser_5->clear();
    ui->textBrowser_5->append(QString("Данные по пациенту: \n"
                                      "Номер истории - %1 \n"
                                      "Дата рождения - %2 \n"
                                      "Год заболевания - %3 \n"
                                      "Возраст - %4 \n"
                                      "Пол - %5 \n"
                                      "Гистологический диагноз - %6 \n"
                                      "Цитологический диагноз - %7 \n")
                              .arg(patModel->record(0).value("historyNum").toString())
                              .arg(patModel->record(0).value("dateofBirth").toString())
                              .arg(patModel->record(0).value("dateOfFallingIll").toString())
                              .arg(patModel->record(0).value("age").toString())
                              .arg(patModel->record(0).value("sex").toString())
                              .arg(patModel->record(0).value("gystoDiagn").toString())
                              .arg(patModel->record(0).value("cytoDiagn").toString()));

    ui->lineEdit_3->setText(currentZoom);
    ui->lineEdit_7->setText(currentColorOfImage);
    ui->lineEdit->setText(currentCytoDiag);
    ui->lineEdit_2->setText(currentGystoDiag);
    ui->textBrowser->setText(currentFirstFeature);
    ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(globalAllRows));

    imageObject = new QImage();
    imageObject->load(currentPath);

    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView_2->setScene(scene);

}

void MainWindow::on_pushButton_2_clicked()
{
    if(getTmpmodel()->record(getCurrentId()-1).value("linkToImage").toString() == ""){
        setCurrentId(getTmpmodel()->rowCount()-1);
    } else {
        setCurrentId(getCurrentId()-1);
    }

    //проверка на существование маркеров!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //но сперва сбрасываем состояние флага в ноль just to be sure и скрываем всю хуйню повторно
    setCurrentMarkUpId(0);
    allFilesInsideMarkupFolder.clear();
    gystoDiagForMarkUp.clear();
    cytoDiagForMarkUp.clear();
    featuresForMarkUp.clear();
    ui->lineEdit_4->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->textBrowser_6->setVisible(false);
    ui->pushButton_14->setVisible(false);
    ui->pushButton_15->setVisible(false);
    MarkUpExists=false;

    int pos = getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
    if(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
            "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)).exists()){
        MarkUpExists=true;
    }

    // если существует разметка, то 1) кладем в лист имена всех разметок внутри папки
    // 2) тащим в два листа соответствующие диагнозы из базы

    //1)


    if(MarkUpExists){

        //показываем всю хуйню
        ui->lineEdit_4->setVisible(true);
        ui->checkBox_2->setVisible(true);
        ui->textBrowser_6->setVisible(true);
        ui->pushButton_14->setVisible(true);
        ui->pushButton_15->setVisible(true);
        //--------------

        QDirIterator it(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                             "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos)));
        while (it.hasNext()){
            QFile f(it.next());
            allFilesInsideMarkupFolder.push_back(it.fileName().left(it.fileName().size()-4).toInt());
            qDebug()<<it.fileName().left(it.fileName().size()-4).toInt();
            qDebug()<<allFilesInsideMarkupFolder;
        }

        allFilesInsideMarkupFolder.removeFirst();
        allFilesInsideMarkupFolder.removeFirst();
         //2)


        for(int i=0;i<allFilesInsideMarkupFolder.size();i++){
            QPair<QString,QString> tmp = DW.selectCytoAndGystoDiagForMarkUp(allFilesInsideMarkupFolder.at(i));
            QStringList tmp2 = DW.selectFeatureForMarkUp(allFilesInsideMarkupFolder.at(i));
            featuresForMarkUp.push_back(tmp2.at(0));
            gystoDiagForMarkUp.push_back(tmp.first);
            cytoDiagForMarkUp.push_back(tmp.second);
        }

        setCurrentMarkUpId(0);
        // сначала загружаем первую в списке хуету
        ui->textBrowser_6->clear();
        ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
        ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                          "Цитологический диагноз - %2"
                                          "Признак на разметке - %3")
                                  .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                                  .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                                  .arg(featuresForMarkUp.at(getCurrentMarkUpId())));



    } else {
        //если ничего, то сразу очищаем just to be sure
        setCurrentMarkUpId(0);
        allFilesInsideMarkupFolder.clear();
        gystoDiagForMarkUp.clear();
        cytoDiagForMarkUp.clear();
        featuresForMarkUp.clear();
    }





    //---------------------------------------------------------------------------------------------------------------------



    QString currentPath = typeOfImages + "_" + ui->comboBox_4->currentText() +
            "\\TImages\\" + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString();
    //костыли для возврата картинки в рисовалку
    pathString = currentPath;


    //getting zoom
    QString currentZoom = getTmpmodel()->record(getCurrentId()).value("zoomOfImage").toString();
    //getting colorOfProbe
    QString currentColorOfImage = getTmpmodel()->record(getCurrentId()).value("colorOfImage").toString();
    //getting all diagnosis
    QString currentCytoDiag = getTmpmodel()->record(getCurrentId()).value("cytDiag").toString();
    QString currentGystoDiag = getTmpmodel()->record(getCurrentId()).value("gystoDiag").toString();
    //getting features
    QString currentFirstFeature = getTmpmodel()->record(getCurrentId()).value("Feature").toString();

    QSqlTableModel *patModel = DW.selectPatientById((MainWindow*)parent(), getTmpmodel()->record(getCurrentId()).value("Patient_Id").toInt());
    ui->textBrowser_5->clear();
    ui->textBrowser_5->append(QString("Данные по пациенту: \n"
                                      "Номер истории - %1 \n"
                                      "Дата рождения - %2 \n"
                                      "Год заболевания - %3 \n"
                                      "Возраст - %4 \n"
                                      "Пол - %5 \n"
                                      "Гистологический диагноз - %6 \n"
                                      "Цитологический диагноз - %7 \n")
                              .arg(patModel->record(0).value("historyNum").toString())
                              .arg(patModel->record(0).value("dateofBirth").toString())
                              .arg(patModel->record(0).value("dateOfFallingIll").toString())
                              .arg(patModel->record(0).value("age").toString())
                              .arg(patModel->record(0).value("sex").toString())
                              .arg(patModel->record(0).value("gystoDiagn").toString())
                              .arg(patModel->record(0).value("cytoDiagn").toString()));

    ui->lineEdit_3->setText(currentZoom);
    ui->lineEdit_7->setText(currentColorOfImage);
    ui->lineEdit->setText(currentCytoDiag);
    ui->lineEdit_2->setText(currentGystoDiag);
    ui->textBrowser->setText(currentFirstFeature);
    ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(globalAllRows));

    imageObject = new QImage();
    imageObject->load(currentPath);

    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView_2->setScene(scene);
}

void MainWindow::on_pushButton_11_clicked()
{

    QString checker="";
    for(int i=getCurrentIdImageList();i<getCurrentIdImageList()+5;i++){
        checker = checker.append(getTmpmodel()->record(i).value("linkToImage").toString());
    }

    if(checker == ""){
        setCurrentIdImageList(0);
    }

    for(int i=getCurrentIdImageList();i<getCurrentIdImageList()+5;i++){
        imageObject = new QImage();
        imageObject->load(typeOfImages + "_" + ui->comboBox_4->currentText() + "\\TImages\\" + getTmpmodel()->record(i).value("linkToImage").toString());

        image = QPixmap::fromImage(*imageObject);

        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());


        // i dont know how to do this properly
        if(i==getCurrentIdImageList()){ui->graphicsView_3->setScene(scene);
                                       ui->graphicsView_3->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+1){ui->graphicsView_4->setScene(scene);
                                         ui->graphicsView_4->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+2){ui->graphicsView_5->setScene(scene);
                                         ui->graphicsView_5->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+3){ui->graphicsView_6->setScene(scene);
                                         ui->graphicsView_6->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+4){ui->graphicsView_7->setScene(scene);
                                         ui->graphicsView_7->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}


    }

    setCurrentIdImageList(getCurrentIdImageList()+5);

}

int MainWindow::getCurrentIdImageList() const
{
    return currentIdImageList;
}

void MainWindow::setCurrentIdImageList(int value)
{
    currentIdImageList = value;
}



void MainWindow::on_pushButton_10_clicked()
{
    QString checker="";
    for(int i=getCurrentIdImageList();i<getCurrentIdImageList()+5;i++){
        checker = checker.append(getTmpmodel()->record(i).value("linkToImage").toString());
    }

    if(checker == ""){
        setCurrentIdImageList(4); //fix bug  - На последней картинке первые загружаются пачкой
    }

    for(int i=getCurrentIdImageList()-4;i<getCurrentIdImageList()+1;i++){
        imageObject = new QImage();
        imageObject->load(typeOfImages + "_" + ui->comboBox_4->currentText() + "\\TImages\\" + getTmpmodel()->record(i).value("linkToImage").toString());

        image = QPixmap::fromImage(*imageObject);

        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());


        // i dont know how to do this properly
        if(i==getCurrentIdImageList()-4){ui->graphicsView_3->setScene(scene);
                                       ui->graphicsView_3->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-3){ui->graphicsView_4->setScene(scene);
                                         ui->graphicsView_4->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-2){ui->graphicsView_5->setScene(scene);
                                         ui->graphicsView_5->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-1){ui->graphicsView_6->setScene(scene);
                                         ui->graphicsView_6->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()){ui->graphicsView_7->setScene(scene);
                                         ui->graphicsView_7->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}


    }

    setCurrentIdImageList(getCurrentIdImageList()+1);
}

void MainWindow::on_pushButton_13_clicked()
{
    if (getCurrentIdImageList()==5){
        setCurrentIdImageList(getTmpmodel()->rowCount());
    } else {

        QString checker;
        for(int i=getCurrentIdImageList()-5;i<getCurrentIdImageList();i++){
            checker = checker.append(getTmpmodel()->record(i).value("linkToImage").toString());
        }

        if(checker == ""){
            setCurrentIdImageList(getTmpmodel()->rowCount());
        }
    }

    for(int i=getCurrentIdImageList()-5;i<getCurrentIdImageList();i++){
        imageObject = new QImage();
        imageObject->load(typeOfImages + "_" + ui->comboBox_4->currentText() + "\\TImages\\" + getTmpmodel()->record(i).value("linkToImage").toString());

        image = QPixmap::fromImage(*imageObject);

        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());


        // i dont know how to do this properly

        // Сделать массив из graphicsView и проходить в цикле
        if(i==getCurrentIdImageList()-5){ui->graphicsView_3->setScene(scene);
                                       ui->graphicsView_3->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-4){ui->graphicsView_4->setScene(scene);
                                         ui->graphicsView_4->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-3){ui->graphicsView_5->setScene(scene);
                                         ui->graphicsView_5->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-2){ui->graphicsView_6->setScene(scene);
                                         ui->graphicsView_6->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-1){ui->graphicsView_7->setScene(scene);
                                         ui->graphicsView_7->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}


    }

    setCurrentIdImageList(getCurrentIdImageList()-5);
}

void MainWindow::on_pushButton_12_clicked()
{


        QString checker="";
        for(int i=getCurrentIdImageList()-10;i<getCurrentIdImageList()-4;i++){
            checker = checker.append(getTmpmodel()->record(i).value("linkToImage").toString());
        }

        if(checker == ""){
            setCurrentIdImageList(getTmpmodel()->rowCount());
        }


    for(int i=getCurrentIdImageList()-5;i<getCurrentIdImageList();i++){
        imageObject = new QImage();
        imageObject->load(typeOfImages + "_" + ui->comboBox_4->currentText() + "\\TImages\\" + getTmpmodel()->record(i).value("linkToImage").toString());

        image = QPixmap::fromImage(*imageObject);

        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());


        // i dont know how to do this properly
        if(i==getCurrentIdImageList()-5){ui->graphicsView_3->setScene(scene);
                                       ui->graphicsView_3->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-4){ui->graphicsView_4->setScene(scene);
                                         ui->graphicsView_4->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-3){ui->graphicsView_5->setScene(scene);
                                         ui->graphicsView_5->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-2){ui->graphicsView_6->setScene(scene);
                                         ui->graphicsView_6->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()-1){ui->graphicsView_7->setScene(scene);
                                         ui->graphicsView_7->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}


    }

    setCurrentIdImageList(getCurrentIdImageList()-1);
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    //ui->lineEdit->setText(ui->lineEdit->text() + ui->comboBox->currentText() + ",");
}

int MainWindow::getSelectionLogic()
{
    return selectionIndex;
}



dbworker MainWindow::getDW()
{
    return DW;
}

void MainWindow::copyAndCreateDirIfNessesaryForMarkupInsertion()
{
    QString fName = "";
    fName = QString::number(DW.getLastInsertedMarkUpId());
    int pos = pathString.lastIndexOf(QChar('.'));
    if(!QDir(pathString.left(pos)).exists()){
        QDir().mkdir(pathString.left(pos));
    }

    QFile::copy(pathForInsertingMarkUp,pathString.left(pos)+"\\"+fName+".jpg");
    //qDebug()<<"Im here!";
}



dbworker MainWindow::DW;

void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    selectionIndex = index;
}


void MainWindow::on_comboBox_4_currentIndexChanged(const QString &arg1)
{
    tableName = arg1;
}

void MainWindow::on_pushButton_19_clicked()
{
    if(ui->comboBox_4->currentIndex()==0){
        tableName="Почка";
    }

    if(tableName=="Толстая кишка"){
        ColonFilter *cf = new ColonFilter();
        cf->showMaximized();
    }
    if(tableName=="Почка"){
        KidneyFilter *kf = new KidneyFilter();
        kf->showMaximized();
    }
    if(tableName=="Молочная железа"){
        MammalGlandFilter *mgf = new MammalGlandFilter();
        mgf->showMaximized();
    }
}

void MainWindow::on_pushButton_20_clicked()
{
    if(ui->comboBox_4->currentIndex()==0){
        tableName="Почка";
    }

    DiagnosisFilter *df = new DiagnosisFilter();
    df->show();
}

void MainWindow::on_pushButton_5_clicked()
{

    // ui->lineEdit_3->setText(diagnosisString);
    ui->textBrowser_4->clear();
    ui->textBrowser_4->append("Гистологические диагнозы: ");
    ui->textBrowser_4->append(gystoDiagnosisString);
    ui->textBrowser_4->append("Цитологические диагнозы: ");
    ui->textBrowser_4->append(cytoDiagnosisString);

    QSqlTableModel *model = DW.selectImageByDiagnosis((MainWindow*)parent(), gystoDiagnosisString, cytoDiagnosisString, tableName);
    setTmpmodel(model);
    setCurrentId(0);
    setCurrentIdImageList(0);

    QSqlTableModel *patModel = DW.selectPatientById((MainWindow*)parent(), model->record(getCurrentId()).value("Patient_Id").toInt());
    ui->textBrowser_5->clear();
    ui->textBrowser_5->append(QString("Данные по пациенту: \n"
                                      "Номер истории - %1 \n"
                                      "Дата рождения - %2 \n"
                                      "Год заболевания - %3 \n"
                                      "Возраст - %4 \n"
                                      "Пол - %5 \n"
                                      "Гистологический диагноз - %6 \n"
                                      "Цитологический диагноз - %7 \n")
                              .arg(patModel->record(0).value("historyNum").toString())
                              .arg(patModel->record(0).value("dateofBirth").toString())
                              .arg(patModel->record(0).value("dateOfFallingIll").toString())
                              .arg(patModel->record(0).value("age").toString())
                              .arg(patModel->record(0).value("sex").toString())
                              .arg(patModel->record(0).value("gystoDiagn").toString())
                              .arg(patModel->record(0).value("cytoDiagn").toString()));


    //проверка на существование маркеров!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //но сперва сбрасываем состояние флага в ноль just to be sure и скрываем всю хуйню повторно
    setCurrentMarkUpId(0);
    allFilesInsideMarkupFolder.clear();
    gystoDiagForMarkUp.clear();
    cytoDiagForMarkUp.clear();
    featuresForMarkUp.clear();
    ui->lineEdit_4->setVisible(false);
    ui->checkBox_2->setVisible(false);
    ui->textBrowser_6->setVisible(false);
    ui->pushButton_14->setVisible(false);
    ui->pushButton_15->setVisible(false);
    MarkUpExists=false;

    int pos = model->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
    if(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
            "\\TImages\\" + model->record(getCurrentId()).value("linkToImage").toString().left(pos)).exists()){
        MarkUpExists=true;
    }

    // если существует разметка, то 1) кладем в лист имена всех разметок внутри папки
    // 2) тащим в два листа соответствующие диагнозы из базы

    //1)


    if(MarkUpExists){

        //показываем всю хуйню
        ui->lineEdit_4->setVisible(true);
        ui->checkBox_2->setVisible(true);
        ui->textBrowser_6->setVisible(true);
        ui->pushButton_14->setVisible(true);
        ui->pushButton_15->setVisible(true);
        //--------------

        QDirIterator it(QDir(typeOfImages + "_" + ui->comboBox_4->currentText() +
                             "\\TImages\\" + model->record(getCurrentId()).value("linkToImage").toString().left(pos)));
        while (it.hasNext()){
            QFile f(it.next());
            allFilesInsideMarkupFolder.push_back(it.fileName().left(it.fileName().size()-4).toInt());
            qDebug()<<it.fileName().left(it.fileName().size()-4).toInt();
            qDebug()<<allFilesInsideMarkupFolder;
        }

        allFilesInsideMarkupFolder.removeFirst();
        allFilesInsideMarkupFolder.removeFirst();
         //2)


        for(int i=0;i<allFilesInsideMarkupFolder.size();i++){
            QPair<QString,QString> tmp = DW.selectCytoAndGystoDiagForMarkUp(allFilesInsideMarkupFolder.at(i));
            QStringList tmp2 = DW.selectFeatureForMarkUp(allFilesInsideMarkupFolder.at(i));
            featuresForMarkUp.push_back(tmp2.at(0));
            gystoDiagForMarkUp.push_back(tmp.first);
            cytoDiagForMarkUp.push_back(tmp.second);
        }

        setCurrentMarkUpId(0);
        // сначала загружаем первую в списке хуету
        ui->textBrowser_6->clear();
        ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
        ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                          "Цитологический диагноз - %2"
                                          "Признак на разметке - %3")
                                  .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                                  .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                                  .arg(featuresForMarkUp.at(getCurrentMarkUpId())));



    } else {
        //если ничего, то сразу очищаем just to be sure
        setCurrentMarkUpId(0);
        allFilesInsideMarkupFolder.clear();
        gystoDiagForMarkUp.clear();
        cytoDiagForMarkUp.clear();
        featuresForMarkUp.clear();
    }





    //---------------------------------------------------------------------------------------------------------------------


    QString firstPath = typeOfImages + "_" + ui->comboBox_4->currentText() +
            "\\TImages\\" + model->record(getCurrentId()).value("linkToImage").toString();
    //костыли для возврата картинки в рисовалку
    pathString = firstPath;


    //getting zoom
    QString zoom = model->record(getCurrentId()).value("zoomOfImage").toString();
    //getting colorOfProbe
    QString colorOfImage = model->record(getCurrentId()).value("colorOfImage").toString();
    //getting all diagnosis
    QString cytoDiag = model->record(getCurrentId()).value("cytDiag").toString();
    QString gystoDiag = model->record(getCurrentId()).value("gystoDiag").toString();
    //getting features
    QString firstFeature = model->record(getCurrentId()).value("Feature").toString();

    //showing params to doctor
    ui->lineEdit_3->setText(zoom);
    ui->lineEdit_7->setText(colorOfImage);
    ui->lineEdit->setText(cytoDiag);
    ui->lineEdit_2->setText(gystoDiag);
    ui->textBrowser->setText(firstFeature);

    imageObject = new QImage();
    imageObject->load(firstPath);

    image = QPixmap::fromImage(*imageObject);

    scene = new QGraphicsScene(this);
    scene->addPixmap(image);
    scene->setSceneRect(image.rect());
    ui->graphicsView_2->setScene(scene);

    //Statistics for Gystodiagnos selection
    ui->textBrowser_2->clear();
    int allRows = model->rowCount();
    globalAllRows = allRows;
    //ui->lineEdit_4->setText(QString::number(allRows));
    double dAllRows = (double) allRows;


    QVector<double> tmpvect;

    for(int i=0;i<allRows;i++){
        int counter = 0;
        for(int j=0;j<allRows;j++){
            if (model->record(i).value("gystoDiag").toString() == model->record(j).value("gystoDiag").toString()){
                counter++;
            }
        }

        tmpvect.append(100*counter/dAllRows);
    }


    QVector<QString> diagNames;
    for(int i=0;i<allRows;i++){
        diagNames.append(model->record(i).value("gystoDiag").toString());
    }

    QList<QPair<QString,double>> mightySelector;

    for(int i=0;i<allRows;i++){
        mightySelector.append(qMakePair(diagNames.at(i),tmpvect.at(i)));
    }

    //toSet then toList doesnt work apparently so
    qSort(mightySelector.begin(),mightySelector.end());
    mightySelector.erase(std::unique(mightySelector.begin(),mightySelector.end()), mightySelector.end());


    ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(allRows));
    ui->textBrowser_2->append("Статистика выборки по гистологическим диагнозам:");
    ui->textBrowser_2->append(QString("Всего диагнозов - %1 \n").arg(mightySelector.size()));

    for(int i=0;i<mightySelector.size();i++){
        ui->textBrowser_2->append(QString("Изображений с диагнозом %1 - %2% процентов \n")
                                  .arg(mightySelector.at(i).first).arg(mightySelector.at(i).second));
        //filling up list for analytic
        GystoDiagnosisForAnalytics.push_back(mightySelector.at(i).first);
        GystoPercentageDiagnosisForAnalytics.push_back(mightySelector.at(i).second);
    }

    tmpvect.clear();


    //Statistics for Cytodiagnos selection


    for(int i=0;i<allRows;i++){
        int counter = 0;
        for(int j=0;j<allRows;j++){
            if (model->record(i).value("cytDiag").toString() == model->record(j).value("cytDiag").toString()){
                counter++;
            }
        }

        tmpvect.append(100*counter/dAllRows);
    }

    QVector<QString> cytoDiagNames;
    for(int i=0;i<allRows;i++){
        cytoDiagNames.append(model->record(i).value("cytDiag").toString());
    }


    QList<QPair<QString,double>> mightierSelector;

    for(int i=0;i<allRows;i++){
        mightierSelector.append(qMakePair(cytoDiagNames.at(i),tmpvect.at(i)));
    }

    //toSet then toList doesnt work apparently so
    qSort(mightierSelector.begin(),mightierSelector.end());
    mightierSelector.erase(std::unique(mightierSelector.begin(),mightierSelector.end()), mightierSelector.end());


    ui->lineEdit_6->setText(QString("%1 изобр. из %2").arg(getCurrentId()+1).arg(allRows));
    ui->textBrowser_2->append("________________________________________");
    ui->textBrowser_2->append("Статистика выборки по цитологическим диагнозам:");
    ui->textBrowser_2->append(QString("Всего диагнозов - %1 \n").arg(mightierSelector.size()));

    for(int i=0;i<mightierSelector.size();i++){
        ui->textBrowser_2->append(QString("Изображений с диагнозом %1 - %2% процентов \n")
                                  .arg(mightierSelector.at(i).first).arg(mightierSelector.at(i).second));
        //filling up list for analytic
        CytoDiagnosisForAnalytics.push_back(mightierSelector.at(i).first);
        CytoPercentageDiagnosisForAnalytics.push_back(mightierSelector.at(i).second);
    }


    tmpvect.clear();



    //Statistics for Feature selection

    QString tmpFeature;
    for (int i=0;i<allRows;i++){
        tmpFeature.append(model->record(i).value("Feature").toString() + ",");
    }
    tmpFeature.chop(1);

    QStringList q = tmpFeature.split(',');
    q = q.toSet().toList(); //getting rid of duplicates

    for(int i=0;i<q.size();i++){
        int counter = 0;
        for(int j=0;j<allRows;j++){
            if (model->record(j).value("Feature").toString().contains(q.value(i))){
                counter++;
            }
        }

        tmpvect.append(100*counter/dAllRows);
    }


    ui->textBrowser_2->append("___________________");
    ui->textBrowser_2->append("Статистика выборки по признакам: \n");
    ui->textBrowser_2->append(QString("Всего признаков - %1 \n").arg(q.size()));

    for(int i=0;i<tmpvect.size();i++){
        ui->textBrowser_2->append(QString("Изображений с признаком %1 - %2% процентов \n")
                                  .arg(q.value(i)).arg(tmpvect.value(i)));
    }


    //block for analytics
    FeatureForAnalytics = q;
    percentageFeatureForAnalytics = tmpvect;

    // Loading images in the bottom

    for(int i=getCurrentIdImageList();i<getCurrentIdImageList()+5;i++){


        imageObject = new QImage();
        imageObject->load(typeOfImages + "_" + ui->comboBox_4->currentText() +
                          "\\TImages\\" + model->record(i).value("linkToImage").toString());

        image = QPixmap::fromImage(*imageObject);

        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());


        // i dont know how to do this properly
        if(i==getCurrentIdImageList()){ui->graphicsView_3->setScene(scene);
                                       ui->graphicsView_3->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+1){ui->graphicsView_4->setScene(scene);
                                         ui->graphicsView_4->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+2){ui->graphicsView_5->setScene(scene);
                                         ui->graphicsView_5->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+3){ui->graphicsView_6->setScene(scene);
                                         ui->graphicsView_6->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}
        if(i==getCurrentIdImageList()+4){ui->graphicsView_7->setScene(scene);
                                         ui->graphicsView_7->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);}




    }

    setCurrentIdImageList(getCurrentIdImageList()+5);


}

void MainWindow::on_checkBox_stateChanged(int arg1)
{

    if(ui->checkBox->isChecked()){
        //ui->graphicsView->hide();
        ui->graphicsView_2->move(10,50);
        ui->graphicsView->hide();
        ui->graphicsView_2->resize(1240,521);
    } else {

        ui->graphicsView_2->resize(621,521);
        ui->graphicsView_2->move(630,50);
        ui->graphicsView->show();
    }


}

void MainWindow::on_action_3_triggered()
{
    ImageMarkUp *img = new ImageMarkUp();
    img->show();
}

void MainWindow::on_action_2_triggered()
{
    Analytics *an = new Analytics();
    an->showMaximized();
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    typeOfImages = arg1;
}


void MainWindow::on_action_7_triggered()
{
    AnalyticForCyto *an = new AnalyticForCyto();
    an->showMaximized();
}

void MainWindow::on_action_6_triggered()
{
    AnalyticForGysto *an = new AnalyticForGysto();
    an->showMaximized();
}

int MainWindow::getCurrentMarkUpId() const
{
    return currentMarkUpId;
}

void MainWindow::setCurrentMarkUpId(int value)
{
    currentMarkUpId = value;
}

void MainWindow::on_pushButton_15_clicked()
{

    //on_checkBox_2_stateChanged(2);
    qDebug()<<allFilesInsideMarkupFolder.size();
    if(getCurrentMarkUpId()==allFilesInsideMarkupFolder.size()-1){
        setCurrentMarkUpId(0);
    } else {
        setCurrentMarkUpId(getCurrentMarkUpId()+1);
    }

    ui->textBrowser_6->clear();
    ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
    ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                      "Цитологический диагноз - %2"
                                      "Признаки на изображении - %3")
                              .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                              .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                              .arg(featuresForMarkUp.at(getCurrentMarkUpId())));


}

QVector<int> MainWindow::getAllFilesInsideMarkupFolder() const
{
    return allFilesInsideMarkupFolder;
}

void MainWindow::setAllFilesInsideMarkupFolder(const QVector<int> &value)
{
    allFilesInsideMarkupFolder = value;
}

QStringList MainWindow::getGystoDiagForMarkUp() const
{
    return gystoDiagForMarkUp;
}

void MainWindow::setGystoDiagForMarkUp(const QStringList &value)
{
    gystoDiagForMarkUp = value;
}

QStringList MainWindow::getCytoDiagForMarkUp() const
{
    return cytoDiagForMarkUp;
}

void MainWindow::setCytoDiagForMarkUp(const QStringList &value)
{
    cytoDiagForMarkUp = value;
}

void MainWindow::on_pushButton_14_clicked()
{
    //on_checkBox_2_stateChanged(2);
    if(getCurrentMarkUpId()==0){
        setCurrentMarkUpId(allFilesInsideMarkupFolder.size()-1);
    } else {
        setCurrentMarkUpId(getCurrentMarkUpId()-1);
    }

    ui->textBrowser_6->clear();
    ui->lineEdit_4->setText(QString("%1 разметка из %2").arg(getCurrentMarkUpId()+1).arg(featuresForMarkUp.size()));
    ui->textBrowser_6->append(QString("Гистологический диагноз - %1"
                                      "Цитологический диагноз - %2"
                                      "Признаки на изображении - %3")
                              .arg(gystoDiagForMarkUp.at(getCurrentMarkUpId()))
                              .arg(cytoDiagForMarkUp.at(getCurrentMarkUpId()))
                              .arg(featuresForMarkUp.at(getCurrentMarkUpId())));


}

void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    /*if(arg1==0){

    }*/

    if(ui->checkBox_2->isChecked()){


        //for overlay(markup) image

        int pos = getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().lastIndexOf(QChar('.'));
        QString linkTo = typeOfImages + "_" + ui->comboBox_4->currentText() + "\\TImages\\"
                + getTmpmodel()->record(getCurrentId()).value("linkToImage").toString().left(pos) + "\\"
                + QString::number(allFilesInsideMarkupFolder.at(getCurrentMarkUpId())) + ".jpg";


        QImage *imageObject2 = new QImage();
        imageObject2->load(linkTo);

        QPixmap image2 = QPixmap::fromImage(*imageObject2);
        scene = new QGraphicsScene(this);
        scene->addPixmap(image2);
        scene->setSceneRect(image2.rect());
        ui->graphicsView_2->setScene(scene);



    }


    if(!ui->checkBox_2->isChecked()){
        QImage *imageObject = new QImage();
        imageObject->load(pathString);

        QPixmap image = QPixmap::fromImage(*imageObject);
        scene = new QGraphicsScene(this);
        scene->addPixmap(image);
        scene->setSceneRect(image.rect());
        ui->graphicsView_2->setScene(scene);

    }

}

void MainWindow::on_pushButton_17_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"),
                                                    "C:\\",
                                                    tr("Images (*.png *.jpg)"));
    QFile::copy(pathString,fileName);
}

void MainWindow::on_pushButton_16_clicked()
{
    MarkUpAdder *mua = new MarkUpAdder();
    mua->show();
}

QStringList MainWindow::getFeaturesForMarkUp() const
{
    return featuresForMarkUp;
}

void MainWindow::setFeaturesForMarkUp(const QStringList &value)
{
    featuresForMarkUp = value;
}
