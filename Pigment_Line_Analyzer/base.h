#ifndef BASE_H
#define BASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/*!
 * \brief Класс для работы с базой данных
 */

class Base : public QObject
{
    Q_OBJECT
public:
    explicit Base(QObject *parent = nullptr);
    ~Base();
    void connectToDataBase();
    bool insertIntoDataBase(const QString &file_name, const QString &result_name);
    void selectFromResults(const QString &file_name, std::vector<QString> &result);

private:
    QSqlDatabase db;
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTableNames();
    bool createTableResults();
    bool createTableFeatures();
    bool insertIntoTableNames(const QString &file_name);
    bool insertIntoTableResults(const QString &file_name, const QString &result_name);
    bool insertIntoTableFeatures(const QString &file_name, const QString &result_name, const std::vector<QString> &points, const std::vector<std::vector<int>> &features);

};

#endif // BASE_H
