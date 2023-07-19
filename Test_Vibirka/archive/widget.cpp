#include "widget.h"
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/ml.hpp>
#include "cvservice.h"
#include <QtDebug>
#include <qFile>
#include <fstream>

#include <cstdio>
#include <vector>


#include <ActiveQt\QAxBase.h>
#include <ActiveQt\QAxObject.h>
#include <ActiveQt\QAxWidget.h>

#pragma once

using namespace cv::ml;
using namespace cv;
using namespace std;

Widget::Widget(QWidget* parent)
    : QWidget(parent)
{
    button1 = new QPushButton(this);

    radiobutton1 = new QRadioButton(this);
    radiobutton2 = new QRadioButton(this);
    radiobutton3 = new QRadioButton(this);
    radiobutton4 = new QRadioButton(this);
    radiobutton5 = new QRadioButton(this);
    radiobutton6 = new QRadioButton(this);
    radiobutton7 = new QRadioButton(this);
    radiobutton8 = new QRadioButton(this);
    grid = new QGridLayout(this);
    comboBox1 = new QComboBox(this);
    //edit1 = new QLineEdit(this);
    edit2 = new QLineEdit(this);
    //edit2->setValidator(new QRegExpValidator(QRegExp("[0-9]*"),edit2));
    //doubleValidator->setNotation(QDoubleValidator::StandardNotation);
    edit2->setValidator(new QDoubleValidator(0, 100, 3, this));
    //QDoubleValidator* validFirstX = new QDoubleValidator(0, 1000, 3, this->edit2);
    //validFirstX->setNotation(QDoubleValidator::StandardNotation);
    edit3 = new QLineEdit(this);
    edit3->setValidator(new QDoubleValidator(0, 100, 3, this));
    edit4 = new QLineEdit(this);
    edit4->setValidator(new QIntValidator(0, 1, this));
    //---------------------------------------------------------
    label1 = new QLabel(this);
    label2 = new QLabel(this);
    label3 = new QLabel(this);
    label4 = new QLabel(this);
    label5 = new QLabel(this);
    label6 = new QLabel(this);
    label7 = new QLabel(this);
    label8 = new QLabel(this);
    label9 = new QLabel(this);
    label10 = new QLabel(this);
    label11 = new QLabel(this);
    label12 = new QLabel(this);
    label13 = new QLabel(this);
    label14 = new QLabel(this);
    label15 = new QLabel(this);
    label16 = new QLabel(this);
    label17 = new QLabel(this);
    label18 = new QLabel(this);
    /*label19 = new QLabel(this);
    label20 = new QLabel(this);*/
    //---------------------------------
    edit5 = new QLineEdit(this);
    edit6 = new QLineEdit(this);
    edit7 = new QLineEdit(this);
    edit9 = new QLineEdit(this);
    edit10 = new QLineEdit(this);
    edit11 = new QLineEdit(this);
    edit12 = new QLineEdit(this);
    edit13 = new QLineEdit(this);
    edit14 = new QLineEdit(this);
    edit15 = new QLineEdit(this);
    edit16 = new QLineEdit(this);
    edit17 = new QLineEdit(this);
    edit18 = new QLineEdit(this);
    //--------------------------------------------------------
    comboBox2 = new QComboBox(this);
    grid->addWidget(comboBox2, 8, 3);


    grid->addWidget(comboBox1, 1, 3);
    grid->addWidget(label1, 1, 2);
    grid->addWidget(label2, 2, 2);
    grid->addWidget(label3, 3, 2);
    grid->addWidget(label4, 4, 2);
    //------------------------------------
    grid->addWidget(label5, 6, 2);
    grid->addWidget(label6, 7, 2);
    grid->addWidget(label7, 8, 2);
    grid->addWidget(label8, 9, 2);
    //---------------------------------------------------------
    grid->addWidget(label9, 11, 2);
    grid->addWidget(label10, 12, 2);
    grid->addWidget(label11, 13, 2);
    grid->addWidget(label12, 14, 2);
    grid->addWidget(label13, 15, 2);
    grid->addWidget(label14, 16, 2);
    grid->addWidget(label15, 17, 2);

    //--------------------------------------------------------
    grid->addWidget(label16, 20, 2);
    grid->addWidget(label17, 21, 2);
    grid->addWidget(label18, 22, 2);

    //---------------------------------------------------------
    //grid->addWidget(edit1, 1, 3);
    grid->addWidget(edit2, 2, 3);
    grid->addWidget(edit3, 3, 3);
    grid->addWidget(edit4, 4, 3);
    label1->setText("Type of Kernel");
    label2->setText("C");
    label3->setText("Gamma");
    label4->setText("Power");

    label5->setText("Learning Rate");
    label6->setText("Iterator");
    label7->setText("Reg");
    label8->setText("Size");

    label9->setText("MaxDepht");// MaxDepht, int MinSampleCount, int RegressionAccuracy, int MaxCategories, int ActiveVarCount,int Count , float Eps
    label10->setText("MinSampleCount");
    label11->setText("RegressionAccuracy");
    label12->setText("MaxCategories");
    label13->setText("ActiveVarCount");
    label14->setText("Count");
    label15->setText("Eps");

    label16->setText("DefaultN");//DefaultN
    label17->setText("AlgoritmTipe");
    label18->setText("Emax");

    grid->addWidget(edit5, 6, 3);
    edit5->setValidator(new QDoubleValidator(0, 100, 10, this));
    grid->addWidget(edit6, 7, 3);
    edit6->setValidator(new QDoubleValidator(0, 100,1,  this));
    grid->addWidget(edit7, 9, 3);
    edit7->setValidator(new QIntValidator(0, 1, this));
    //-----------------------------------------------------------
    grid->addWidget(edit9, 11, 3);
    edit9->setValidator(new QIntValidator(0, 10, this));
    grid->addWidget(edit10, 12, 3);
    edit10->setValidator(new QIntValidator(0, 10, this));
    grid->addWidget(edit11, 13, 3);
    edit11->setValidator(new QIntValidator(0, 1, this));
    grid->addWidget(edit12, 14, 3);
    edit12->setValidator(new QIntValidator(0, 10, this));
    grid->addWidget(edit13, 15, 3);
    edit13->setValidator(new QIntValidator(0, 1, this));
    grid->addWidget(edit14, 16, 3);
    edit14->setValidator(new QIntValidator(0, 100, this));
    grid->addWidget(edit15, 17, 3);
    edit15->setValidator(new QDoubleValidator(0, 100,5, this));

    grid->addWidget(edit16, 20, 3);
    edit16->setValidator(new QIntValidator(0, 10, this));
    grid->addWidget(edit17, 21, 3);
    edit17->setValidator(new QIntValidator(0, 0, this));
    grid->addWidget(edit18, 22, 3);
    edit18->setValidator(new QIntValidator(0, 1, this));

    button1->setText("to Clisiffie ");

    grid->addWidget(button1, 6, 5);

    /*layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setDirection(QVBoxLayout::BottomToTop);
    layout->setSpacing(50);*/


    radiobutton1->setText("SVM");
    // layout->addWidget(radiobutton1);
    grid->addWidget(radiobutton1, 1, 1);
    radiobutton2->setText("Logistic Regression");
    // layout->addWidget(radiobutton2);
    grid->addWidget(radiobutton2, 5, 1);
    radiobutton3->setText("Neural Network");
    //  layout->addWidget(radiobutton3);
    grid->addWidget(radiobutton3, 23, 1);
    radiobutton4->setText("Random Forest");
    grid->addWidget(radiobutton4, 10, 1);
    //   layout->addWidget(radiobutton4);
    radiobutton5->setText("KNearest");
    grid->addWidget(radiobutton5, 19, 1);
    radiobutton6->setText("SVM_auto");
    //  layout->addWidget(radiobutton3);
    grid->addWidget(radiobutton6, 24, 1);
    radiobutton7->setText("RanromForest _ auto");
    //  layout->addWidget(radiobutton3);
    grid->addWidget(radiobutton7, 26, 1);
    radiobutton8->setText("KNearst_auto");
    //  layout->addWidget(radiobutton3);
    grid->addWidget(radiobutton8, 27, 1);



    //SVM::LINEAR, SVM::RBF, SVM::SIGMOID ,*/ SVM::POLY
    comboBox1->addItem("LINEAR");
    comboBox1->addItem("RBF");
    comboBox1->addItem("SIGMOID");
    comboBox1->addItem("POLY");
    comboBox2->addItem("REG_L2");
    comboBox2->addItem("REG_L1");
    comboBox2->addItem("REG_DISABLE");

    QObject::connect(button1, SIGNAL(clicked()), SLOT(classificationButtonClick()));


}

Widget::~Widget()
{

}

inline TermCriteria Widget::TC(int iters, double eps)
{
    return TermCriteria(TermCriteria::MAX_ITER + (eps > 0 ? TermCriteria::EPS : 0), iters, eps);
}

void Widget::classificationButtonClick()
{
    if (radiobutton1->isChecked())
    {//qreal& gamma_value, double& c_values, int pow_values
        //edit2 ->setValidator(new QDoubleValidator(0, 100,2, this));
        QString CString = edit2->text();
        CString.replace(',', '.');
        double C = CString.toDouble();
        QString GammaString = edit3->text();
        GammaString.replace(',', '.');
        qreal gamma = GammaString.toDouble();
        QString powString = edit4->text();
        int pow = powString.toInt();
        QString Kernel = comboBox1->currentText();
        cv::ml::SVM::KernelTypes types;
        //SVM::LINEAR, SVM::RBF, SVM::SIGMOID , SVM::POLY
        if (Kernel == "LINEAR")
            types = SVM::LINEAR;
        if (Kernel == "RBF")
            types = SVM::RBF;
        if (Kernel == "SIGMOID")
            types = SVM::SIGMOID;
        if (Kernel == "POLY")
            types = SVM::POLY;
        SVMClassification(C, gamma, pow, types);
        //SVMClassification1();
    }

    if (radiobutton2->isChecked())
    {
        QString LearningString = edit5->text();
        LearningString.replace(',', '.');
        double Learning = LearningString.toDouble();
        QString IteratorString = edit6->text();
        IteratorString.replace(',', '.');
        double Iterator = IteratorString.toDouble();
        QString SizeString = edit7->text();
        SizeString.replace(',', '.');
        int Size = SizeString.toInt();
        QString RegString = comboBox2->currentText();
        cv::ml::LogisticRegression::RegKinds Reg;
        if (RegString == "REG_L1")
            Reg = LogisticRegression::REG_L1;
        if (RegString == "REG_L2")
            Reg = LogisticRegression::REG_L2;
        if (RegString == "REG_DISABLE")//REG_DISABLE
            Reg = LogisticRegression::REG_DISABLE;
        LGClassification(Learning, Iterator, Reg, Size);
    }
    //int MaxDepht, int MinSampleCount, int RegressionAccuracy, int MaxCategories, int ActiveVarCount,int Count , float Eps
    if (radiobutton4->isChecked())//int MaxDepth, int MinSampleCount, int RegressionAccuracy, int MaxCategories, int ActiveVarCount,int Count , float Eps
    {
        QString MaxDephtS = edit9->text();
        int MaxDepth = MaxDephtS.toInt();
        QString MinSampleCountS = edit10->text();
        int MinSampleCount = MinSampleCountS.toInt();
        QString RegressionAccuracyS = edit11->text();
        int RegressionAccuracy = RegressionAccuracyS.toInt();
        QString MaxCategoriesS = edit12->text();
        int MaxCategories = MaxCategoriesS.toInt();
        QString ActiveVarCountS = edit13->text();
        int ActiveVarCount = ActiveVarCountS.toInt();
        QString  CountS = edit14->text();
        int  Count = CountS.toInt();
        QString EpsS = edit9->text();
        float Eps = EpsS.toFloat();

        RandomForestClassification(MaxDepth, MinSampleCount, RegressionAccuracy, MaxCategories, ActiveVarCount, Count, Eps);
        //RandomForestClassification1();
    }
    if (radiobutton3->isChecked())
        MLPClassification();
    if (radiobutton5->isChecked())
    {
        QString Default_Nearsts_S = edit16->text();
        int Default_Nearsts = Default_Nearsts_S.toInt();
        QString Algoritm_S = edit17->text();
        int Algoritm = Algoritm_S.toInt();
        QString Emaximum_S = edit18->text();
        int Emaximum = Emaximum_S.toInt();
        KNearestClassify(Default_Nearsts, Algoritm, Emaximum);//int DefaultN,int AlgoritmTipe,int Emax
    }

    if (radiobutton6->isChecked())
    {
        SVMClassification1();
    }

    if (radiobutton7->isChecked())
    {
        RandomForestClassification1();
    }
    if (radiobutton8->isChecked())
    {
        KNearestClassify1();
    }
}


void Widget::RandomForestClassification(int MaxDepth, int MinSampleCount, int RegressionAccuracy, int MaxCategories, int ActiveVarCount, int Count, float Eps)
{

    QVector<cv::Mat> mat1;
    QVector<QString> name1;
    setlocale(LC_ALL, "RUS");
    mat1 = CvServices::readAllFiles("C:/Users/user/Documents/TestViborka", 0.25f);
    name1 = CvServices::readAllFilesName("C:/Users/user/Documents/TestViborka", 0.25f);
    //QVector<String> names = CvServices::readMatNames();
    //std::vector<string> stdvector = name1.toStdVector();
    Mat trainingDataMat = mat1.at(0);
    Mat labelsMat = mat1.at(2);
    Mat testDataMat = mat1.at(1);
    Mat testLabelsMat = mat1.at(3);


    double excellent = 0;

    double MaxAccuracy = 0;

    //QVector<int> MaxDepth = { 10 };//10
    //QVector<int> MinSampleCount = {8, 9,10};//8
    //QVector<int> RegressionAccuracy = { 0/*1,2,3*/};//0
    ////QVector<bool> UseSurrogates = { false, true };
    //QVector<int> MaxCategories = {/*5,6,*/7/*,8,9,*/ /*10, 11*//*,15,20,30*/ };// для категориальных переменных с возможными значениями N> max_categories.
    ////QVector<int> Priors(Mat()); Массив априорных вероятностей класса, отсортированный по значению метки класса.
    //QVector<bool > CalculateVarImportance = { true , false };
    //QVector<int> ActiveVarCount = {/* 1,2,3,4,*/5/*,6,7 */};//Размер случайно выбранного подмножества объектов в каждом узле дерева, которые используются для нахождения наилучшего разбиения. Если вы установите его в 0, то размер будет установлен в квадратный корень от общего числа объектов. Значение по умолчанию 0.
    //QVector<int> Count = { /*50,55,98,75,*/99/*100,105*/};//100
    //QVector<float> Eps = {0.0085/*,0.0087,0.0088, 0.0083,0.0084,0.00855,0.00849,0.00851,0.00853,0.00848,*//* 0.001, 0.0001 */};//0.009


    int BestMaxDepth = 0;
    int BestMinSampleCount = 0;
    int BestRegressionAccuracy = 0;
    bool BestUseSurrogates = false;
    int BestMaxCategories = 0;
    //QVector<int> Priors(Mat());
    bool BestCalculateVarImportance = true;
    int BestActiveVarCount = 0;
    int BestCount = 0;
    float BestEps = 0;



    //for (int id = 0; id < MaxDepth.size(); ++id)
    //{
    //	for (int is = 0; is < MinSampleCount.size(); ++is)
    //	{
    //		for (int ir = 0; ir < RegressionAccuracy.size(); ++ir)
    //		{
    //			/*for (int iu = 0; iu < UseSurrogates.size(); ++iu)
    //			{*/

    //				for (int ic = 0; ic < MaxCategories.size(); ++ic)
    //				{
    //					/*for (int icv = 0; icv < CalculateVarImportance.size(); ++icv)
    //					{*/
    //						for (int ia = 0; ia < ActiveVarCount.size(); ++ia)
    //						{
    //							for (int iCount = 0; iCount < Count.size(); ++iCount)
    //							{
    //								for (int iEps = 0; iEps < Eps.size(); ++iEps)
    //								{





    Ptr<RTrees> model;
    model = RTrees::create();
    model->setMaxDepth(MaxDepth);
    model->setMinSampleCount(MinSampleCount);
    model->setRegressionAccuracy(RegressionAccuracy);
    //model->setUseSurrogates(UseSurrogates[iu]);
    model->setUseSurrogates(false);
    //model->setMaxCategories(MaxCategories[ic]);
    model->setMaxCategories(MaxCategories);// для категориальных переменных с возможными значениями N> max_categories.
    try
    {
        model->setPriors(Mat());//Массив априорных вероятностей класса, отсортированный по значению метки класса.
    }
    catch (cv::Exception exc)
    {
        cout << exc.msg << endl;

    }

    //model->setPriors(Mat());
    model->setCalculateVarImportance(true);//

    model->setActiveVarCount(ActiveVarCount);//Размер случайно выбранного подмножества объектов в каждом узле дерева, которые используются для нахождения наилучшего разбиения. Если вы установите его в 0, то размер будет установлен в квадратный корень от общего числа объектов. Значение по умолчанию 0.
    model->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, Count, Eps)); // 1000 , 0,01f
    std::cout << endl;

    model->train(trainingDataMat, ROW_SAMPLE, labelsMat);
    cv::Mat res;// (testLabelsMat.rows, testLabelsMat.cols, CV_32F);


    float response = model->predict(testDataMat, res);



    int colNumber = testLabelsMat.size().height;
    float trueAnswer = 0;

    /*ofstream fout;
    fout.open("file.txt");*/


    /*QFile file("C:/Users/vaski_000/source/repos/OurTaskSolution/OurTaskSolution/file.txt");*/






    std::ofstream myfile;
    myfile.open("exampleRandomForest.csv");


    for (int i = 0; i < colNumber; i++)
    {
        if (res.at<float>(i, 0) == testLabelsMat.at<int>(i, 0))
        {
            //cout << true;
            trueAnswer++;
        }

        QString N = name1.value(i);//error
        QString N1 = 0;
        string N2 ;
        std::string current_locale_text = N.toLocal8Bit().constData();
        int m = res.at<float>(i, 0);
        int p = testLabelsMat.at<int>(i, 0);
        QString print = QString::number(p);//тест
        QString print2 = QString::number(m);//*ответ
        std::string print_str = print.toLocal8Bit().constData();
        std::string print2_str = print2.toLocal8Bit().constData();



        myfile << current_locale_text << "<" << print_str << "<" << print2_str << "\n";

    }


    myfile.close();


    excellent = trueAnswer / colNumber;



    if (excellent > MaxAccuracy)
    {
        MaxAccuracy = excellent;
        BestMaxDepth = MaxDepth;
        BestMinSampleCount = MinSampleCount;
        BestRegressionAccuracy = RegressionAccuracy;
        //BestUseSurrogates = UseSurrogates[iu];
        BestMaxCategories = MaxCategories;

        //BestCalculateVarImportance = CalculateVarImportance[icv];
        BestActiveVarCount = ActiveVarCount;
        BestCount = Count;
        BestEps = Eps;
    }



        std::cout << "MaxAccuracy " << MaxAccuracy << endl;
        std::cout << "BestMaxDepth  "<< BestMaxDepth << endl;
        std::cout << "BestMinSampleCount  " <<BestMinSampleCount << endl;
        std::cout<< "BestRegressionAccuracy " << BestRegressionAccuracy << endl;
        std::cout << "BestUseSurrogates " <<BestUseSurrogates << endl;
        std::cout << "BestMaxCategories " <<BestMaxCategories << endl;

        std::cout<< "BestCalculateVarImportance " << BestCalculateVarImportance << endl;
        std::cout << "BestActiveVarCount "  <<BestActiveVarCount << endl;
        std::cout<< "BestCount " << BestCount << endl;
        std::cout<<"BestEps " << BestEps << endl;




    }





void Widget::LGClassification(double Learning, double Iterator, LogisticRegression::RegKinds Reg, int  Size)
{

    QVector<cv::Mat> mat1;
    QVector<QString> name1;
    mat1 = CvServices::readAllFiles("C:/Users/user/Documents/TestViborka", 0.25f);
    name1 = CvServices::readAllFilesName("C:/Users/user/Documents/TestViborka", 0.25f);

    Mat trainingDataMat = mat1.at(0);
    Mat labelsMat = mat1.at(2);
    Mat testDataMat = mat1.at(1);
    Mat testLabelsMat = mat1.at(3);

    Mat labelsMatF;
    //Mat labelsMatF = labelsMat;
    Mat testLabelsMatF;
    //Mat testLabelsMatF = testLabelsMat;

    labelsMat.convertTo(labelsMatF, CV_32F);
    testLabelsMat.convertTo(testLabelsMatF, CV_32F);



    double excellent = 0;

    //QVector<double> LearningRate = { 0.0000001, /*0.000001, 0.00001, 0.0001, 0.001, 0.01,0.1,0.5,0.75,1,1.5,2,5 */};
    //QVector<double> Iterator = { 100, /*1000, 10000, 100000 */};
    //QVector<cv::ml::LogisticRegression::RegKinds> Reg = { cv::ml::LogisticRegression::REG_L2 /*, cv::ml::LogisticRegression::REG_L1 , cv::ml::LogisticRegression::REG_DISABLE*/ };
    //QVector<cv::ml::LogisticRegression::Methods> TrainMethod = { /*cv::ml::LogisticRegression::MINI_BATCH ,*/ cv::ml::LogisticRegression::BATCH };
    //QVector<int> Size = {/* 1,2,3,4,*/5/*,6,7,8,9 */};

    // 0.0000001
    //100
    //5

    double MaxLearningRate = 0;
    double MaxIterator = 0;
    cv::ml::LogisticRegression::RegKinds MaxReg;
    cv::ml::LogisticRegression::Methods MaxTrainMethod;
    int MaxSize = 0;
    double MaxAccuracy = 0;

    /*for (int il = 0; il < LearningRate.size(); ++il)
    {
        for (int ii = 0; ii < Iterator.size(); ++ii)
        {

            for (int ir = 0; ir < Reg.size(); ++ir)
            {

                for (int it = 0; it < TrainMethod.size(); ++it)
                {
                    for (int is = 0; is < Size.size(); ++is)
                    {
                        */


                        auto lr = LogisticRegression::create();
                        lr->setLearningRate(Learning);//скорость обучения
                        lr->setIterations(Iterator);//количество предпринятых шагов, этот и предыдущий параметры определяют, как быстро мы приходим к возможному решению.
                        lr->setRegularization(Reg); //Чтобы компенсировать переоснащение, выполняется регуляризация, которая может быть включена, тип регуляризации
                        lr->setTrainMethod(cv::ml::LogisticRegression::BATCH);
                        lr->setMiniBatchSize(Size);

                        //cv::Ptr<cv::ml::TrainData> tData = cv::ml::TrainData::create(trainingDataMat, cv::ml::SampleTypes::ROW_SAMPLE,
                    //		labelsMatF);

                            //lr->train(tData);
                        lr->train(trainingDataMat, cv::ml::SampleTypes::ROW_SAMPLE, labelsMatF);



                        cv::Mat res;// (testLabelsMat.rows, testLabelsMat.cols, CV_32F);


                        float response = lr->predict(testDataMat, res);

                        int colNumber = testLabelsMatF.size().height;//testLabelsMat
                        float trueAnswer = 0;
                        std::ofstream myfile;
                        myfile.open("exampleLR.csv");


                        for (int i = 0; i < colNumber; i++)
                        {
                            if (res.at<float>(i, 0) == testLabelsMat.at<int>(i, 0))
                            {
                                //cout << true;
                                trueAnswer++;
                            }
                            QString N = name1.value(i);//error
                            QString N1 = 0;
                            string N2;
                            std::string current_locale_text = N.toLocal8Bit().constData();
                            int m = res.at<float>(i, 0);
                            int p = testLabelsMat.at<int>(i, 0);
                            QString print = QString::number(p);//тест
                            QString print2 = QString::number(m);//*ответ
                            std::string print_str = print.toLocal8Bit().constData();
                            std::string print2_str = print2.toLocal8Bit().constData();



                            myfile << current_locale_text << "<" << print_str << "<" << print2_str << "\n";
                        }
                        myfile.close();

                        excellent = trueAnswer / colNumber;

                        cout << excellent << endl;;

                        if (excellent > MaxAccuracy)
                        {
                            MaxLearningRate = Learning;
                            MaxIterator = Iterator;
                            MaxReg = Reg;
                            //MaxTrainMethod = TrainMethod;
                            MaxSize = Size;
                        }




}

void Widget::SVMClassification(double C, qreal gamma, int pow, cv::ml::SVM::KernelTypes types)
{
    //std::cout << "Boom!";



    QVector<cv::Mat> mat1;
    QVector<QString> name1;
    mat1 = CvServices::readAllFiles("C:/Users/user/Documents/TestViborka", 0.25f);
    name1 = CvServices::readAllFilesName("C:/Users/user/Documents/TestViborka", 0.25f);
    //int labels[7] = { 1, 2, 3, 4, 5 , 6 , 7 };

    Mat trainingDataMat = mat1.at(0);
    Mat labelsMat = mat1.at(2);
    Mat testDataMat = mat1.at(1);
    Mat testLabelsMat = mat1.at(3);

    //auto td = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);

    //QVector<SVM::KernelTypes> types = { /*SVM::LINEAR, SVM::RBF, SVM::SIGMOID ,*/ SVM::POLY };

    cv::ml::SVM::KernelTypes types_value = types;
    //QVector<double> c_values = { /*.01, .05,  .1, .25, .5, .75, 1, 1.25, 2, 4,*/ 4.24, 4.249, 4.25,  4.251, 4.26, 4.5,/* 4.6, 4.75, 5, 5.56 , 7 , 8 , 9, 10,11,11.6,11.75,11.95,12,12.1 ,12.2, 12,35*/ };
    double c_values = C;
    //QVector<double> gamma_values = { /*.001, .005, .01, .05, .1, .15, .2,.21, .215, .219, .22, .221, .224, .225 , .226, .24,*/.249, .25, .251/*.26, .27, .28, .29, .30, .5, 1, 2, 5, 10*/ };
    //QVector<int> pow_values = {2};
    double gamma_values = gamma;
    int pow_value = pow;


    double maxAccuracy = 0;
    string Type;
    double Value = 0;
    double Gamma = 0;
    int power = 0;
    /*for (int it = 0; it < types.size(); ++it)
    {*/
        /*for (int ic = 0; ic < c_values.size(); ++ic)
        {*/
            if (types != SVM::LINEAR)

                if (types == SVM::POLY)
                {
                    double excellent = trainAndPredict(types_value,  gamma_values, c_values, pow_value, trainingDataMat, labelsMat, testDataMat, testLabelsMat);
                    std::cout << excellent << endl;
                    if (excellent > maxAccuracy)
                    {
                        maxAccuracy = excellent;
                        Type = "POLY";
                        Value = c_values;
                        Gamma = gamma_values;
                        power = pow_value;
                    }
                }
                    else
                    {
                        double excellent = trainAndPredict(types_value,  gamma_values, c_values, pow_value,  trainingDataMat, labelsMat, testDataMat, testLabelsMat);
                        std::cout << excellent << endl;
                        if (excellent > maxAccuracy)
                        {
                            maxAccuracy = excellent;

                            Type = (types == SVM::RBF ? "RBF"  : "Sigmoid");
                            Value = c_values;
                            Gamma = gamma_values;
                        }
                    }

            else
            {
                double excellent = trainAndPredict(types_value,  gamma_values,  C,  pow_value,  trainingDataMat, labelsMat, testDataMat, testLabelsMat);
                std::cout << excellent << endl;
                if (excellent > maxAccuracy)
                {
                    maxAccuracy = excellent;

                    Type = "LINEAR";
                    Value = c_values;
                }
            }




    cout << "Max Accuracy: " << maxAccuracy << endl;
    cout << "Type: " << Type << endl;
    cout << "C:  " << Value << endl;
    cout << "Gamma: " << Gamma << endl;
    cout << "Power: " << power << endl;



    //svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1e7, 1e-6));




}

double Widget::trainAndPredict(cv::ml::SVM::KernelTypes types_value,  qreal& gamma_value, double& c_values, int pow_values,  cv::Mat& trainingDataMat, cv::Mat& labelsMat, cv::Mat& testDataMat, cv::Mat& testLabelsMat)
{
    double excellent = 0;

    QVector<QString> name1;


    name1 = CvServices::readAllFilesName("C:/Users/user/Documents/TestViborka", 0.25f);

    auto svm = SVM::create();
    svm->setType(SVM::C_SVC);
    svm->setKernel(types_value);
    svm->setGamma(gamma_value);
    svm->setC(c_values);
    svm->setDegree(pow_values);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1e4, 1e-5));//1e4,COUNT,

    try
    {
        svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);
    }
    catch (cv::Exception exc)
    {
        cout << exc.msg << endl;
        return 0;
    }

    try
    {
        cv::Mat res;// (testLabelsMat.rows, testLabelsMat.cols, CV_32F);


        float response = svm->predict(testDataMat, res);

        int colNumber = testLabelsMat.size().height;
        float trueAnswer = 0;


            std::ofstream myfile;
            myfile.open("exampleSVM.csv");

        for (int i = 0; i < colNumber; i++)
        {

            if (res.at<float>(i, 0) == testLabelsMat.at<int>(i, 0))
            {
                //cout << true;
                trueAnswer++;
            }



                QString N = name1.value(i);//error
                QString N1 = 0;
                string N2;
                std::string current_locale_text = N.toLocal8Bit().constData();
                int m = res.at<float>(i, 0);
                int p = testLabelsMat.at<int>(i, 0);
                QString print = QString::number(p);//тест
                QString print2 = QString::number(m);//*ответ
                std::string print_str = print.toLocal8Bit().constData();
                std::string print2_str = print2.toLocal8Bit().constData();



                myfile << current_locale_text << "<" << print_str << "<" << print2_str << "\n";





        }
        myfile.close();
        excellent = trueAnswer / colNumber;

        return excellent;
    }
    catch (cv::Exception exc)
    {
        cout << exc.msg << endl;
        return 0;
    }

}


    void Widget::KNearestClassify(int DefaultN,int AlgoritmTipe,int Emax)
    {
        QVector<cv::Mat> mat1;
        mat1 = CvServices::readAllFiles("C:/Users/user/Documents/TestViborka", 0.25f);
        QVector<QString> name1;
        name1 = CvServices::readAllFilesName("C:/Users/user/Documents/TestViborka", 0.25f);

        Mat trainingDataMat = mat1.at(0);
        Mat labelsMat = mat1.at(2);
        Mat testDataMat = mat1.at(1);
        Mat testLabelsMat = mat1.at(3);
        double excellent = 0;

        Mat labelsMatF;

        Mat testLabelsMatF;
        int BestN = 0;
        int BestAlgoritm = 0;
        int BestEmax = 0;
        double maxAccuracy = 0;

        //labelsMat.convertTo(labelsMatF, CV_32F);
        //testLabelsMat.convertTo(testLabelsMatF, CV_32F);
         excellent = 0;

        //QVector<int> DefaultN = { 1, 2, 3 ,4 ,5 ,6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 ,14 , 15 }; // 9//14
        //QVector<int> AlgoritmTipe = { ml::KNearest::Types::BRUTE_FORCE, ml::KNearest::Types::KDTREE };//1, 2, 3 ,4 ,5 ,6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 ,14 , 15 }; // 1
        //QVector<int> Emax = {0, 1, 2, 3 ,4 ,5 ,6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 ,14 , 15 };//1




        //for (int i = 0; i < DefaultN.size(); i++)
        //{
        //	for (int ia = 0; ia < AlgoritmTipe.size(); ia++)
        //	{
        //		for (int ie = 0; ie < Emax.size(); ie++)
        //		{


         //if (AlgoritmTipe == 0)//ml::KNearest::Types::BRUTE_FORCE, ml::KNearest::Types::KDTREE)
            //auto AlgoritmTipe1 = ml::KNearest::Types::BRUTE_FORCE;

         //if (AlgoritmTipe == 1)//ml::KNearest::Types::BRUTE_FORCE, ml::KNearest::Types::KDTREE)
            //auto AlgoritmTipe1 = ml::KNearest::Types::KDTREE;

                    Ptr<KNearest> model = KNearest::create();
                    model->setDefaultK(DefaultN);
                    model->setAlgorithmType(AlgoritmTipe);
                    model->setEmax(Emax);
                    model->setIsClassifier(true);
                    model->train(trainingDataMat, ROW_SAMPLE, labelsMat);//labelsMatF);

                    cv::Mat res;// (testLabelsMat.rows, testLabelsMat.cols, CV_32F);


                    float response = model->predict(testDataMat, res);

                    int colNumber = testLabelsMat.size().height;//testLabelsMatF.size().height;
                    float trueAnswer = 0;

                    std::ofstream myfile;
                    myfile.open("exampleKnearest.csv");

                    for (int i = 0; i < colNumber; i++)
                    {


                        QString N = name1.value(i);//error
                        QString N1 = 0;
                        string N2;
                        std::string current_locale_text = N.toLocal8Bit().constData();
                        int m = res.at<float>(i, 0);
                        int p = testLabelsMat.at<int>(i, 0);
                        QString print = QString::number(p);//тест
                        QString print2 = QString::number(m);//*ответ
                        std::string print_str = print.toLocal8Bit().constData();
                        std::string print2_str = print2.toLocal8Bit().constData();



                        myfile << current_locale_text << "<" << print_str << "<" << print2_str << "\n";

                        //cout << res.at<float>(i, 0) << endl;
                        //cout << testLabelsMatF.at<int>(i, 0) << endl;
                        //cout << testLabelsMat.at<int>(i, 0) << endl;



                        if (res.at<float>(i, 0) == testLabelsMat.at<int>(i, 0))//testLabelsMatF.at<float>(i, 0))
                        {
                            //cout << true;
                            trueAnswer++;
                        }

                    }

                    myfile.close();

                    excellent = trueAnswer / colNumber;

                    cout << excellent << endl;;


                    if (excellent > maxAccuracy)
                    {
                         BestN = DefaultN;
                         BestAlgoritm = AlgoritmTipe;
                         BestEmax = Emax;
                        maxAccuracy = excellent;

                    }


        std::cout << "BestN" << BestN << endl;
        std::cout <<" BestAlgoritm " << BestAlgoritm <<endl;
        std::cout << "Emax " << BestEmax << endl;
        std::cout << "maxAccuracy " << maxAccuracy << endl;


    }



    void Widget::MLPClassification()
    {
        const int class_count = 7;
        //Mat data;
        //Mat responses;
        QVector<QString> name1;
        auto mat1 = CvServices::readAllFiles("C:/Users/user/Documents/TestViborka", 0.25f);
        name1 = CvServices::readAllFilesName("C:/Users/user/Documents/TestViborka", 0.25f);

        Mat train_data = mat1.at(0);
        Mat labelsMat = mat1.at(2);
        Mat testDataMat = mat1.at(1);
        Mat testLabelsMat = mat1.at(3);

        //bool ok = read_num_class_data(data_filename, 16, &data, &responses);
        //if (!ok)
        //	return ok;

        Ptr<ANN_MLP> model;

        //int nsamples_all = data.rows;
        int ntrain_samples = labelsMat.rows;//(int)(nsamples_all * 0.8);

        // Create or load MLP classifier
        //if (!filename_to_load.empty())
        //{
        //	model = load_classifier<ANN_MLP>(filename_to_load);
        //	if (model.empty())
        //		return false;
        //	ntrain_samples = 0;
        //}
        //else
        //{
            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            //
            //MLP не поддерживает категориальные переменные явно.
 /*Итак, вместо метки выходного класса мы будем использовать
 двоичный вектор компонентов <class_count> для обучения и,
 следовательно, MLP даст нам вектор "вероятностей" на
 этап прогнозирования*/

            //
            // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

            //Mat train_data = data.rowRange(0, ntrain_samples);
            Mat train_responses = Mat::zeros(ntrain_samples, class_count, CV_32F);//  двоичная матрица двоичных векторов

            // 1. unroll the responses
            cout << "Unrolling the responses...\n";
            for (int i = 0; i < ntrain_samples; i++)
            {
                int cls_label = labelsMat.at<int>(i);
                train_responses.at<float>(i, cls_label) = 1.f;
            }

            // 2. train classifier
            int layer_sz[] = { train_data.cols, 100,100, 100, class_count };// кол - во слоёв 4 ( параметры , 100 нейронов , 100 нейронов , кол - во классов)
            int nlayers = (int)(sizeof(layer_sz) / sizeof(layer_sz[0]));//1,2,3,4,
            Mat layer_sizes(1, nlayers, CV_32S, layer_sz);

//#if 1
            int method = ANN_MLP::BACKPROP;
            double method_param = 0.001;
            int max_iter = 300;
//#else
            /*int method = ANN_MLP::RPROP;
            double method_param = 0.1;
            int max_iter = 1000;*/
//#endif



            Ptr<TrainData> tdata = TrainData::create(train_data, ROW_SAMPLE, train_responses);

            cout << "Training the classifier (may take a few minutes)...\n";
            model = ANN_MLP::create();
            model->setLayerSizes(layer_sizes);
            model->setActivationFunction(ANN_MLP::SIGMOID_SYM, 0, 0);
            model->setTermCriteria(TC(max_iter, 0));
            model->setTrainMethod(method, method_param);
            model->train(tdata);
            cout << endl;
        //}

        //test_and_save_classifier(model, data, responses, ntrain_samples, 'A', filename_to_save);

        //int i, nsamples_all = train_data.rows + testDataMat.rows;

            //--------------
            cv::Mat res;// (testLabelsMat.rows, testLabelsMat.cols, CV_32F);
            float response = model->predict(testDataMat, res);
            int colNumber = testLabelsMat.size().height;//testLabelsMatF.size().height;
            float trueAnswer = 0;
            for (int i = 0; i < colNumber; i++)
            {
                cout << res.at<int>(i, 0) << endl;
                //cout << testLabelsMatF.at<int>(i, 0) << endl;
                cout << testLabelsMat.at<int>(i, 0) << endl;

                if (res.at<int>(i, 0) == testLabelsMat.at<int>(i, 0))//testLabelsMatF.at<float>(i, 0))
                {
                    //cout << true;
                    trueAnswer++;
                }
            }

            //-------------------
        double train_hr = 0, test_hr = 0;

        // compute prediction error on train data
        for (int i = 0; i < train_data.rows; i++)
        {
            Mat sample = train_data.row(i);

            float r = model->predict(sample);
            r = std::abs(r - labelsMat.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;

            //if (i < ntrain_samples)
                train_hr += r;
            //else
            //	test_hr += r;
        }
        // compute prediction error on test data
        for (int i = 0; i < testDataMat.rows; i++)
        {
            Mat sample = testDataMat.row(i);

            float r = model->predict(sample);
            r = std::abs(r - testLabelsMat.at<int>(i)) <= FLT_EPSILON ? 1.f : 0.f;

            //if (i < ntrain_samples)
            //train_hr += r;
            //else
            test_hr += r;
        }


        test_hr /= testDataMat.rows;//nsamples_all - ntrain_samples;
        train_hr = ntrain_samples > 0 ? train_hr / ntrain_samples : 1.;

        printf("Recognition rate: train = %.1f%%, test = %.1f%%\n",
            train_hr * 100., test_hr * 100.);

        /*if (!filename_to_save.empty())
        {
            model->save(filename_to_save);
        }

        return true;*/
    }




    /////////////////////////-----------------------------------------------------------------------------------------------------------------------------------------
    /////////////////////////-----------------------------------------------------------------------------------------------------------------------------------------
    /////////////////////////-----------------------------------------------------------------------------------------------------------------------------------------
    /////////////////////////-----------------------------------------------------------------------------------------------------------------------------------------
    /////////////////////////-----------------------------------------------------------------------------------------------------------------------------------------

    void Widget::SVMClassification1()
    {
        //std::cout << "Boom!";



        QVector<cv::Mat> mat1;
        mat1 = CvServices::readAllFiles("C:/Users/user/Documents/TestViborka", 0.25f);

        //int labels[7] = { 1, 2, 3, 4, 5 , 6 , 7 };

        Mat trainingDataMat = mat1.at(0);
        Mat labelsMat = mat1.at(2);
        Mat testDataMat = mat1.at(1);
        Mat testLabelsMat = mat1.at(3);

        //auto td = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);

        QVector<SVM::KernelTypes> types = { SVM::LINEAR, SVM::RBF, SVM::SIGMOID , SVM::POLY };
        QVector<double> c_values = { .01,.02,.03, .04, .05, .06, .065, .1, .25, .5, .75, 1, 1.25, 2, 4, 4.24, 4.249, 4.25,  4.251, 4.26, 4.5, 4.6, 4.75, 5,  };
        QVector<double> gamma_values = { .00001,.0007,.0008,.00095, .0009, .001, .005, .01, .05, .1, .15, };
        QVector<int> pow_values = { 1/*,2,3,4*/ };

        double maxAccuracy = 0;
        string Type;
        double Value = 0;
        double Gamma = 0;
        int power = 0;
        for (int it = 0; it < types.size(); ++it)
        {
            for (int ic = 0; ic < c_values.size(); ++ic)
            {
                if (types[it] != SVM::LINEAR)
                    for (int ig = 0; ig < gamma_values.size(); ++ig)
                    {
                        if (types[it] == SVM::POLY)
                            for (int ip = 0; ip < pow_values.size(); ++ip)
                            {
                                double excellent = trainAndPredict1(types, it, gamma_values, ig, c_values, ic, pow_values, ip, trainingDataMat, labelsMat, testDataMat, testLabelsMat);
                                std::cout << excellent << endl;
                                if (excellent > maxAccuracy)
                                {
                                    maxAccuracy = excellent;
                                    Type = "POLY";
                                    Value = c_values[ic];
                                    Gamma = gamma_values[ig];
                                    power = pow_values[ip];
                                }
                            }
                        else
                        {
                            double excellent = trainAndPredict1(types, it, gamma_values, ig, c_values, ic, pow_values, 0, trainingDataMat, labelsMat, testDataMat, testLabelsMat);
                            std::cout << excellent << endl;
                            if (excellent > maxAccuracy)
                            {
                                maxAccuracy = excellent;

                                Type = (types[it] == SVM::RBF ? "RBF" : "Sigmoid");
                                Value = c_values[ic];
                                Gamma = gamma_values[ig];
                            }
                        }
                    }
                else
                {
                    double excellent = trainAndPredict1(types, it, gamma_values, 0, c_values, ic, pow_values, 0, trainingDataMat, labelsMat, testDataMat, testLabelsMat);
                    std::cout << excellent << endl;
                    if (excellent > maxAccuracy)
                    {
                        maxAccuracy = excellent;

                        Type = "LINEAR";
                        Value = c_values[ic];
                    }
                }

            }
        }

        cout << "Max Accuracy: " << maxAccuracy << endl;
        cout << "Type: " << Type << endl;
        cout << "C:  " << Value << endl;
        cout << "Gamma: " << Gamma << endl;
        cout << "Power: " << power << endl;



        //svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1e7, 1e-6));




    }

    double Widget::trainAndPredict1(QVector<cv::ml::SVM::KernelTypes>& types, int it, QVector<qreal>& gamma_values, int ig, QVector<qreal>& c_values, int ic, QVector<int>& pow_values, int ip, cv::Mat& trainingDataMat, cv::Mat& labelsMat, cv::Mat& testDataMat, cv::Mat& testLabelsMat)
    {
        double excellent = 0;

        auto svm = SVM::create();
        svm->setType(SVM::C_SVC);
        svm->setKernel(types[it]);
        svm->setGamma(gamma_values[ig]);
        svm->setC(c_values[ic]);
        svm->setDegree(pow_values[ip]);
        svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 1e2, 1e-1));//1e4,COUNT,

        try
        {
            svm->train(trainingDataMat, ROW_SAMPLE, labelsMat);
        }
        catch (cv::Exception exc)
        {
            cout << exc.msg << endl;
            return 0;
        }

        try
        {
            cv::Mat res;// (testLabelsMat.rows, testLabelsMat.cols, CV_32F);


            float response = svm->predict(testDataMat, res);

            int colNumber = testLabelsMat.size().height;
            float trueAnswer = 0;
            for (int i = 0; i < colNumber; i++)
            {
                //cout << res.at<float>(i, 0) << endl;
                //cout << testLabelsMat.at<int>(i, 0) << endl;

                if (res.at<float>(i, 0) == testLabelsMat.at<int>(i, 0))
                {
                    //cout << true;
                    trueAnswer++;
                }
            }

            excellent = trueAnswer / colNumber;

            return excellent;
        }
        catch (cv::Exception exc)
        {
            cout << exc.msg << endl;
            return 0;
        }

    }


    void Widget::KNearestClassify1()
    {
        QVector<cv::Mat> mat1;
        mat1 = CvServices::readAllFiles("C:/Users/user/Documents/TestViborka", 0.25f);
        QVector<QString> name1;
        name1 = CvServices::readAllFilesName("C:/Users/user/Documents/TestViborka", 0.25f);


        Mat trainingDataMat = mat1.at(0);
        Mat labelsMat = mat1.at(2);
        Mat testDataMat = mat1.at(1);
        Mat testLabelsMat = mat1.at(3);
        double excellent = 0;

        Mat labelsMatF;

        Mat testLabelsMatF;
        int BestN = 0;
        int BestAlgoritm = 0;
        int BestEmax = 0;
        double maxAccuracy = 0;

        //labelsMat.convertTo(labelsMatF, CV_32F);
        //testLabelsMat.convertTo(testLabelsMatF, CV_32F);
        excellent = 0;

        QVector<int> DefaultN = { 1, 2, 3 ,4 ,5 ,6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 ,14 , 15 }; // 9
        QVector<int> AlgoritmTipe = { ml::KNearest::Types::BRUTE_FORCE, ml::KNearest::Types::KDTREE };//1, 2, 3 ,4 ,5 ,6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 ,14 , 15 }; // 1
        QVector<int> Emax = { 0, 1, 2, 3 ,4 ,5 ,6 , 7 , 8 , 9 , 10 , 11 , 12 , 13 ,14 , 15 };//1



        for (int i = 0; i < DefaultN.size(); i++)
        {
            for (int ia = 0; ia < AlgoritmTipe.size(); ia++)
            {
                for (int ie = 0; ie < Emax.size(); ie++)
                {


                    Ptr<KNearest> model = KNearest::create();
                    model->setDefaultK(DefaultN[i]);
                    model->setAlgorithmType(AlgoritmTipe[ia]);
                    model->setEmax(Emax[ie]);
                    model->setIsClassifier(true);
                    model->train(trainingDataMat, ROW_SAMPLE, labelsMat);//labelsMatF);

                    cv::Mat res;// (testLabelsMat.rows, testLabelsMat.cols, CV_32F);


                    float response = model->predict(testDataMat, res);

                    int colNumber = testLabelsMat.size().height;//testLabelsMatF.size().height;
                    float trueAnswer = 0;
                    for (int i = 0; i < colNumber; i++)
                    {
                        cout << res.at<float>(i, 0) << endl;
                        //cout << testLabelsMatF.at<int>(i, 0) << endl;
                        cout << testLabelsMat.at<int>(i, 0) << endl;

                        if (res.at<float>(i, 0) == testLabelsMat.at<int>(i, 0))//testLabelsMatF.at<float>(i, 0))
                        {
                            //cout << true;
                            trueAnswer++;
                        }
                    }

                    excellent = trueAnswer / colNumber;

                    cout << excellent << endl;;


                    if (excellent > maxAccuracy)
                    {
                        BestN = DefaultN[i];
                        BestAlgoritm = AlgoritmTipe[ia];
                        BestEmax = Emax[ie];
                        maxAccuracy = excellent;

                    }
                }
            }
        }

        std::cout << "BestN" << BestN << endl;
        std::cout << " BestAlgoritm " << BestAlgoritm << endl;
        std::cout << "Emax " << BestEmax << endl;
        std::cout << "maxAccuracy " << maxAccuracy << endl;


    }

    void Widget::RandomForestClassification1()
    {

        QVector<cv::Mat> mat1;
        mat1 = CvServices::readAllFiles("C:/Users/user/Documents/TestViborka", 0.25f);
        //QVector<String> names = CvServices::readMatNames();

        Mat trainingDataMat = mat1.at(0);
        Mat labelsMat = mat1.at(2);
        Mat testDataMat = mat1.at(1);
        Mat testLabelsMat = mat1.at(3);

        double excellent = 0;

        double MaxAccuracy = 0;
        QVector<int> MaxDepth = {10 };
        QVector<int> MinSampleCount = { 8, 9,10 };
        QVector<int> RegressionAccuracy = { 0,/*1,2,3 */};
        QVector<int> MaxCategories = {/*7,5,6,*/7/*,8,9, 10, 11,15,20,30*/ };// для категориальных переменных с возможными значениями N> max_categories.
        QVector<bool > CalculateVarImportance = { true , false };
        QVector<int> ActiveVarCount = { 1,2,3,4,5,6,7  };//Размер случайно выбранного подмножества объектов в каждом узле дерева, которые используются для нахождения наилучшего разбиения. Если вы установите его в 0, то размер будет установлен в квадратный корень от общего числа объектов. Значение по умолчанию 0.
        QVector<int> Count = { 35,36,/*37,38,39,40,41,42,43,44,*/45,46,47 };//100
        QVector<float> Eps = { 0.0073, 0.0083, 0.008,/* 0.0083, 0.0085,0.0087, 0.001, 0.000,1  */};//0.009


        int BestMaxDepth = 0;
        int BestMinSampleCount = 0;
        int BestRegressionAccuracy = 0;
        bool BestUseSurrogates = false;
        int BestMaxCategories = 0;
        //QVector<int> Priors(Mat());
        bool BestCalculateVarImportance = true;
        int BestActiveVarCount = 0;
        int BestCount = 0;
        float BestEps = 0;



        for (int id = 0; id < MaxDepth.size(); ++id)
        {
            for (int is = 0; is < MinSampleCount.size(); ++is)
            {
                for (int ir = 0; ir < RegressionAccuracy.size(); ++ir)
                {
                    /*for (int iu = 0; iu < UseSurrogates.size(); ++iu)
                    {*/

                    for (int ic = 0; ic < MaxCategories.size(); ++ic)
                    {
                        /*for (int icv = 0; icv < CalculateVarImportance.size(); ++icv)
                        {*/
                        for (int ia = 0; ia < ActiveVarCount.size(); ++ia)
                        {
                            for (int iCount = 0; iCount < Count.size(); ++iCount)
                            {
                                for (int iEps = 0; iEps < Eps.size(); ++iEps)
                                {





                                    Ptr<RTrees> model;
                                    model = RTrees::create();
                                    model->setMaxDepth(MaxDepth[id]);
                                    model->setMinSampleCount(MinSampleCount[is]);
                                    model->setRegressionAccuracy(RegressionAccuracy[ir]);
                                    //model->setUseSurrogates(UseSurrogates[iu]);
                                    model->setUseSurrogates(false);
                                    //model->setMaxCategories(MaxCategories[ic]);
                                    model->setMaxCategories(MaxCategories[ic]);// для категориальных переменных с возможными значениями N> max_categories.
                                    try
                                    {
                                        model->setPriors(Mat());//Массив априорных вероятностей класса, отсортированный по значению метки класса.
                                    }
                                    catch (cv::Exception exc)
                                    {
                                        cout << exc.msg << endl;

                                    }

                                    //model->setPriors(Mat());
                                    model->setCalculateVarImportance(true);//

                                    model->setActiveVarCount(ActiveVarCount[ia]);//Размер случайно выбранного подмножества объектов в каждом узле дерева, которые используются для нахождения наилучшего разбиения. Если вы установите его в 0, то размер будет установлен в квадратный корень от общего числа объектов. Значение по умолчанию 0.
                                    model->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, Count[iCount], Eps[iEps])); // 1000 , 0,01f
                                    std::cout << endl;

                                    model->train(trainingDataMat, ROW_SAMPLE, labelsMat);
                                    cv::Mat res;// (testLabelsMat.rows, testLabelsMat.cols, CV_32F);


                                    float response = model->predict(testDataMat, res);

                                    int colNumber = testLabelsMat.size().height;
                                    float trueAnswer = 0;
                                    for (int i = 0; i < colNumber; i++)
                                    {
                                        /*std::vector < string > stdvector = names.toStdVector();
                                        std::cout << stdvector[i] <<*/
                                        //std::cout << CvServices::names[i] << endl;
                                        //std::cout << res.at<float>(i, 0) << endl;
                                        //std::cout << testLabelsMat.at<int>(i, 0) << endl;

                                        if (res.at<float>(i, 0) == testLabelsMat.at<int>(i, 0))
                                        {
                                            //cout << true;
                                            trueAnswer++;
                                        }
                                    }

                                    excellent = trueAnswer / colNumber;
                                    std::cout << excellent << endl;;


                                    if (excellent > MaxAccuracy)
                                    {
                                        MaxAccuracy = excellent;
                                        BestMaxDepth = MaxDepth[id];
                                        BestMinSampleCount = MinSampleCount[is];
                                        BestRegressionAccuracy = RegressionAccuracy[ir];
                                        //BestUseSurrogates = UseSurrogates[iu];
                                        BestMaxCategories = MaxCategories[ic];

                                        //BestCalculateVarImportance = CalculateVarImportance[icv];
                                        BestActiveVarCount = ActiveVarCount[ia];
                                        BestCount = Count[iCount];
                                        BestEps = Eps[iEps];
                                    }


                                }
                            }


                        }
                    }
                }
            }
        }
        std::cout << "MaxAccuracy " << MaxAccuracy << endl;
        std::cout << "BestMaxDepth  " << BestMaxDepth << endl;
        std::cout << "BestMinSampleCount  " << BestMinSampleCount << endl;
        std::cout << "BestRegressionAccuracy " << BestRegressionAccuracy << endl;
        std::cout << "BestUseSurrogates " << BestUseSurrogates << endl;
        std::cout << "BestMaxCategories " << BestMaxCategories << endl;

        std::cout << "BestCalculateVarImportance " << BestCalculateVarImportance << endl;
        std::cout << "BestActiveVarCount " << BestActiveVarCount << endl;
        std::cout << "BestCount " << BestCount << endl;
        std::cout << "BestEps " << BestEps << endl;




    }
