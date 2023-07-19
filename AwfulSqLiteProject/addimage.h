#ifndef ADDIMAGE_H
#define ADDIMAGE_H

#include <QDialog>
#include "dbworker.h"

namespace Ui {
class AddImage;
}

class AddImage : public QDialog
{
    Q_OBJECT

public:
    explicit AddImage(QWidget *parent = 0);
    ~AddImage();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddImage *ui;
};

#endif // ADDIMAGE_H
