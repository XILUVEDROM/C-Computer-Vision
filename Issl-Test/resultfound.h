#ifndef RESULTFOUND_H
#define RESULTFOUND_H

#include <QMainWindow>

namespace Ui {
class ResultFound;
}

class ResultFound : public QMainWindow
{
    Q_OBJECT

public:
    explicit ResultFound(QWidget *parent = nullptr);
    ~ResultFound();

private:
    Ui::ResultFound *ui;
};

#endif // RESULTFOUND_H
