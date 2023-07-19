#ifndef FEATURES_H
#define FEATURES_H

#include <QDebug>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
/*!
 * \brief Класс для расчета характеристик сегмента
 */

class Features
{
public:
    Features();
    void calculate(cv::Mat &segment, std::vector<int> &features);
};

#endif // FEATURES_H
