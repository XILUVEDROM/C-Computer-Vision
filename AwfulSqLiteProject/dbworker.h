#ifndef DBWORKER_H
#define DBWORKER_H

#include <QObject>
#include <QMainWindow>
#include <QString>
#include <QDateTime>
#include <QStringList>
#include <QIntegerForSize>
#include <QTableView>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>


class dbworker
{
public:
    dbworker();
   ~dbworker();

    bool checkIfAuthCorrect(QString login, QString password);
    void insertMarkUp(QString gystoDiag, QString cytDiag, QString features);
    void insertImage(QString gystoDiag,QString cytDiag,QString colorofImage,QString typeOfImage,
                     QString zoomOfImage,QDate dateOfAdding, QString linkToImage, int pat_id, QString table);
    void insertPatientWhileInsertingImage(QString historyNum, QDate dateOfBirth, int dateOfFallingIll,
                                          int age, QString sex, QString gystoDiag, QString cytoDiag,
                                          QString organType, QString table);
    void insertPatient(QString historyNum, QDate dateOfBirth, int dateOfFallingIll,
                                          int age, QString sex, QString gystoDiag, QString cytoDiag,
                                          QString organType, QString table);



    int getLastInsertedPatient();
    int getLastInsertedMarkUpId();

    QStringList fillGystoDiagInInsertForm(QString table);
    QStringList fillCytoDiagInInsertForm(QString table);
    QStringList fillColorOfImage(QString table);
    QStringList fillScope(QString table);
    QPair<QStringList,QStringList> fillDiagnosisInComboBoxes(QString table);
    QPair<QString,QString> selectCytoAndGystoDiagForMarkUp(int markupId);
    QStringList selectFeatureForMarkUp(int markupId);
    QSqlTableModel* selectImage(QWidget *parent,QString params,QString table);
    QSqlTableModel* selectImageByDiagnosis(QWidget *parent,QString gystoParams,QString cytoParams,QString table);
    QSqlTableModel* selectPatientById(QWidget *parent, int pat_id);

    static QString userRole;

    static QString getUserRole();
    static void setUserRole(const QString &value);

private:
    QSqlDatabase db;



};

#endif // DBWORKER_H
