#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <algorithm>
#include <map>
#include <tuple>
#include <utility>
#include <cmath>
#include <ostream>
#include <string>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QFileInfo>

#include "connectedcomponents.h"

enum Filters { gauss = 0, median, box };

QImage getNevusSegment(QString fileName, bool is_fitted = false);
QImage getNevusToAnalize(QString fileName, bool is_fitted = false);
QImage MatToQImage(const cv::Mat &image);
bool fourConnectivity(int x, int y, const std::vector<std::vector<QColor> > &img);
QImage areaSelection(QString fileName, int treshold = 70, int maxTresholdValue = 96,
                     int minSquare = -1, int maxSquare = -1);
QImage pointSelection(QString fileName, Filters filter = gauss, int filterSize = 3,
                      int coreGauss_x = 2, int coreGauss_y = 2,
                      int minDistance = 10, int treshold = 45,
                      int tresholdStorage = 15 , int minRadius = 0, int maxRadius = 10, int color = 0, int minTh = 0, int maxTh = 10);
QImage npointSelection(QString fileName, Filters filter = gauss, int filterSize = 3,
                      int coreGauss_x = 2, int coreGauss_y = 2,
                      int minDistance = 10, int treshold = 45,
                      int tresholdStorage = 15 , int minRadius = 0, int maxRadius = 30);
QImage darkAreaSelection(QString fileName, int treshold = 70, int maxTresholdValue = 96, int minSquare = -1);
QImage objectSegmentation(QString fileName, Filters filter = gauss, int filterSize = 3, int lowThreshold = 0, int kernel_size = 5, int ratio = 3);
QImage getImgBorders(QString fileName);
QImage openCVdetectLines(QString fileName, int min_otv, int min_lin);
QImage openCVdetectGlobul(QString fileName);

#endif // ALGORITHMS_H
