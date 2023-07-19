#pragma once
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/ml.hpp>
#include <QtCore/QString>
#include <QtCore/QVector>
#

class CvServices
{
    static QVector<QString> labelNames;
    //static QVector<QString> names;
     //std::vector< string > stdvector;

public:
    static QVector<cv::Mat> readMat(QString filename, float testSize = .25f);
    static QVector<cv::Mat> createLabelsMat(int label, int size, float testSize = .25f);
    static QVector<QString> readName(QString filename, float testSize = .25f);

    static QVector<cv::Mat> readMatWithLabels(QString filename, int label, float testSize = .25f);
    static QVector<cv::Mat> readAllFiles(QString path, float testSize = .25f);
    static QVector<QString> readAllFilesName(QString path, float testSize = .25f);



    static QString getLabelName(int label);

};
