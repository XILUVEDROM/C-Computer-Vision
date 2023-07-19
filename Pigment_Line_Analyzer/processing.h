#ifndef PROCESSING_H
#define PROCESSING_H

#include <QDebug>
#include <QPoint>
#include <deque>
#include <vector>
#include <utility>
#include "imagematrix.h"

/*!
 * \brief Содержит функции для обработки изображения
 * \todo Переделать из class в namespace?
 */
class Processing
{
public:
    Processing();
    void bilateralFilter(ImageMatrix &matrix, int radius, int sigma_d, int sigma_i);
    void otsuThreshold(ImageMatrix &matrix, int &threshold);
    void threshold(ImageMatrix &matrix, int threshold);
    void erosion(ImageMatrix &matrix, int radius);
    void dilatation(ImageMatrix &matrix, int radius);    
    void matchedFiler(ImageMatrix &matrix);
    void removeSmallOnMask(ImageMatrix &matrix, ImageMatrix &skelet, int min_size);
    void removeSmallOnSkelet(ImageMatrix &skelet, int max_size);    
    void removeCross(ImageMatrix &matrix);
    void cutOrigin(QImage &orig, QImage &mask);
    void laplace(ImageMatrix &matrix);

private:
    bool hasMoreThanTwoNeig(ImageMatrix &matrix, int x, int y, int r);
    bool hasBlackNeig(ImageMatrix &matrix, int x, int y, int r);
    void setNeigBlack(ImageMatrix &matrix, int x, int y, int r);
    void setProgress();
};

#endif // PROCESSING_H
