#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <vector>
#include <algorithm>
#include "base.h"
#include "patientsetting.h"
#include "imagesetting.h"
#include "markerwindow.h"
#include "diagramma.h"
#include <QModelIndex>

#include<QTreeWidget>

namespace Ui {
class MainWindow;
}

struct PatientImageVisualisation;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Base *db;
    QSqlRelationalTableModel *model;

private slots:
    void on_pushButton_clicked();

    void setNewPatientData(const QVariantList &data);
    void UpdatePatient(const QVariantList &data);
    void closePatientSetting();

    void setNewImage(const QVariantList &data, const QList<QPair<int, int > > &dataSymptoms);
    void updateImage(const QVariantList &data, const QList<QPair<int, int> > &dataSymptoms);
    void closeImageSetting();

    void closeMarkerWindow();

    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_tableView_clicked(const QModelIndex &index);
    void on_pushButton_4_clicked();
    void on_tabWidget_currentChanged(int index);
    void on_pushButton_6_clicked();
    void on_pushButton_5_clicked();

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_treeWidget_itemClicked(QTreeWidgetItem *item, int column);

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_radioButton_3_clicked();

    void on_radioButton_4_clicked();

    void on_treeWidget_2_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_tableWidget_2_clicked(const QModelIndex &index);

    void on_tableWidget_3_clicked(const QModelIndex &index);

    void on_pushButton_9_clicked();

    void on_treeWidget_3_itemClicked(QTreeWidgetItem *item, int column);

   // void on_tabWidget_2_currentChanged(int index);

    void on_pushButton_10_clicked();

    void on_tableWidget_clicked(const QModelIndex &index);

    void test();

    int serega();

private:
    Ui::MainWindow *ui;
    Diagramma *DI =nullptr;
    PatientSetting *PS = nullptr;
    ImageSetting *IS = nullptr;
    MarkerWindow *Marker = nullptr;
    int id_Patient = -1;
    int id_Image = -1;
    QList<PatientImageVisualisation *> patientImage;
    QList<PatientImageVisualisation *> findImage;
    QList<QTreeWidgetItem *> itemsSearch;
    QGraphicsScene *sceneImage, *sceneFindImage;
    QGraphicsPixmapItem *markerView = nullptr;
    QGraphicsPixmapItem *markerFindView = nullptr;
    void setupModel();
    void setupSearch();
    void createUI();
    PatientImageVisualisation *selectImagesInformation(int id);
    void setChart();
    void setDiagnozChart();

    QWidget *findPatientsWidget = nullptr;
    QWidget *findImagesWidget = nullptr;
};


struct PatientImageVisualisation
{
    int id;
    int idPatient;
    QString fileName;
    QString number;
    QString color;
    QString enlargment;
    QString diagnos_gyst;
    QString diagnos_cyt;
    QString comment;

    QList<QTreeWidgetItem *> items;
};

#endif // MAINWINDOW_H
