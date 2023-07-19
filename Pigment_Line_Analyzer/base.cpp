#include "base.h"

Base::Base(QObject *parent) : QObject(parent)
{

}

Base::~Base()
{

}

void Base::connectToDataBase()
{
    if(!QFile("DataBase.sqlite").exists())
        this->restoreDataBase();
    else
        this->openDataBase();
}

bool Base::insertIntoDataBase(const QString &file_name, const QString &result_name/*, const std::vector<QString> &points, const std::vector<std::vector<int> > &features*/)
{
    return (insertIntoTableNames(file_name) && insertIntoTableResults(file_name, result_name) /*&&
                    insertIntoTableFeatures(file_name, result_name, points, features)*/);
}

void Base::selectFromResults(const QString &file_name, std::vector<QString> &result)
{
    QSqlQuery query;
    query.exec("SELECT Id FROM Names WHERE FileName = '"+file_name+"';");
    query.next();
    int id = query.value(0).toInt();
    query.exec("SELECT ResultName FROM Results WHERE NameId = '"+QString::number(id)+"';");
    result.clear();
    while (query.next())
    {
        qDebug() << query.value(0).toString();
        result.push_back(query.value(0).toString());
    }
}

bool Base::restoreDataBase()
{
    if(this->openDataBase())
        return (this->createTableNames() && this->createTableResults() /*&& this->createTableFeatures()*/);
    else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
}

bool Base::openDataBase()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("SegmentsDataBase");
    db.setDatabaseName("DataBase.sqlite");
    if(db.open())
        return true;
    else
        return false;
}

void Base::closeDataBase()
{
    db.close();
}

bool Base::createTableNames()
{
    QSqlQuery query;
    if(!query.exec("CREATE TABLE Names (Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "FileName VARCHAR(255) NOT NULL"
                   ")" ))
    {
        qDebug() << "DataBase: error of create ";
        qDebug() << query.lastError().text();
        return false;
    } else
        return true;
}

bool Base::createTableResults()
{
    QSqlQuery query;
    if(!query.exec("CREATE TABLE Results (Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "NameId INTEGER NOT NULL, ResultName VARCHAR(255) NOT NULL, "
                   "SigmaD SMALLINT NOT NULL, SigmaI SMALLINT NOT NULL, "
                   "Threshold SMALLINT NOT NULL, MinSize SMALLINT NOT NULL"
                   ")" ))
    {
        qDebug() << "DataBase: error of create ";
        qDebug() << query.lastError().text();
        return false;
    } else
        return true;
}

bool Base::createTableFeatures()
{
    QSqlQuery query;
    if(!query.exec("CREATE TABLE Features (Id INTEGER PRIMARY KEY AUTOINCREMENT, "
                   "NameId INTEGER NOT NULL, ResultId INTEGER NOT NULL, Points TEXT NOT NULL, "
                   "Length SMALLINT NOT NULL, Thickness TINYINT UNSIGNED NOT NULL, Angle SMALLINT NOT NULL, "
                   "MinRed TINYINT UNSIGNED NOT NULL, MeanRed TINYINT UNSIGNED NOT NULL, MaxRed TINYINT UNSIGNED NOT NULL, "
                   "MinGreen TINYINT UNSIGNED NOT NULL, MeanGreen TINYINT UNSIGNED NOT NULL, MaxGreen TINYINT UNSIGNED NOT NULL, "
                   "MinBlue TINYINT UNSIGNED NOT NULL, MeanBlue TINYINT UNSIGNED NOT NULL, MaxBlue TINYINT UNSIGNED NOT NULL"
                   ")" ))
    {
        qDebug() << "DataBase: error of create ";
        qDebug() << query.lastError().text();
        return false;
    } else
        return true;
}

bool Base::insertIntoTableNames(const QString &file_name)
{
    QSqlQuery query;
    query.prepare("INSERT INTO Names (FileName)"
                  "VALUES (:FN)");
    query.bindValue(":FN", file_name);
    if(!query.exec()){
        qDebug() << "error insert into Names";
        qDebug() << query.lastError().text();
        return false;
    }
    else
        return true;
}

bool Base::insertIntoTableResults(const QString &file_name, const QString &result_name)
{
    QSqlQuery query;
    query.exec("SELECT Id FROM Names WHERE FileName = '"+file_name+"';");
    query.next();
    int id = query.value(0).toInt();
    query.prepare("INSERT INTO Results (NameId, ResultName, SigmaD, SigmaI, Threshold, MinSize) "
                  "VALUES (:NID, :RN, :SD, :SI, :T, :MS)");
    query.bindValue(":NID", id);
    query.bindValue(":RN", result_name);
    QStringList sl = result_name.split("_");
    query.bindValue(":SD", sl.at(0).toInt());
    query.bindValue(":SI", sl.at(1).toInt());
    query.bindValue(":T", sl.at(2).toInt());
    query.bindValue(":MS", sl.at(3).split(".").at(0).toInt());
    if(!query.exec()){
        qDebug() << "error insert into Results";
        qDebug() << query.lastError().text();
        return false;
    }
    else
        return true;
}

bool Base::insertIntoTableFeatures(const QString &file_name, const QString &result_name, const std::vector<QString> &points, const std::vector<std::vector<int> > &features)
{
    QSqlQuery query;
    query.exec("SELECT Id FROM Names WHERE FileName = '"+file_name+"';");
    query.next();
    int nid = query.value(0).toInt();
    query.exec("SELECT Id FROM Results WHERE ResultName = '"+result_name+"';");
    query.next();
    int rid = query.value(0).toInt();
    for (size_t i = 0; i < points.size(); ++i)
    {
        query.prepare("INSERT INTO Features (NameId, ResultId, Points, "
                      "Length, Thickness, Angle, "
                      "MinRed, MeanRed, MaxRed, "
                      "MinGreen, MeanGreen, MaxGreen, "
                      "MinBlue, MeanBlue, MaxBlue)"
                      "VALUES (:NID, :RID, :P, :L, :T, :A, :R1, :R2, :R3, "
                      ":G1, :G2, :G3, :B1, :B2, :B3)");
        query.bindValue(":NID", nid);
        query.bindValue(":RID", rid);
        query.bindValue(":P", points[i]);
        query.bindValue(":L", features[i][0]);
        query.bindValue(":T", features[i][1]);
        query.bindValue(":A", features[i][2]);
        query.bindValue(":R1", features[i][3]);
        query.bindValue(":R2", features[i][4]);
        query.bindValue(":R3", features[i][5]);
        query.bindValue(":G1", features[i][6]);
        query.bindValue(":G2", features[i][7]);
        query.bindValue(":G3", features[i][8]);
        query.bindValue(":B1", features[i][9]);
        query.bindValue(":B2", features[i][10]);
        query.bindValue(":B3", features[i][11]);
        if(!query.exec()){
            qDebug() << "error insert into Features";
            qDebug() << query.lastError().text();
            return false;
        }
        qDebug() << i << " from " << points.size();
    }
    return true;
}
