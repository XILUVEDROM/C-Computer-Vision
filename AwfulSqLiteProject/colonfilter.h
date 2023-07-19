#ifndef COLONFILTER_H
#define COLONFILTER_H

#include <QDialog>

namespace Ui {
class ColonFilter;
}

class ColonFilter : public QDialog
{
    Q_OBJECT

public:
    explicit ColonFilter(QWidget *parent = 0);
    ~ColonFilter();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ColonFilter *ui;
};

#endif // COLONFILTER_H
