#ifndef PATIENTSETTING_H
#define PATIENTSETTING_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDebug>

namespace Ui {
class PatientSetting;
}

class PatientSetting : public QDialog
{
    Q_OBJECT

public:
    explicit PatientSetting(const QString Table_1, const QString Table_2, QWidget *parent = nullptr);
    explicit PatientSetting(const QString Table_1, const QString Table_2, const QVariantList &dataPatient,QWidget *parent = nullptr);
    ~PatientSetting();

private:
    Ui::PatientSetting *ui;

    QSqlQueryModel *model;

signals:
    void PatientData(const QVariantList &data);
    void closeWindow();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // PATIENTSETTING_H
