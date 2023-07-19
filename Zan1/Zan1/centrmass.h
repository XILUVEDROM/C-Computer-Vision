#ifndef CENTRMASS_H
#define CENTRMASS_H
#include <QImage>
#include <math.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
class CentrMass
{
public:
    CentrMass();

    cv::Point2f Simmetr(IplImage s3);

    QImage*  IplImage2QImage(IplImage *iplImg);

};


#endif CENTRMASS_H
