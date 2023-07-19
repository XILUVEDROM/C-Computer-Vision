#ifndef BASE_H
#define BASE_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDir>
#include <QPixmap>
#include <QDebug>
#include <list>

#define DATABASE_HOSTNAME           "BreastCancer"
#define DATABASE_NAME               "BreastCancer.db"

#define TABLE_IMAGES                "Images"
#define TABLE_CASESOFDISEASES       "CasesOfDiseases"
#define TABLE_DIAGNOSIS             "Diagnosis"
//changed
#define TABLE_DIAGNOSIS_GYST        "Diagnosis_gyst"
#define TABLE_DIAGNOSIS_CYT         "Diagnosis_cyt"

#define TABLE_ENLARGMENT            "Enlargement"
#define TABLE_COLOR                 "Color"
#define TABLE_IMAGESYMPTOMS         "SymptomsOfTheImage"
#define TABLE_SYMPTOMS              "Symptoms"
#define TABLE_GROUPSYMPTOMS         "GroupOfSymptoms"
#define TABLE_MARKUP                "Markup"


class Base : public QObject
{
    Q_OBJECT
public:
    explicit Base(QObject *parent = nullptr);
    ~Base();

    void connectToDataBase();
    bool addNewPatient(const QVariantList &data);
    bool updatePatient(int id, const QVariantList &data);
    bool removePatient(int id);
    QVariantList GetPatientInformation(int id);

    bool addNewImage(int id, const QVariantList &data, const QList<QPair<int, int > > &dataSymptoms);
    bool updateImage(int id, const QVariantList &data, const QList<QPair<int, int > > &DS);
    bool removeImage(int id_patient, int id_image);
    QVariantList GetImageInformation(int id);
    QList<int> GetImageSymptoms(int id);

    int analitic(const QString &nameTable);
private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();

    bool InitialColorTable(const QString &data);
    bool InitialEnlargmentTable(const QString &data);
    bool InitialDiagnosisGystTable(const QString &data);
    bool InitialDiagnosisCytTable(const QString &data);
    bool InitialSymptoms(const QString &data);

    bool removeImageSymptom(int id);

signals:

public slots:
};

#endif // BASE_H
