/*#ifndef ANALYTICFORCYTO_H
#define ANALYTICFORCYTO_H

#include <QDialog>

namespace Ui {
class AnalyticForCyto;
}

class AnalyticForCyto : public QDialog
{
    Q_OBJECT

public:
    explicit AnalyticForCyto(QWidget *parent = 0);
    ~AnalyticForCyto();

private:
    Ui::AnalyticForCyto *ui;
};

#endif // ANALYTICFORCYTO_H
