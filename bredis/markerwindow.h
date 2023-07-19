#ifndef MARKERWINDOW_H
#define MARKERWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QResizeEvent>
#include <QPixmap>
#include <QImage>
#include <QDir>
#include <QSqlQuery>
#include "paintscene.h"

namespace Ui {
class MarkerWindow;
}

class MarkerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MarkerWindow(QString fileName, int idSymptom, QWidget *parent = nullptr);
    ~MarkerWindow();

private:
    Ui::MarkerWindow *ui;
    PaintScene *scene;  // Объявляем кастомную графическую сцену
    QTimer *timer;      /* Определяем таймер для подготовки актуальных размеров
                         * графической сцены
                         * */
    QGraphicsPixmapItem *pmItem;
    int id;

   void initialColor();

private slots:
   void on_comboBox_currentIndexChanged(int index);
   void on_comboBox_2_currentIndexChanged(const QString &arg1);
   void on_comboBox_3_currentIndexChanged(int index);
   void on_pushButton_5_clicked();

   void on_pushButton_clicked();

   void on_pushButton_2_clicked();

   void on_pushButton_3_clicked();

   void on_pushButton_4_clicked();

signals:
   void closeWindow();
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MARKERWINDOW_H
