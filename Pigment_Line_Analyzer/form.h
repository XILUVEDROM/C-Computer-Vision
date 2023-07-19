#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QGraphicsScene>
#include <QFileDialog>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    QString openFromBase();
    void createForm(const std::vector<QString> &results);
    ~Form();

private slots:
    void on_horizontalScrollBar_valueChanged(int value);

private:
    Ui::Form *ui;
    QString file_name;
    std::vector<QString> res;
    int count;
};

#endif // FORM_H
