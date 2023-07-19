#ifndef KIDNEYFILTER_H
#define KIDNEYFILTER_H

#include <QDialog>

namespace Ui {
class KidneyFilter;
}

class KidneyFilter : public QDialog
{
    Q_OBJECT

public:
    explicit KidneyFilter(QWidget *parent = 0);
    ~KidneyFilter();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::KidneyFilter *ui;
};

#endif // KIDNEYFILTER_H
