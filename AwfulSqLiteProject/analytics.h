/*#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <QDialog>

namespace Ui {
class Analytics;
}

class Analytics : public QDialog
{
    Q_OBJECT

public:
    explicit Analytics(QWidget *parent = 0);
    ~Analytics();

private:
    Ui::Analytics *ui;
};

#endif // ANALYTICS_H
