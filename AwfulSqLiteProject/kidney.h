#ifndef KIDNEY_H
#define KIDNEY_H

#include <QDialog>

namespace Ui {
class Kidney;
}

class Kidney : public QDialog
{
    Q_OBJECT

public:
    explicit Kidney(QWidget *parent = 0);
    ~Kidney();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Kidney *ui;
};

#endif // KIDNEY_H
