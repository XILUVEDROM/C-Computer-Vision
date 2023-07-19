#ifndef CERTIFICATEDIALOG_H
#define CERTIFICATEDIALOG_H

#include <QMainWindow>

namespace Ui {
class СertificateВialog;
}

class СertificateВialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit СertificateВialog(QWidget *parent = nullptr);
    ~СertificateВialog();

private:
    Ui::СertificateВialog *ui;
};

#endif // CERTIFICATEDIALOG_H
