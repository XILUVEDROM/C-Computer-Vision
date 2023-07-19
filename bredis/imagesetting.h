#ifndef IMAGESETTING_H
#define IMAGESETTING_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QFileDialog>
#include <QPixmap>
#include <QBuffer>
#include <QFile>
#include <QTreeWidgetItem>
#include <QPair>


#include <QDebug>

namespace Ui {
class ImageSetting;
}

class ImageSetting : public QWidget
{
    Q_OBJECT

public:
    explicit ImageSetting(const QString &diagnosis_gyst, const QString &diagnosis_cyt, const QString &enlargment, const QString &color,
                          const QString &group, const QString &symptom, QWidget *parent = nullptr);
    explicit ImageSetting(const QString &diagnosis_gyst, const QString &diagnosis_cyt, const QString &enlargment, const QString &color,
                          const QString &group, const QString &symptom,
                          const QVariantList &dataImage, QList<int> &&symptoms, QWidget *parent = nullptr);
    ~ImageSetting();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

signals:
    void ImageData(const QVariantList &data, const QList<QPair<int, int > > &dataSymptoms);
    void closeWindow();

private:
    Ui::ImageSetting *ui;

    QSqlQueryModel *modelDiadnosis_gyst;
    QSqlQueryModel *modelDiadnosis_cyt;
    QSqlQueryModel *modelEnlargment;
    QSqlQueryModel *modelColor;

    QList<QTreeWidgetItem *> items;

    QString fileName;

    void createTree(const QString &group, const QString &symptom);
protected:
    void closeEvent(QCloseEvent *event);
};

#endif // IMAGESETTING_H
