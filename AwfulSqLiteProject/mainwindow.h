#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <dbworker.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    //MainWindow(QWidget *parent = 0, int test);

    ~MainWindow();

    bool eventFilter(QObject* watched, QEvent* event) override;

    static QString getTable();

    static dbworker DW;

    static int getSelectionLogic();

    static dbworker getDW();


    static void copyAndCreateDirIfNessesaryForMarkupInsertion();
    static void setFilterString(const QString &value);
    static void setGystoDiagnosisString(const QString &value);
    static void setCytoDiagnosisString(const QString &value);
    static QString getFilterString();
    static QString getGystoDiagnosisString();
    static QString getCytoDiagnosisString();
    static QString getImagePathForMarkup();
    static void setPathForInsertingMarkUp(const QString &value);
    static QStringList getFeatureForAnalytics();
    static QStringList getGystoDiagnosisForAnalytic();
    static QStringList getCytoDiagnosisForAnalytic();
    static QVector<double> getPercentageFeatureForAnalytics();
    static QVector<double> getGystoPercentageDiagnosisForAnalytics();
    static QVector<double> getCytoPercentageDiagnosisForAnalytics();

    int getCurrentMarkUpId() const;
    void setCurrentMarkUpId(int value);

    QStringList getCytoDiagForMarkUp() const;
    void setCytoDiagForMarkUp(const QStringList &value);

    QStringList getGystoDiagForMarkUp() const;
    void setGystoDiagForMarkUp(const QStringList &value);

    QVector<int> getAllFilesInsideMarkupFolder() const;
    void setAllFilesInsideMarkupFolder(const QVector<int> &value);

    QStringList getFeaturesForMarkUp() const;
    void setFeaturesForMarkUp(const QStringList &value);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_action_triggered();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();


    void on_pushButton_2_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_14_clicked();

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(const QString &arg1);

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_5_clicked();

    void on_checkBox_stateChanged(int arg1);

    void on_action_3_triggered();

    void on_action_2_triggered();

    void on_comboBox_4_currentIndexChanged(int index);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_action_7_triggered();

    void on_action_6_triggered();

    void on_pushButton_15_clicked();

    void on_checkBox_2_stateChanged(int arg1);

    void on_pushButton_17_clicked();

    void on_pushButton_16_clicked();

private:
    Ui::MainWindow *ui;

    QPixmap image;
    QImage  *imageObject;
    QGraphicsScene *scene;
    QSqlTableModel *tmpmodel;
    int currentId;
    int currentIdImageList;
    int currentMarkUpId;

    QStringList cytoDiagForMarkUp;
    QStringList gystoDiagForMarkUp;
    QStringList featuresForMarkUp;
    QVector<int> allFilesInsideMarkupFolder;

    QSqlTableModel *getTmpmodel() const;
    void setTmpmodel(QSqlTableModel *value);

    int getCurrentId() const;
    void setCurrentId(int value);

    int getCurrentIdImageList() const;
    void setCurrentIdImageList(int value);
};

#endif // MAINWINDOW_H
