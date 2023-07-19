#include "features.h"

using namespace cv;

Features::Features()
{

}

/*!
 * \brief Функция расчета характеристик
 * \param [in] segment - сегмент линии на черном фоне
 * \param [out] features - вектор с расчитанными характеристиками
 */
void Features::calculate(cv::Mat &segment, std::vector<int> &features)
{
    int count=0, r=0, g=0, b=0;
    int minR=255, meanR=0, maxR=0;
    int minG=255, meanG=0, maxG=0;
    int minB=255, meanB=0, maxB=0;
    for (int i = 0; i < segment.rows; ++i)
        for (int j = 0; j < segment.cols; ++j)
            if (segment.at<Vec3b>(i,j) != Vec3b(0,0,0))
            {
                ++count;
                r = segment.at<Vec3b>(i,j)[2];
                g = segment.at<Vec3b>(i,j)[1];
                b = segment.at<Vec3b>(i,j)[0];
                minR = std::min(minR, r);
                meanR += r;
                maxR = std::max(maxR, r);
                minG = std::min(minG, g);
                meanG += g;
                maxG = std::max(maxG, g);
                minB = std::min(minB, b);
                meanB += b;
                maxB = std::max(maxB, b);
            }
    if (count != 0)
    {
        meanR /= count;
        meanG /= count;
        meanB /= count;
    }
    cvtColor(segment, segment, CV_BGR2GRAY);
    std::vector<std::vector<Point>> cont;
    findContours(segment, cont, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    RotatedRect rect = minAreaRect(cont[0]);
    int angle = int(rect.angle);
    int length = std::max(int(rect.size.width), int(rect.size.height));
    int thickness = std::min(int(rect.size.width), int(rect.size.height));
    features.clear();
    features.push_back(length);
    features.push_back(thickness);
    features.push_back(angle);
    features.push_back(minR);
    features.push_back(meanR);
    features.push_back(maxR);
    features.push_back(minG);
    features.push_back(meanG);
    features.push_back(maxG);
    features.push_back(minB);
    features.push_back(meanB);
    features.push_back(maxB);
}
