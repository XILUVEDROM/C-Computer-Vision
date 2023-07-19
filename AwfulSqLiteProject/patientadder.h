#ifndef PATIENTADDER_H
#define PATIENTADDER_H

#include <QDialog>

namespace Ui {
class PatientAdder;
}

class PatientAdder : public QDialog
{
    Q_OBJECT

public:
    explicit PatientAdder(QWidget *parent = 0);
    ~PatientAdder();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::PatientAdder *ui;
};

#endif // PATIENTADDER_H
