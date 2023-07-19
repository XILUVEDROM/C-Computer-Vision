#ifndef IMAGEMARKUP_H
#define IMAGEMARKUP_H

#include <QDialog>
#include <QPainter>

namespace Ui {
class ImageMarkUp;
}

class ImageMarkUp : public QDialog
{
    Q_OBJECT

public:
    explicit ImageMarkUp(QWidget *parent = 0);
    ~ImageMarkUp();

    bool eventFilter(QObject* watched, QEvent* event) override;
    bool mousePressed;
    bool drawStarted;

protected:
    void paintEvent(QPaintEvent *event);


private slots:
    void on_pushButton_2_clicked();

private:
    Ui::ImageMarkUp *ui;
    QRect mRect;
    QPainter painter;
    QPixmap mPix;

};

#endif // IMAGEMARKUP_H
