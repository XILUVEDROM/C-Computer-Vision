#ifndef TABLET_H
#define TABLET_H

#include <QMainWindow>
#include "mainwindow.h"

namespace Ui {
class tablet;
}

class MainWindow; //необходимо упоминание класса???

class tablet : public QMainWindow
{
    Q_OBJECT

public:
    tablet();
    tablet(QWidget *parent);
    ~tablet();

    QWidget* createButtonWidget() const;

private slots:
    void onBtnClicked();

private:
    //MainWindow::MainWindow parentw;
    Ui::tablet *ui;

    MainWindow *parentw;

};

#endif // TABLET_H
