#ifndef OTCHETALDDIV_H
#define OTCHETALDDIV_H

#include <QMainWindow>

namespace Ui {
class OtchetAldDiv;
}

class OtchetAldDiv : public QMainWindow
{
    Q_OBJECT

public:
    explicit OtchetAldDiv(QWidget *parent = nullptr);
    ~OtchetAldDiv();
    public slots:
    void recieveData(uint g,uint g1,uint g2,uint g3);

private:
    Ui::OtchetAldDiv *ui;
};

#endif // OTCHETALDDIV_H
