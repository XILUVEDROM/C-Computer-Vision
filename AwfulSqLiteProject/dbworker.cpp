#include "dbworker.h"
#include "iostream"
#include <vector>
#include "mainwindow.h"

dbworker::dbworker()
{
    QSqlDatabase db;
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:\\Projects\\SQLiteProject\\sqliteDB.db");
    db.open();

    if(!db.open())
    {
        qDebug()<<"SQL driver not loaded";
    }

}


dbworker::~dbworker()
{

}



QString dbworker::userRole = QString("");

bool dbworker::checkIfAuthCorrect(QString login, QString password)
{

    QSqlQuery q = QSqlQuery(db);
    //q.prepare(QString("select * from Users where login=\"admin\" and pass=\"IHateThis\""));
    q.prepare(QString("select * from Users where login=\"%1\" and pass=\"%2\"").arg(login).arg(password));



    if(!q.exec() || !q.next()){

        if(login.size()==0 || password.size()==0){ //fucking login form error doesnt work
            QMessageBox::critical(0,"Ошибка аутентификации", "Пароль или логин не могут быть пустыми");
            return false;
        } else {
            QMessageBox::critical(0,"Ошибка аутентификации", "Неверный пароль или логин!");
            return false;
        }


    } else{
        setUserRole(q.value(3).toString());
        return true;
    }


}

void dbworker::insertMarkUp(QString gystoDiag, QString cytDiag, QString features)
{
    //Query for image insert
    QSqlQuery q = QSqlQuery(db);
    q.prepare("INSERT INTO MarkUp (gystoDiag,cytoDiag,Feature) "
              "VALUES (?,?,?)");
    q.addBindValue(gystoDiag);
    q.addBindValue(cytDiag);
    q.addBindValue(features);
    q.exec();

}

void dbworker::insertImage(QString gystoDiag, QString cytDiag, QString colorofImage, QString typeOfImage, QString zoomOfImage,
                           QDate dateOfAdding, QString linkToImage, int pat_id, QString table)
{
    //Query for image insert
    QSqlQuery q = QSqlQuery(db);

    // checking what table to access
    if(table=="Толстая кишка"){
        q.prepare("INSERT INTO Colon (gystoDiag,cytDiag,colorOfImage,typeOfImage,zoomOfImage,dateOfAdding,linkToImage,Patient_Id) "
                  "VALUES (?,?,?,?,?,?,?,?)");
    }
    if(table=="Почка"){
        q.prepare("INSERT INTO Kidney (gystoDiag,cytDiag,colorOfImage,typeOfImage,zoomOfImage,dateOfAdding,linkToImage,Patient_Id) "
                  "VALUES (?,?,?,?,?,?,?,?)");
    }
    if(table=="Молочная железа"){
        q.prepare("INSERT INTO MammalGland (gystoDiag,cytDiag,colorOfImage,typeOfImage,zoomOfImage,dateOfAdding,linkToImage,Patient_Id) "
                  "VALUES (?,?,?,?,?,?,?,?)");
    }



    q.addBindValue(gystoDiag);
    q.addBindValue(cytDiag);
    q.addBindValue(colorofImage);
    q.addBindValue(typeOfImage);
    q.addBindValue(zoomOfImage);
    q.addBindValue(dateOfAdding);
    q.addBindValue(linkToImage);
    q.addBindValue(pat_id);



    if( !q.exec() ){
        QMessageBox::critical(0,
                              "Critical", "Ошибка при добавлении "
                                          "информации в базу");
        return;
    }
    else
        QMessageBox::information(0,
                                 "Information", ""
                                                "Запись добавлена");

}

void dbworker::insertPatientWhileInsertingImage(QString historyNum, QDate dateOfBirth, int dateOfFallingIll, int age, QString sex, QString gystoDiag, QString cytoDiag, QString organType, QString table)
{
    //Query for image insert
    QSqlQuery q = QSqlQuery(db);

    q.prepare("INSERT INTO Patient (historyNum,dateOfBirth,dateOfFallingIll,age,sex,gystoDiag,cytoDiag,organType) "
                  "VALUES (?,?,?,?,?,?,?,?)");




    q.addBindValue(historyNum);
    q.addBindValue(dateOfBirth);
    q.addBindValue(dateOfFallingIll);
    q.addBindValue(age);
    q.addBindValue(sex);
    q.addBindValue(gystoDiag);
    q.addBindValue(cytoDiag);
    q.addBindValue(organType);



    if( !q.exec() ){
        QMessageBox::critical(0,
                              "Critical", "Ошибка при добавлении "
                                          "информации в базу");
        return;
    }
    else
        QMessageBox::information(0,
                                 "Information", ""
                                                "Запись добавлена");
}

void dbworker::insertPatient(QString historyNum, QDate dateOfBirth, int dateOfFallingIll, int age, QString sex, QString gystoDiag, QString cytoDiag, QString organType, QString table)
{
    //Query for image insert
    QSqlQuery q = QSqlQuery(db);

    q.prepare("INSERT INTO Patient (historyNum,dateOfBirth,dateOfFallingIll,age,sex,gystoDiag,cytoDiag,organType) "
                  "VALUES (?,?,?,?,?,?,?,?)");




    q.addBindValue(historyNum);
    q.addBindValue(dateOfBirth);
    q.addBindValue(dateOfFallingIll);
    q.addBindValue(age);
    q.addBindValue(sex);
    q.addBindValue(gystoDiag);
    q.addBindValue(cytoDiag);
    q.addBindValue(organType);



    if( !q.exec() ){
        QMessageBox::critical(0,
                              "Critical", "Ошибка при добавлении "
                                          "информации в базу");
        return;
    }
    else
        QMessageBox::information(0,
                                 "Information", ""
                                                "Запись добавлена");
}

int dbworker::getLastInsertedPatient()
{
    QSqlQuery q = QSqlQuery(db);
    q.prepare("SELECT MAX(Patient_id) FROM Patient");
    q.exec();
    while (q.next()) {
       return q.value(0).toInt();
    }
}

int dbworker::getLastInsertedMarkUpId()
{
    QSqlQuery q = QSqlQuery(db);
    q.prepare("SELECT MAX(markUp_id) FROM MarkUp");
    q.exec();
    while (q.next()) {
       return q.value(0).toInt();
    }
}


QStringList dbworker::fillGystoDiagInInsertForm(QString table)
{
    QSqlQuery q = QSqlQuery(db);
    QStringList result;
    q.setForwardOnly(true);
    if(table=="Толстая кишка"){
        q.prepare("SELECT gystoDiag FROM Colon");
    }
    if(table=="Почка"){
        q.prepare("SELECT gystoDiag FROM Kidney");
    }
    if(table=="Молочная железа"){
        q.prepare("SELECT gystoDiag FROM MammalGland");
    }


    q.exec();

    while(q.next())
    {
        result.push_back(q.value(0).toString());
    }

    QString tmp = result.join(",");
    QStringList finalresult = tmp.split(',');
    finalresult = finalresult.toSet().toList();

    return finalresult;
}

QStringList dbworker::fillCytoDiagInInsertForm(QString table)
{
    QSqlQuery q = QSqlQuery(db);
    QStringList result;
    q.setForwardOnly(true);
    if(table=="Толстая кишка"){
        q.prepare("SELECT cytDiag FROM Colon");
    }
    if(table=="Почка"){
        q.prepare("SELECT cytDiag FROM Kidney");
    }
    if(table=="Молочная железа"){
        q.prepare("SELECT cytDiag FROM MammalGland");
    }


    q.exec();

    while(q.next())
    {
        result.push_back(q.value(0).toString());
    }

    QString tmp = result.join(",");
    QStringList finalresult = tmp.split(',');
    finalresult = finalresult.toSet().toList();

    return finalresult;
}

QStringList dbworker::fillColorOfImage(QString table)
{
    QSqlQuery q = QSqlQuery(db);
    QStringList result;
    q.setForwardOnly(true);
    if(table=="Толстая кишка"){
        q.prepare("SELECT colorOfImage FROM Colon");
    }
    if(table=="Почка"){
        q.prepare("SELECT colorOfImage FROM Kidney");
    }
    if(table=="Молочная железа"){
        q.prepare("SELECT colorOfImage FROM MammalGland");
    }


    q.exec();

    while(q.next())
    {
        result.push_back(q.value(0).toString());
    }

    QString tmp = result.join(",");
    QStringList finalresult = tmp.split(',');
    finalresult = finalresult.toSet().toList();

    return finalresult;
}

QStringList dbworker::fillScope(QString table)
{
    QSqlQuery q = QSqlQuery(db);
    QStringList result;
    q.setForwardOnly(true);
    if(table=="Толстая кишка"){
        q.prepare("SELECT zoomOfImage FROM Colon");
    }
    if(table=="Почка"){
        q.prepare("SELECT zoomOfImage FROM Kidney");
    }
    if(table=="Молочная железа"){
        q.prepare("SELECT zoomOfImage FROM MammalGland");
    }


    q.exec();

    while(q.next())
    {
        result.push_back(q.value(0).toString());
    }

    QString tmp = result.join(",");
    QStringList finalresult = tmp.split(',');
    finalresult = finalresult.toSet().toList();

    return finalresult;
}


QPair<QStringList, QStringList> dbworker::fillDiagnosisInComboBoxes(QString table)
{
    QSqlQuery q = QSqlQuery(db);
    QStringList gystoResult;
    QStringList cytoResult;
    q.setForwardOnly(true);
    if(table=="Толстая кишка"){
        q.prepare("SELECT gystoDiag,cytDiag FROM Colon");
    }
    if(table=="Почка"){
        q.prepare("SELECT gystoDiag,cytDiag FROM Kidney");
    }
    if(table=="Молочная железа"){
        q.prepare("SELECT gystoDiag,cytDiag FROM MammalGland");
    }


    q.exec();

    while(q.next())
    {
        gystoResult.push_back(q.value(0).toString());
        cytoResult.push_back(q.value(1).toString());
    }


    gystoResult = gystoResult.toSet().toList();
    cytoResult = cytoResult.toSet().toList();


    QPair<QStringList,QStringList> finalResult = qMakePair(gystoResult,cytoResult);

    /*
    QString tmp = gystoResult.join(",");
    QStringList finalresult = tmp.split(',');
    finalresult = finalresult.toSet().toList(); */

    return finalResult;
}

QPair<QString, QString> dbworker::selectCytoAndGystoDiagForMarkUp(int markupId)
{
    QSqlQuery q = QSqlQuery(db);
    q.prepare(QString("SELECT * FROM MarkUp WHERE markUp_id=%1").arg(markupId));
    q.exec();
    QPair<QString,QString> gystoCytoPair;
    while(q.next()){
        gystoCytoPair = qMakePair(q.value(1).toString(),q.value(2).toString());
    }
    return gystoCytoPair;
}

QStringList dbworker::selectFeatureForMarkUp(int markupId)
{
    QSqlQuery q = QSqlQuery(db);
    q.prepare(QString("SELECT * FROM MarkUp WHERE markUp_id=%1").arg(markupId));
    q.exec();
    QStringList features;
    while(q.next()){
        features.push_back(q.value(3).toString());
    }
    return features;
}





QSqlTableModel *dbworker::selectImage(QWidget *parent, QString params, QString table)
{

    QSqlTableModel *model = new QSqlTableModel(parent, db);
    if(table=="Толстая кишка"){
        model->setTable("Colon");
    }
    if(table=="Почка"){
        model->setTable("Kidney");
    }
    if(table=="Молочная железа"){
        model->setTable("MammalGland");
    }

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);


    //params = params.simplified().replace(" ","");
    //params.chop(1);
    QStringList tempvector = params.split(',');

    QString tmp = "";
    // if "AND" criteria selected
    if(MainWindow::getSelectionLogic()==0){
        for(int i=0;i<tempvector.size();i++){
            tmp = tmp.append(QString("Feature LIKE \"%%1%\" AND ").arg(tempvector[i]));
        }
        tmp.chop(4);
        model->setFilter(tmp);
    }


    // if "OR" criteria selected
    if(MainWindow::getSelectionLogic()==1){
        for(int i=0;i<tempvector.size();i++){
            tmp = tmp.append(QString("Feature LIKE \"%%1%\" OR ").arg(tempvector[i]));
        }
        tmp.chop(3);
        model->setFilter(tmp);
    }

    model->select();
    qDebug()<<model->record(0).value("gystoDiag").toString();

    return model;
}

QSqlTableModel *dbworker::selectImageByDiagnosis(QWidget *parent, QString gystoParams, QString cytoParams, QString table)
{
    QSqlTableModel *model = new QSqlTableModel(parent, db);
    if(table=="Толстая кишка"){
        model->setTable("Colon");
    }
    if(table=="Почка"){
        model->setTable("Kidney");
    }
    if(table=="Молочная железа"){
        model->setTable("MammalGland");
    }

    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    QStringList tempvector = gystoParams.split(',');
    QStringList tempvector2 = cytoParams.split(',');

    QString tmp = "";


    // if "AND" criteria selected
    if(MainWindow::getSelectionLogic()==0){
        if(tempvector.at(0)!=""){
            for(int i=0;i<tempvector.size();i++){
                tmp = tmp.append(QString("gystoDiag LIKE \"%%1%\" AND ").arg(tempvector[i]));
            }
        }

        if(tempvector2.at(0)!=""){
            for(int i=0;i<tempvector2.size();i++){
                tmp = tmp.append(QString("cytoDiag LIKE \"%%1%\" AND ").arg(tempvector2[i]));
            }
        }

        tmp.chop(4);
        model->setFilter(tmp);
    }


    // if "OR" criteria selected
    if(MainWindow::getSelectionLogic()==1){
        if(tempvector.at(0)!=""){
            for(int i=0;i<tempvector.size();i++){
                tmp = tmp.append(QString("gystoDiag LIKE \"%%1%\" OR ").arg(tempvector[i]));
            }
        }

        if(tempvector2.at(0)!=""){
            for(int i=0;i<tempvector2.size();i++){
                tmp = tmp.append(QString("cytoDiag LIKE \"%%1%\" OR ").arg(tempvector2[i]));
            }
        }

        tmp.chop(3);
        model->setFilter(tmp);
    }




    model->select();

    return model;
}

QSqlTableModel *dbworker::selectPatientById(QWidget *parent, int pat_id)
{
    QSqlTableModel *model = new QSqlTableModel(parent, db);
    model->setTable("Patient");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->setFilter(QString("patient_Id_Old = %1").arg(pat_id));
    model->select();
    return model;
}

QString dbworker::getUserRole()
{
    return userRole;
}

void dbworker::setUserRole(const QString &value)
{
    userRole = value;
}


