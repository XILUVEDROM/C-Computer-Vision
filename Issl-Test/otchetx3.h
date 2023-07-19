#ifndef OTCHETX3_H
#define OTCHETX3_H

#include <QMainWindow>

namespace Ui {
class Otchetx3;
}

class Otchetx3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Otchetx3(QWidget *parent = nullptr);
    ~Otchetx3();
   public slots:
    void recieveData(QString str,uint g1,uint g2,float g3,uint g4, uint g5);
private:
    Ui::Otchetx3 *ui;
};

#endif // OTCHETX3_H
