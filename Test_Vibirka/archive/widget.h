#ifndef WIDGET_H
#define WIDGET_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QComboBox>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>

class Widget : public QWidget
{
    Q_OBJECT
public:
    Widget(QWidget* parent = 0);
    ~Widget();

private:

    QPushButton* button1;
    QRadioButton* radiobutton1;
    QRadioButton* radiobutton2;
    QRadioButton* radiobutton3;
    QRadioButton* radiobutton4;
    QRadioButton* radiobutton5;
    QRadioButton* radiobutton6;
    QRadioButton* radiobutton7;
    QRadioButton* radiobutton8;
    QVBoxLayout* layout;
    QGridLayout* grid;
    QComboBox* comboBox1;
    QComboBox* comboBox2;
    QLineEdit* edit1;
    QLineEdit* edit2;
    QLineEdit* edit3;
    QLineEdit* edit4;
    QLineEdit* edit5;
    QLineEdit* edit6;
    QLineEdit* edit7;
    QLineEdit* edit8;
    QLineEdit* edit9;
    QLineEdit* edit10;
    QLineEdit* edit11;
    QLineEdit* edit12;
    QLineEdit* edit13;
    QLineEdit* edit14;
    QLineEdit* edit15;
    QLineEdit* edit16;
    QLineEdit* edit17;
    QLineEdit* edit18;
    QLabel* label1;
    QLabel* label2;
    QLabel* label3;
    QLabel* label4;
    QLabel* label5;
    QLabel* label6;
    QLabel* label7;
    QLabel* label8;
    QLabel* label9;
    QLabel* label10;
    QLabel* label11;
    QLabel* label12;
    QLabel* label13;
    QLabel* label14;
    QLabel* label15;
    QLabel* label16;
    QLabel* label17;
    QLabel* label18;
    QLabel* label19;
    QLabel* label20;

private slots:
    void classificationButtonClick();

private:
    void SVMClassification(double C,qreal gamma, int pow, cv::ml::SVM::KernelTypes types);
    double trainAndPredict(cv::ml::SVM::KernelTypes types_value,  qreal& gamma_values,  double& c_values, int pow_value, cv::Mat& trainingDataMat, cv::Mat& labelsMat, cv::Mat& testDataMat, cv::Mat& testLabelsMat);
    void LGClassification(double Learning, double Iterator, cv::ml::LogisticRegression::RegKinds Reg, int  Size);
    void RandomForestClassification(int MaxDepth, int MinSampleCount, int RegressionAccuracy, int MaxCategories, int ActiveVarCount, int Count, float Eps);
    void KNearestClassify(int DefaultN, int AlgoritmTipe, int Emax);
    void MLPClassification();
    double trainAndPredict1(QVector<cv::ml::SVM::KernelTypes>& types, int it, QVector<qreal>& gamma_values, int ig, QVector<qreal>& c_values, int ic, QVector<int>& pow_values, int ip, cv::Mat& trainingDataMat, cv::Mat& labelsMat, cv::Mat& testDataMat, cv::Mat& testLabelsMat);
    void SVMClassification1();
    void KNearestClassify1();
    void RandomForestClassification1();
    cv::TermCriteria TC(int iters, double eps);
};

#endif // WIDGET_H
