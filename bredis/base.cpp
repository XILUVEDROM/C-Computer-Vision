#include "base.h"

Base::Base(QObject *parent) : QObject(parent)
{

}

Base::~Base()
{

}

void Base::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile(".\\" DATABASE_NAME).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

bool Base::addNewPatient(const QVariantList &data)
{
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE_CASESOFDISEASES " (id_" TABLE_DIAGNOSIS_GYST ", id_" TABLE_DIAGNOSIS_CYT ", DateOfBirth, Surname, Name, Patronymic,"
                                                          "NumberCard, Gender, Age, DateOfDisease, Comment, NumberOfImages)"
                  "VALUES (:id_diagnos_gyst, :id_diagnos_cyt, :DateOfBirth, :Surname, :Name, :Patronymic, :NumberCard, :Gender, :Age, "
                                                                               ":DateOfDisease, :Comment, :NumberOfImages)");
    query.bindValue(":id_diagnos_gyst", data.at(0).toInt());
    query.bindValue(":id_diagnos_cyt", data.at(1).toInt());
    query.bindValue(":DateOfBirth", data.at(2).toDate());
    query.bindValue(":Surname", data.at(3).toString());
    query.bindValue(":Name", data.at(4).toString());
    query.bindValue(":Patronymic", data.at(5).toString());
    query.bindValue(":NumberCard", data.at(6).toString());
    query.bindValue(":Gender", data.at(7).toString());
    query.bindValue(":Age", data.at(8).toInt());
    query.bindValue(":DateOfDisease", data.at(9).toString());
    query.bindValue(":Comment", data.at(10).toString());
    query.bindValue(":NumberOfImages", 0);
    if(!query.exec()){
        qDebug() << "Error insert new patient";
        qDebug() << query.lastError();
        return false;
    } else{
        qDebug() << "Insert new patient compleate";
        return true;
    }
}

bool Base::updatePatient(int id, const QVariantList &data)
{
    QSqlQuery query;

    query.prepare("UPDATE " TABLE_CASESOFDISEASES " SET id_" TABLE_DIAGNOSIS_GYST " = :id_diagnos_gyst, id_" TABLE_DIAGNOSIS_CYT " = :id_diagnos_cyt, DateOfBirth = :DateOfBirth, "
                                                  "Surname = :Surname, Name = :Name, Patronymic = :Patronymic, "
                                                  "NumberCard = :NumberCard, Gender = :Gender, Age = :Age, "
                                                  "DateOfDisease = :DateOfDisease, Comment = :Comment "
                                                  "WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":id_diagnos_gyst", data.at(0).toInt());
    query.bindValue(":id_diagnos_cyt", data.at(1).toInt());
    query.bindValue(":DateOfBirth", data.at(2).toDate());
    query.bindValue(":Surname", data.at(3).toString());
    query.bindValue(":Name", data.at(4).toString());
    query.bindValue(":Patronymic", data.at(5).toString());
    query.bindValue(":NumberCard", data.at(6).toString());
    query.bindValue(":Gender", data.at(7).toString());
    query.bindValue(":Age", data.at(8).toInt());
    query.bindValue(":DateOfDisease", data.at(9).toString());
    query.bindValue(":Comment", data.at(10).toString());
    if(!query.exec()){
        qDebug() << "Error update patient";
        qDebug() << query.lastError();
        return false;
    } else{
        qDebug() << "Update patient compleate";
        return true;
    }
}

bool Base::removePatient(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM " TABLE_CASESOFDISEASES " WHERE id = :id");
    query.bindValue(":id", id);
    if(!query.exec()){
        qDebug() << "Error remove patient";
        qDebug() << query.lastError();
        return false;
    }
    query.prepare("SELECT id FROM " TABLE_IMAGES " WHERE id_" TABLE_CASESOFDISEASES " = :id");
    query.bindValue(":id", id);
    query.exec();
    while(query.next())
        removeImage(id, query.value(0).toInt());
    return true;
}

QVariantList Base::GetPatientInformation(int id)
{
    QSqlQuery query;
    query.prepare("SELECT id_" TABLE_DIAGNOSIS_GYST ", id_" TABLE_DIAGNOSIS_CYT ", DateOfBirth, Surname, Name, Patronymic, "
                                               "NumberCard, Gender, DateOfDisease, Comment"
                                               " FROM " TABLE_CASESOFDISEASES " WHERE id = :id ");
    query.bindValue(":id", id);
    QVariantList data;
    if(!query.exec()){
        qDebug() << "Error get information about patient";
        qDebug() << query.lastError();
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
        data.append(0);
    } else
        while(query.next())
        {
            data.append(query.value(0));
            data.append(query.value(1));
            data.append(query.value(2));
            data.append(query.value(3));
            data.append(query.value(4));
            data.append(query.value(5));
            data.append(query.value(6));
            data.append(query.value(7));
            data.append(query.value(8));
            data.append(query.value(9));
        }
    return data;
}

bool Base::addNewImage(int id, const QVariantList &data, const QList<QPair<int, int> > &dataSymptoms)
{
    QSqlQuery query;

    query.prepare("SELECT NumberOfImages FROM " TABLE_CASESOFDISEASES " WHERE id = :id" );
    query.bindValue(":id", id);
    if(!query.exec()){
        qDebug() << "Error insert new image";
        qDebug() << query.lastError();
        return false;
    }
    int number = 5;
    while (query.next())
        number = query.value(0).toInt();

    query.prepare("UPDATE " TABLE_CASESOFDISEASES " SET NumberOfImages = :number WHERE id = :id");
    query.bindValue(":number", ++number);
    query.bindValue(":id", id);
    if(!query.exec()){
        qDebug() << "Error insert new image";
        qDebug() << query.lastError();
        return false;
    }

    query.prepare("INSERT INTO " TABLE_IMAGES " (id_" TABLE_CASESOFDISEASES ", id_" TABLE_DIAGNOSIS_GYST ", id_" TABLE_DIAGNOSIS_CYT ", id_" TABLE_ENLARGMENT ", "
                                                "id_" TABLE_COLOR ", SerialNumber, Comment)"
                  "VALUES (:id_patient, :id_diagnosis_gyst, :id_diagnosis_cyt,  :id_enlargment, :id_color, :number, :comment)");
    query.bindValue(":id_patient", id);
    query.bindValue(":id_diagnosis_gyst", data.at(1).toInt());
    query.bindValue(":id_diagnosis_cyt", data.at(2).toInt());
    query.bindValue(":id_color", data.at(3).toInt());
    query.bindValue(":id_enlargment", data.at(4).toInt());
    query.bindValue(":number", number);
    query.bindValue(":comment", data.at(5).toString());
    if(!query.exec()){
        qDebug() << "Error insert new image";
        qDebug() << query.lastError();
        return false;
    } else{
        qDebug() << "Insert new image compleate";
    }
    query.exec("SELECT id FROM " TABLE_IMAGES " ORDER BY id DESC LIMIT 1");
    while (query.next()) {
       number = query.value(0).toInt();
       qDebug() << number;
    }
    QDir dir;
    if(!dir.exists("ImageDir"))
        dir.mkdir("ImageDir");
    QPixmap pm = QPixmap(data.value(0).toString());
    pm.save("ImageDir/" + QString::number(id) + "_" + QString::number(number) + ".bmp");
    query.exec("UPDATE " TABLE_IMAGES " SET Image = 'ImageDir/" + QString::number(id) + "_" + QString::number(number) + ".bmp' "
               "WHERE id = " + QString::number(number));
    for(QPair<int, int> pair : dataSymptoms){
        query.prepare("INSERT INTO " TABLE_IMAGESYMPTOMS " (id_" TABLE_IMAGES ", "
                            "id_" TABLE_GROUPSYMPTOMS ", id_" TABLE_SYMPTOMS ") "
                            "VALUES (:id_image, :id_group, :id_symptom)");
        query.bindValue(":id_image", number);
        query.bindValue(":id_group", pair.first);
        query.bindValue(":id_symptom", pair.second);
        qDebug() << query.exec();
    }
    return true;
}

bool Base::updateImage(int id, const QVariantList &data, const QList<QPair<int, int> > &DS)
{
    QSqlQuery query;
    query.prepare("UPDATE " TABLE_IMAGES " SET id_" TABLE_DIAGNOSIS_GYST " = :id_diagnos_gyst, id_" TABLE_DIAGNOSIS_CYT " = :id_diagnos_cyt,  id_" TABLE_ENLARGMENT " = :id_enlargment, "
                                          "id_" TABLE_COLOR " = :id_color, Comment = :comment "
                                          "WHERE id = :id");
    query.bindValue(":id", id);
    query.bindValue(":id_diagnos_gyst", data.at(1).toInt());
    query.bindValue(":id_diagnos_cyt", data.at(2).toInt());
    query.bindValue(":id_color", data.at(3).toInt());
    query.bindValue(":id_enlargment", data.at(4).toInt());
    query.bindValue(":comment", data.at(5).toString());
    query.exec();
    query.exec("SELECT id, id_" TABLE_SYMPTOMS " FROM " TABLE_IMAGESYMPTOMS " "
               "WHERE id_" TABLE_IMAGES " = " + QString::number(id));
    std::list<QPair<int, int> > dataSymptoms = DS.toStdList();
    while(query.next()){
        bool isFind = true;
        for(QPair<int, int> pair: dataSymptoms)
            if(pair.second == query.value(1).toInt()){
                dataSymptoms.remove(pair);
                isFind = false;
            }
        if(isFind)
            removeImageSymptom(query.value(0).toInt());
    }
    for(QPair<int, int> pair: dataSymptoms){
        query.prepare("INSERT INTO " TABLE_IMAGESYMPTOMS " (id_" TABLE_IMAGES ", "
                            "id_" TABLE_GROUPSYMPTOMS ", id_" TABLE_SYMPTOMS ") "
                            "VALUES (:id_image, :id_group, :id_symptom)");
        query.bindValue(":id_image", id);
        query.bindValue(":id_group", pair.first);
        query.bindValue(":id_symptom", pair.second);
        qDebug() << query.exec();
    }
    return true;
}

bool Base::removeImage(int id_patient, int id_image)
{
    QSqlQuery query;

    query.prepare("DELETE FROM " TABLE_IMAGES " WHERE id = :id");
    query.bindValue(":id", id_image);
    if(!query.exec()){
        qDebug() << "Error remove image";
        qDebug() << query.lastError();
        return false;
    }
    QDir dir;
    dir.remove("ImageDir/" + QString::number(id_patient) + "_" + QString::number(id_image) + ".bmp");
    qDebug() << "Image remove";
    query.exec("SELECT id FROM " TABLE_IMAGESYMPTOMS " WHERE id_" TABLE_IMAGES " = " + QString::number(id_image));
    while(query.next())
        removeImageSymptom(query.value(0).toInt());
    query.prepare("SELECT NumberOfImages FROM " TABLE_CASESOFDISEASES " WHERE id = :id");
    query.bindValue(":id", id_patient);
    int number = 0;
    if(!query.exec()){
        qDebug() << "Error get num of images";
        qDebug() << query.lastError();
    } else
        while(query.next())
            number = query.value(0).toInt();

    query.prepare("UPDATE " TABLE_CASESOFDISEASES " SET NumberOfImages = :number WHERE id = :id");
    query.bindValue(":number", --number);
    query.bindValue(":id", id_patient);
    if(!query.exec()){
        qDebug() << "Error set num of images";
        qDebug() << query.lastError();
        return false;
    }
    number = 0;

    query.prepare("SELECT id FROM " TABLE_IMAGES " WHERE id_" TABLE_CASESOFDISEASES" = :id");
    query.bindValue(":id", id_patient);
    if(!query.exec()){
        qDebug() << "Error select id of image";
        qDebug() << query.lastError();
        return false;
    }
    while (query.next()) {
       QSqlQuery query2;
       qDebug() << "ID of updated image: " << query.value(0).toInt();
       query2.prepare("UPDATE " TABLE_IMAGES " SET SerialNumber = :number WHERE id = :id");
       query2.bindValue(":number", ++number);
       query2.bindValue(":id", query.value(0).toInt());
       if(!query2.exec()){
           qDebug() <<"Error update num of image";
           qDebug() << query2.lastError();
       }
    }
    return true;
}

QVariantList Base::GetImageInformation(int id)
{
    QSqlQuery query;

    query.prepare("SELECT id_" TABLE_DIAGNOSIS_GYST ", id_" TABLE_DIAGNOSIS_CYT ", id_" TABLE_COLOR ", "
                         "id_" TABLE_ENLARGMENT ", Image, Comment FROM " TABLE_IMAGES " WHERE id = :id");
    query.bindValue(":id", id);
    QVariantList data;
    if(!query.exec()){
        qDebug() << "Error get information of image";
        qDebug() << query.lastError();
        data.append(1);
        data.append(1);
        data.append(1);
        data.append(1);
        data.append(0);
        data.append(0);
    } else
        while(query.next())
        {
            data.append(query.value(0));
            data.append(query.value(1));
            data.append(query.value(2));
            data.append(query.value(3));
            data.append(query.value(4));
            data.append(query.value(5));
        }
    return data;
}

QList<int> Base::GetImageSymptoms(int id)
{
    QSqlQuery query;
    query.exec("SELECT id_" TABLE_SYMPTOMS " FROM " TABLE_IMAGESYMPTOMS " WHERE id_" TABLE_IMAGES " = " +QString::number(id));
    QList<int> symptoms;
    while(query.next())
        symptoms.append(query.value(0).toInt());
    return symptoms;
}

bool Base::removeImageSymptom(int id)
{
    QDir dir;
    dir.remove("MarkersDir/" + QString::number(id) + ".png");
    QSqlQuery query;
    return query.exec("DELETE FROM " TABLE_IMAGESYMPTOMS " WHERE id = " + QString::number(id));
}

int Base::analitic(const QString &nameTable)
{
    QSqlQuery query;

    query.exec("SELECT COUNT(*) FROM " + nameTable);
    while (query.next())
       return query.value(0).toInt();
    return 0;
}

bool Base::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(".\\" DATABASE_NAME);
    if(db.open()){
        return true;
    } else {
        return false;
    }
}

bool Base::restoreDataBase()
{
    if(this->openDataBase()){
           if(!this->createTable()){
               return false;
           } else {
               return true;
           }
       } else {
           qDebug() << "Не удалось восстановить базу данных";
           return false;
       }
}

void Base::closeDataBase()
{
    db.close();
}

bool Base::createTable()
{
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLE_CASESOFDISEASES " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "NumberCard VARCHAR(64) NOT NULL,"
                    "Surname VARCHAR(32) NOT NULL,"
                    "Name VARCHAR(32) NOT NULL,"
                    "Patronymic VARCHAR(32) NOT NULL,"
                    "DateOfBirth DATE NOT NULL,"
                    "Age INTEGER NOT NULL,"
                    "Gender VARCHAR(1) NOT NULL,"
                    "DateOfDisease DATE NOT NULL,"
                    "id_" TABLE_DIAGNOSIS_GYST " INTEGER NOT NULL,"
                    "id_" TABLE_DIAGNOSIS_CYT " INTEGER NOT NULL,"
                    "Comment VARCHAR(255),"
                    "NumberOfImages INTEGER NOT NULL"
                    ");")){
        qDebug() << "Database: error of create " << TABLE_CASESOFDISEASES;
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec( "CREATE TABLE " TABLE_IMAGES " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "id_" TABLE_CASESOFDISEASES " INTEGER NOT NULL,"
                    "id_" TABLE_DIAGNOSIS_GYST " INTEGER NOT NULL,"
                    "id_" TABLE_DIAGNOSIS_CYT " INTEGER NOT NULL,"
                    "id_" TABLE_ENLARGMENT " INTEGER NOT NULL,"
                    "id_" TABLE_COLOR " INTEGER NOT NULL,"
                    "Image VARCHAR(255),"
                    "SerialNumber INTEGER NOT NULL,"
                    "Comment VARCHAR(255)"
                    ");")){
        qDebug() << "Database: error of create " << TABLE_IMAGES;
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec( "CREATE TABLE " TABLE_COLOR " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "Color VARCHAR(255) NOT NULL"
                    ");")){
        qDebug() << "Database: error of create " << TABLE_COLOR;
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec( "CREATE TABLE " TABLE_ENLARGMENT " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "Enlargment VARCHAR(255) NOT NULL"
                    ");")){
        qDebug() << "Database: error of create " << TABLE_ENLARGMENT;
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec( "CREATE TABLE " TABLE_DIAGNOSIS_GYST " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "Diagnosis VARCHAR(255) NOT NULL"
                    ");")){
        qDebug() << "Database: error of create " << TABLE_DIAGNOSIS;
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec( "CREATE TABLE " TABLE_DIAGNOSIS_CYT " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "Diagnosis VARCHAR(255) NOT NULL"
                    ");")){
        qDebug() << "Database: error of create " << TABLE_DIAGNOSIS;
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec( "CREATE TABLE " TABLE_GROUPSYMPTOMS " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "GroupOfSymptoms VARCHAR(255) NOT NULL"
                    ");")){
        qDebug() << "Database: error of create " << TABLE_GROUPSYMPTOMS;
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec( "CREATE TABLE " TABLE_SYMPTOMS " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "id_" TABLE_GROUPSYMPTOMS " INTEGER NOT NULL,"
                    "Symptom VARCHAR(255) NOT NULL"
                    ");")){
        qDebug() << "Database: error of create " << TABLE_SYMPTOMS;
        qDebug() << query.lastError().text();
        return false;
    }
    if(!query.exec( "CREATE TABLE " TABLE_IMAGESYMPTOMS " ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "id_" TABLE_IMAGES " INTEGER NOT NULL,"
                    "id_" TABLE_GROUPSYMPTOMS " INTEGER NOT NULL,"
                    "id_" TABLE_SYMPTOMS " INTEGER NOT NULL,"
                    "MarkerName VARCHAR(10)"
                    ");")){
        qDebug() << "Database: error of create " << TABLE_IMAGESYMPTOMS;
        qDebug() << query.lastError().text();
        return false;
    }

    QFile file("Symptoms.txt");
    file.open(QIODevice::ReadOnly);
    this->InitialSymptoms(file.readAll());
    file.close();
    file.setFileName("color.txt");
    file.open(QIODevice::ReadOnly);
    this->InitialColorTable(file.readAll());
    file.close();
    file.setFileName("diagnosis_gyst.txt");
    file.open(QIODevice::ReadOnly);
    this->InitialDiagnosisGystTable(file.readAll());
    file.close();
    file.setFileName("diagnosis_cyt.txt");
    file.open(QIODevice::ReadOnly);
    this->InitialDiagnosisCytTable(file.readAll());
    file.close();
    file.setFileName("enlargment.txt");
    file.open(QIODevice::ReadOnly);
    this->InitialEnlargmentTable(file.readAll());
    file.close();

    qDebug() << "Database create ";
    return true;
}

bool Base::InitialColorTable(const QString &data)
{
    QStringList lines = data.split(QString("\r\n"));
    lines.pop_back();

    QSqlQuery query;
    for(QString l:lines){
        query.exec("INSERT INTO " TABLE_COLOR " ( Color) "
                                                "VALUES ('"+l+"');");
    }

    return true;
}

bool Base::InitialEnlargmentTable(const QString &data)
{
    QStringList lines = data.split(QString("\r\n"));
    lines.pop_back();

    QSqlQuery query;
    for(QString l: lines){
        query.exec("INSERT INTO " TABLE_ENLARGMENT " ( Enlargment) "
                                                "VALUES ('"+l+"');");
    }

    return true;
}

bool Base::InitialDiagnosisGystTable(const QString &data)
{
    QStringList lines = data.split(QString("\r\n"));
    lines.pop_back();

    QSqlQuery query;
    for(QString l:lines){
        qDebug() << query.exec("INSERT INTO " TABLE_DIAGNOSIS_GYST " ( Diagnosis) "
                                                "VALUES ('"+l+"');");
    }

    return true;
}

bool Base::InitialDiagnosisCytTable(const QString &data)
{
    QStringList lines = data.split(QString("\r\n"));
    lines.pop_back();

    QSqlQuery query;
    for(QString l:lines){
        qDebug() << query.exec("INSERT INTO " TABLE_DIAGNOSIS_CYT " ( Diagnosis) "
                                                "VALUES ('"+l+"');");
    }

    return true;
}

bool Base::InitialSymptoms(const QString &data)
{
    QStringList lines = data.split(QString("\r\n"));
    lines.pop_back();

    QSqlQuery query;
    int id = 0;
    for(QString l : lines){
        if(l.at(0) != QString(" ")){
            query.exec("INSERT INTO " TABLE_GROUPSYMPTOMS " ( GroupOfSymptoms)"
                                                          "VALUES ('" + l +"');");
            query.exec("SELECT id FROM " TABLE_GROUPSYMPTOMS
                       " WHERE GroupOfSymptoms = '" + l +"'");
            while (query.next()) {
               id = query.value(0).toInt();
            }
        }
        else
            query.exec("INSERT INTO " TABLE_SYMPTOMS " (id_" TABLE_GROUPSYMPTOMS ", Symptom)"
                       "VALUES ('" + QString::number(id) + "', '" + l +"')");
    }
    return true;
}

