#ifndef MAMMALGLAND_H
#define MAMMALGLAND_H

#include <QDialog>

namespace Ui {
class MammalGland;
}

class MammalGland : public QDialog
{
    Q_OBJECT

public:
    explicit MammalGland(QWidget *parent = 0);
    ~MammalGland();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MammalGland *ui;
};

#endif // MAMMALGLAND_H
