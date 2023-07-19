#ifndef COLON_H
#define COLON_H

#include <QDialog>

namespace Ui {
class Colon;
}

class Colon : public QDialog
{
    Q_OBJECT

public:
    explicit Colon(QWidget *parent = 0);
    ~Colon();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::Colon *ui;
};

#endif // COLON_H
