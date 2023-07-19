#ifndef DIAGNOSISFILTER_H
#define DIAGNOSISFILTER_H

#include <QDialog>

namespace Ui {
class DiagnosisFilter;
}

class DiagnosisFilter : public QDialog
{
    Q_OBJECT

public:
    explicit DiagnosisFilter(QWidget *parent = 0);
    ~DiagnosisFilter();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DiagnosisFilter *ui;
};

#endif // DIAGNOSISFILTER_H
