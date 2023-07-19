#ifndef ERROR_H
#define ERROR_H

#include <QMainWindow>

namespace Ui {
class Error;
}

class Error : public QMainWindow
{
    Q_OBJECT

public:
    explicit Error(QWidget *parent = nullptr);
    ~Error();
public slots:
void recieveData(QString Str);
private:
    Ui::Error *ui;
};

#endif // ERROR_H
