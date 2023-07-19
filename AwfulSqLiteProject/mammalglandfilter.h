#ifndef MAMMALGLANDFILTER_H
#define MAMMALGLANDFILTER_H

#include <QDialog>

namespace Ui {
class MammalGlandFilter;
}

class MammalGlandFilter : public QDialog
{
    Q_OBJECT

public:
    explicit MammalGlandFilter(QWidget *parent = 0);
    ~MammalGlandFilter();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MammalGlandFilter *ui;
};

#endif // MAMMALGLANDFILTER_H
