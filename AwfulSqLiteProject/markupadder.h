#ifndef MARKUPADDER_H
#define MARKUPADDER_H

#include <QDialog>

namespace Ui {
class MarkUpAdder;
}

class MarkUpAdder : public QDialog
{
    Q_OBJECT

public:
    explicit MarkUpAdder(QWidget *parent = 0);
    ~MarkUpAdder();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MarkUpAdder *ui;
};

#endif // MARKUPADDER_H
