#include "algorithms.h"

#include <QDebug>
#include <QtMath>
using namespace cv;
using namespace std;

QImage getImgBorders(QString fileName)
{
    Mat3b bgr_img(imread(fileName.toStdString()));
    Mat src(bgr_img.size(), bgr_img.type()), mask(bgr_img.size(), bgr_img.type());
    cvtColor(bgr_img, src, COLOR_BGR2GRAY);
    threshold(src, mask, 0 , 255, THRESH_BINARY_INV | THRESH_OTSU);
    ConnectedComponents cc(mask);
    cc.drawBorders(bgr_img, 5);
    return MatToQImage(bgr_img);
}



QImage getNevusSegment(QString fileName, bool is_fitted)
{
    Mat3b bgr_img(imread(fileName.toStdString()));
    Mat src(bgr_img.size(), bgr_img.type()), mask(bgr_img.size(), bgr_img.type());
    cvtColor(bgr_img, src, COLOR_BGR2GRAY);
    threshold(src, mask, 0 ,255, THRESH_BINARY_INV | THRESH_OTSU);
    ConnectedComponents cc(mask);
    cv::Rect roi(src.cols / 3, src.rows / 3, src.cols / 3, src.rows / 3);
    std::map<int, int> areas = cc.areas(roi);
    if (areas.size() == 1)
        areas = cc.areas();
    int nevus_label = -1;
    int nevus_area = -1;
    for (auto label_and_area: areas) {
        int label = label_and_area.first;
        int area = label_and_area.second;
        if (label == 0)
            continue; //пропуск сегмента фона;
        if (nevus_area < area) {
            nevus_label = label;
            nevus_area = area;
        }
    }
    assert(nevus_label != -1);
    if (is_fitted)
        return MatToQImage(cc.fittedMask(nevus_label)(bgr_img));
    return MatToQImage(cc.simpleMask(nevus_label)(bgr_img));
}


void getNevusSegment(Mat &bgr_img, bool is_fitted)
{
    //Mat3b bgr_img(imread(fileName.toStdString()));
    Mat src(bgr_img.size(), bgr_img.type()), mask(bgr_img.size(), bgr_img.type());
    cvtColor(bgr_img, src, COLOR_BGR2GRAY);
    threshold(src, mask, 0 ,255, THRESH_BINARY_INV | THRESH_OTSU);
    ConnectedComponents cc(mask);
    cv::Rect roi(src.cols / 3, src.rows / 3, src.cols / 3, src.rows / 3);
    std::map<int, int> areas = cc.areas(roi);
    if (areas.size() == 1)
        areas = cc.areas();
    int nevus_label = -1;
    int nevus_area = -1;
    for (auto label_and_area: areas) {
        int label = label_and_area.first;
        int area = label_and_area.second;
        if (label == 0)
            continue; //пропуск сегмента фона;
        if (nevus_area < area) {
            nevus_label = label;
            nevus_area = area;
        }
    }
    assert(nevus_label != -1);
    if (is_fitted)
        bgr_img = cc.fittedMask(nevus_label)(bgr_img);
    else
        bgr_img = cc.simpleMask(nevus_label)(bgr_img);
}

QImage MatToQImage(const cv::Mat &image)
{
    QImage ret(image.data, image.cols, image.rows, static_cast<int>(image.step), QImage::Format_RGB888);
    return ret.rgbSwapped();
}

enum thinningType{
    THINNING_ZHANGSUEN,
    THINNING_GUOHALL
};
// Applies a thinning iteration to a binary image

QImage darkAreaSelection(QString fileName, int treshold, int maxTresholdValue,
                         int minSquare)
{
    Mat src = imread(fileName.toStdString());

    getNevusSegment(src, true);
    imshow("original", src);

    Mat dst;
    src.copyTo(dst);

    cvtColor(src, src, CV_BGR2GRAY);
    imshow("gray", src);

    threshold(src, src, treshold, maxTresholdValue, THRESH_BINARY );
    imshow("threshold", src);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    findContours( src, contours, hierarchy, CV_RETR_TREE , CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

//    char text[200] = "";

    if (minSquare == -1)
        minSquare = static_cast<int>(dst.rows * dst.cols * 0.002);

    for( size_t i = 0; i < contours.size(); i++ )
    {
        cv::Rect brect = cv::boundingRect(contours[i]);
        if (brect.area() < minSquare)
            continue;
        if (brect.area() > dst.rows * dst.cols * 0.5)
            drawContours( dst, contours, static_cast<int>(i), CV_RGB(0, 255, 0), 2, 8, hierarchy, 0, Point() );
        else
            drawContours( dst, contours, static_cast<int>(i), CV_RGB(0, 0, 255), 2, 8, hierarchy, 0, Point() );
//        sprintf(text,"S = %d", brect.area());
//        putText(dst, text, cvPoint(brect.x+20, brect.y+20), 1, 1, CV_RGB(0,0,255));
//        rectangle(dst, brect, CV_RGB(0,0,255), 3);
    }
    imshow("findContours", dst);
    return MatToQImage(dst);
}

QImage pointSelection(QString fileName, Filters filter, int filterSize,
                      int coreGauss_x, int coreGauss_y,
                      int minDistance, int treshold,
                      int tresholdStorage , int minRadius, int maxRadius, int color, int minTh, int maxTh)
{
    Mat image = imread(fileName.toStdString(), 1);
    Mat gray, lab;
    //   отделяем новообразования от фона
    getNevusSegment(image, true);
    imshow("original", image);
    cvtColor(image, gray, COLOR_BGR2GRAY);
//    imshow("gray", gray);
    // smooth it, otherwise a lot of false circles may be detected
    switch (filter) {
    case gauss:
        // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
        GaussianBlur(gray, gray, Size(filterSize, filterSize), 2, 2, BORDER_DEFAULT);
        break;
    case median:
        medianBlur(gray, gray, filterSize);
        break;
    case box:
        blur(gray, gray, Size(filterSize, filterSize));
        break;
    }
//    imshow("blur", gray);
    cvtColor(image, lab, COLOR_BGR2HSV);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, minDistance, treshold, tresholdStorage, minRadius, maxRadius);
//    qDebug() << circles.size();

    for ( size_t i = 0; i < circles.size(); i++ ) {
        Point center(cvRound(circles[i][0] ), cvRound(circles[i][1]));
        int radius = cvRound(circles[i][2]);
        Vec3b tmp = image.at<Vec3b>(center);
        if (tmp == Vec3b(0, 0, 0))
            continue;
        int val = 0;
        int t = 0;
        t = tmp[2] / tmp[0];
        for (int i = 0; i < tmp.rows; i++){
          val += tmp[i];
        }
        val /= tmp.rows;
//        if ((val > 90) && (t > 5)) //105
//           continue;
        vector<int> L;
        vector<int> A;
        vector<int> B;


int fall = 0;
        vector<int> m;
        int mas[8];
        int maxRad = 0;
        int max = 0;
        int qmax = 0;
        int qmin = 1;
                for(size_t q = 0; q < 10; q++)
                {
                Point check(cvRound(circles[i][0]+ 0.15*q*radius), cvRound(circles[i][1]));
                Vec3b ch = image.at<Vec3b>(check);
                Vec3b lch = lab.at<Vec3b>(check);
                int l, a, b;
                l = lch[0];
                a = lch[1];
                b = lch[2];
                L.push_back(l);
                A.push_back(a);
                B.push_back(b);
                int avg = 0;
                    for (int i = 0; i < ch.rows; i++)
                      avg += ch[i];
                avg /= ch.rows;
                int r = val - avg;
                if (r == val){
                    qmin = 0;}
                    if (r > max){
                    max = r;
                    qmax = q;
                    m.push_back(qmax);
                    }
                }
        if ((qmax == 0))
            fall++;
        if ((qmin == 0))
            continue;
        int m1, m2;
        if (m.size() == 2){
        m1 = m[0];
        int p[10] = {0};
        p[0] = 0.15*qmax*radius - radius;
        if (p[0] < 0){
            p[0] = -p[0];
        }
        p[1] = 0.15*m1*radius - radius;
        if (p[1] < 0){
            p[1] = -p[1];
        }
        p[2] = 0.15*m1*radius - 0.15*qmax*radius;
        if (p[2] < 0){
            p[2] = -p[2];
        }
        for (int n = 0; n < 3; n++){
            if (p[n] >= maxRad){
                maxRad = p[n];
            }
        }
        mas[0] = maxRad;
        }
        else if (m.size() > 2) {
        m1 = m[m.size() - 2];
        m2 = m[m.size() - 3];
        int p[10] = {0};
        p[0] = 0.15*qmax*radius - radius;
        if (p[0] < 0){
            p[0] = -p[0];
        }
        p[1] = 0.15*m1*radius - radius;
        if (p[1] < 0){
            p[1] = -p[1];
        }
        p[3] = 0.15*m2*radius - radius;
        if (p[3] < 0){
            p[3] = -p[3];
        }
        p[2] = 0.15*m1*radius - 0.15*qmax*radius;
        if (p[2] < 0){
            p[2] = -p[2];
        }
        p[4] = 0.15*m2*radius - 0.15*qmax*radius;
        if (p[4] < 0){
            p[4] = -p[2];
        }
        p[5] = 0.15*m2*radius - 0.15*m1*radius;
        if (p[5] < 0){
            p[5] = -p[5];
        }
        for (int n = 0; n < 6; n++){
            if (p[n] >= maxRad){
                maxRad = p[n];
            }
        }
        mas[0] = maxRad;
        }
        else {
            m1 = 0;
            m2 = 0;
        maxRad = 0.15*qmax*radius - radius;
        mas[0] = maxRad;
        }
        Point check0(cvRound(circles[i][0]+ 0.15*qmax*radius), cvRound(circles[i][1]));
        Point checkk(cvRound(circles[i][0]+ 0.15*m1*radius), cvRound(circles[i][1]));
        Point checkq(cvRound(circles[i][0]+ 0.15*m2*radius), cvRound(circles[i][1]));
        m.clear();


        int max1 = 0;
        int qmax1 = 0;
        qmin = 1;
                for(size_t q = 0; q < 9; q++)
                {
                Point check(cvRound(circles[i][0]- 0.15*q*radius), cvRound(circles[i][1]));
                Vec3b ch = image.at<Vec3b>(check);
                Vec3b lch = lab.at<Vec3b>(check);
                int l, a, b;
                l = lch[0];
                a = lch[1];
                b = lch[2];
                L.push_back(l);
                A.push_back(a);
                B.push_back(b);
                int avg = 0;
                    for (int i = 0; i < ch.rows; i++)
                      avg += ch[i];
                avg /= ch.rows;
                int r = val - avg;
                if (r == val){
                    qmin = 0;}
                    if (r > max1){
                    max1 = r;
                    qmax1 = q;
                    m.push_back(qmax1);
                    }
                }
        if ((qmax1 == 0))
            fall++;
        if ((qmin == 0))
            continue;
        int m3, m4;
        if (m.size() == 2){
        m3 = m[0];}
        else if (m.size() > 2) {
        m3 = m[m.size() - 2];
        m4 = m[m.size() - 3];
        }
        else {
            m3 = 0;
            m4 = 0;
        }
        Point check1(cvRound(circles[i][0] - 0.15*qmax1*radius), cvRound(circles[i][1]));
        Point checkw(cvRound(circles[i][0] - 0.15*m3*radius), cvRound(circles[i][1]));
        Point checkr(cvRound(circles[i][0] - 0.15*m4*radius), cvRound(circles[i][1]));
        m.clear();

        int max2 = 0;
        int qmax2 = 0;
        qmin = 1;
                for(size_t q = 0; q < 9; q++)
                {
                Point check(cvRound(circles[i][0]), cvRound(circles[i][1])+ 0.15*q*radius);
                Vec3b ch = image.at<Vec3b>(check);
                Vec3b lch = lab.at<Vec3b>(check);
                int l, a, b;
                l = lch[0];
                a = lch[1];
                b = lch[2];
                L.push_back(l);
                A.push_back(a);
                B.push_back(b);
                int avg = 0;
                    for (int i = 0; i < ch.rows; i++)
                      avg += ch[i];
                avg /= ch.rows;
                int r = val - avg;
                if (r == val){
                    qmin = 0;}
                    if (r > max2){
                    max2 = r;
                    qmax2 = q;
                    m.push_back(qmax2);
                    }
                }
        if ((qmax2 == 0))
            fall++;
        if ((qmin == 0))
            continue;
        int m5, m6;
        if (m.size() == 2){
        m5 = m[0];
        int p[10] = {0};
        p[0] = 0.15*qmax2*radius - radius;
        if (p[0] < 0){
            p[0] = -p[0];
        }
        p[1] = 0.15*m5*radius - radius;
        if (p[1] < 0){
            p[1] = -p[1];
        }
        p[2] = 0.15*m5*radius - 0.15*qmax2*radius;
        if (p[2] < 0){
            p[2] = -p[2];
        }
        for (int n = 0; n < 3; n++){
            if (p[n] >= maxRad){
                maxRad = p[n];
            }
        }
        mas[2] = maxRad;}
        else if (m.size() > 2) {
        m5 = m[m.size() - 2];
        m6 = m[m.size() - 3];
        int p[10] = {0};
        p[0] = 0.15*qmax2*radius - radius;
        if (p[0] < 0){
            p[0] = -p[0];
        }
        p[1] = 0.15*m5*radius - radius;
        if (p[1] < 0){
            p[1] = -p[1];
        }
        p[3] = 0.15*m6*radius - radius;
        if (p[3] < 0){
            p[3] = -p[3];
        }
        p[2] = 0.15*m5*radius - 0.15*qmax2*radius;
        if (p[2] < 0){
            p[2] = -p[2];
        }
        p[4] = 0.15*m6*radius - 0.15*qmax2*radius;
        if (p[4] < 0){
            p[4] = -p[2];
        }
        p[5] = 0.15*m6*radius - 0.15*m5*radius;
        if (p[5] < 0){
            p[5] = -p[5];
        }
        for (int n = 0; n < 6; n++){
            if (p[n] >= maxRad){
                maxRad = p[n];
            }
        }
        mas[2] = maxRad;
        }
        else {
            m5 = 0;
            m6 = 0;
            maxRad = 0.15*qmax2*radius - radius;
            mas[2] = maxRad;
        }
        Point check2(cvRound(circles[i][0]), cvRound(circles[i][1])+ 0.15*qmax2*radius);
        Point checkt(cvRound(circles[i][0]), cvRound(circles[i][1])+ 0.15*m5*radius);
        Point checky(cvRound(circles[i][0]), cvRound(circles[i][1])+ 0.15*m6*radius);
        m.clear();


        int max3 = 0;
        int qmax3 = 0;
        qmin = 1;
                for(size_t q = 0; q < 9; q++)
                {
                Point check(cvRound(circles[i][0]), cvRound(circles[i][1])- 0.15*q*radius);
                Vec3b ch = image.at<Vec3b>(check);
                Vec3b lch = lab.at<Vec3b>(check);
                int l, a, b;
                l = lch[0];
                a = lch[1];
                b = lch[2];
                L.push_back(l);
                A.push_back(a);
                B.push_back(b);
                int avg = 0;
                    for (int i = 0; i < ch.rows; i++)
                      avg += ch[i];
                avg /= ch.rows;
                int r = val - avg;
                if (r == val){
                    qmin = 0;}
                    if (r > max3){
                    max3 = r;
                    qmax3 = q;
                    m.push_back(qmax3);
                    }
                }
        if ((qmax3 == 0))
            fall++;
        if ((qmin == 0))
            continue;
        int m7, m8;
        if (m.size() == 2){
        m7 = m[0];}
        else if (m.size() > 2) {
        m7 = m[m.size() - 2];
        m8 = m[m.size() - 3];
        }
        else {
            m7 = 0;
            m8 = 0;
        }
        Point check3(cvRound(circles[i][0]), cvRound(circles[i][1])- 0.15*qmax3*radius);
        Point checki(cvRound(circles[i][0]), cvRound(circles[i][1])- 0.15*m7*radius);
        Point checko(cvRound(circles[i][0]), cvRound(circles[i][1])- 0.15*m8*radius);
        m.clear();

        int max4 = 0;
        int qmax4 = 0;
        qmin = 1;
                for(size_t q = 0; q < 9; q++)
                {
                Point check(cvRound(circles[i][0]+ 0.12*q*radius), cvRound(circles[i][1])+ 0.12*q*radius);
                Vec3b ch = image.at<Vec3b>(check);
                Vec3b lch = lab.at<Vec3b>(check);
                int l, a, b;
                l = lch[0];
                a = lch[1];
                b = lch[2];
                L.push_back(l);
                A.push_back(a);
                B.push_back(b);
                int avg = 0;
                    for (int i = 0; i < ch.rows; i++)
                      avg += ch[i];
                avg /= ch.rows;
                int r = val - avg;
                if (r == val){
                    qmin = 0;}
                    if (r > max4){
                    max4 = r;
                    qmax4 = q;
                    m.push_back(qmax4);
                    }
                }
        if ((qmax4 == 0))
            fall++;
        if ((qmin == 0))
            continue;
        int m9, m10;
        if (m.size() == 2){
        m9 = m[0];
        int p[10] = {0};
        p[0] = 0.15*qmax4*radius - radius;
        if (p[0] < 0){
            p[0] = -p[0];
        }
        p[1] = 0.15*m9*radius - radius;
        if (p[1] < 0){
            p[1] = -p[1];
        }
        p[2] = 0.15*m9*radius - 0.15*qmax4*radius;
        if (p[2] < 0){
            p[2] = -p[2];
        }
        for (int n = 0; n < 3; n++){
            if (p[n] >= maxRad){
                maxRad = p[n];
            }
        }
        mas[4] = maxRad;}
        else if (m.size() > 2) {
        m9 = m[m.size() - 2];
        m10 = m[m.size() - 3];
        int p[10] = {0};
        p[0] = 0.15*qmax4*radius - radius;
        if (p[0] < 0){
            p[0] = -p[0];
        }
        p[1] = 0.15*m9*radius - radius;
        if (p[1] < 0){
            p[1] = -p[1];
        }
        p[3] = 0.15*m10*radius - radius;
        if (p[3] < 0){
            p[3] = -p[3];
        }
        p[2] = 0.15*m9*radius - 0.15*qmax4*radius;
        if (p[2] < 0){
            p[2] = -p[2];
        }
        p[4] = 0.15*m10*radius - 0.15*qmax4*radius;
        if (p[4] < 0){
            p[4] = -p[2];
        }
        p[5] = 0.15*m10*radius - 0.15*m9*radius;
        if (p[5] < 0){
            p[5] = -p[5];
        }
        for (int n = 0; n < 6; n++){
            if (p[n] >= maxRad){
                maxRad = p[n];
            }
        }
        mas[4] = maxRad;
        }
        else {
            m9 = 0;
            m10 = 0;
            maxRad = 0.15*qmax4*radius - radius;
            mas[4] = maxRad;
        }
        Point check4(cvRound(circles[i][0] + 0.12*qmax4*radius), cvRound(circles[i][1])+ 0.12*qmax4*radius);
        Point checkp(cvRound(circles[i][0] + 0.12*m9*radius), cvRound(circles[i][1])+ 0.12*m9*radius);
        Point checka(cvRound(circles[i][0] + 0.12*m10*radius), cvRound(circles[i][1])+ 0.12*m10*radius);
        m.clear();

        int max5 = 0;
        int qmax5 = 0;
        qmin = 1;
                for(size_t q = 0; q < 10; q++)
                {
                Point check(cvRound(circles[i][0]- 0.1*q*radius), cvRound(circles[i][1])+ 0.1*q*radius);
                Vec3b ch = image.at<Vec3b>(check);
                Vec3b lch = lab.at<Vec3b>(check);
                int l, a, b;
                l = lch[0];
                a = lch[1];
                b = lch[2];
                L.push_back(l);
                A.push_back(a);
                B.push_back(b);
                int avg = 0;
                    for (int i = 0; i < ch.rows; i++)
                      avg += ch[i];
                avg /= ch.rows;
                int r = val - avg;
                if (r == val){
                    qmin = 0;}
                    if (r > max5){
                    max5 = r;
                    qmax5 = q;
                    m.push_back(qmax5);
                    }
                }
         if ((qmax5 == 0))
             fall++;
         if ((qmin == 0))
             continue;
         int m11, m12;
         if (m.size() == 2){
         m11 = m[0];}
         else if (m.size() > 2) {
         m11 = m[m.size() - 2];
         m12 = m[m.size() - 3];
         }
         else {
             m11 = 0;
             m12 = 0;
         }
        Point check5(cvRound(circles[i][0] - 0.12*qmax5*radius), cvRound(circles[i][1])+ 0.12*qmax5*radius);
        Point checks(cvRound(circles[i][0] - 0.12*m11*radius), cvRound(circles[i][1])+ 0.12*m11*radius);
        Point checkd(cvRound(circles[i][0] - 0.12*m12*radius), cvRound(circles[i][1])+ 0.12*m12*radius);
        m.clear();

        int max6 = 0;
        int qmax6 = 0;
        qmin = 1;
                for(size_t q = 0; q < 9; q++)
                {
                Point check(cvRound(circles[i][0]- 0.12*q*radius), cvRound(circles[i][1])- 0.12*q*radius);
                Vec3b ch = image.at<Vec3b>(check);
                Vec3b lch = lab.at<Vec3b>(check);
                int l, a, b;
                l = lch[0];
                a = lch[1];
                b = lch[2];
                L.push_back(l);
                A.push_back(a);
                B.push_back(b);
                int avg = 0;
                    for (int i = 0; i < ch.rows; i++)
                      avg += ch[i];
                avg /= ch.rows;
                int r = val - avg;
                if (r == val){
                    qmin = 0;}
                    if (r > max6){
                    max6 = r;
                    qmax6 = q;
                    m.push_back(qmax6);
                    }
                }
         if ((qmax6 == 0))
             fall++;
         if ((qmin == 0))
             continue;
         int m13, m14;
         if (m.size() == 2){
         m13 = m[0];
         int p[10] = {0};
         p[0] = 0.15*qmax6*radius - radius;
         if (p[0] < 0){
             p[0] = -p[0];
         }
         p[1] = 0.15*m13*radius - radius;
         if (p[1] < 0){
             p[1] = -p[1];
         }
         p[2] = 0.15*m13*radius - 0.15*qmax6*radius;
         if (p[2] < 0){
             p[2] = -p[2];
         }
         for (int n = 0; n < 3; n++){
             if (p[n] >= maxRad){
                 maxRad = p[n];
             }
         }
         mas[6] = maxRad;}
         else if (m.size() > 2) {
         m13 = m[m.size() - 2];
         m14 = m[m.size() - 3];
         int p[10] = {0};
         p[0] = 0.15*qmax6*radius - radius;
         if (p[0] < 0){
             p[0] = -p[0];
         }
         p[1] = 0.15*m13*radius - radius;
         if (p[1] < 0){
             p[1] = -p[1];
         }
         p[3] = 0.15*m14*radius - radius;
         if (p[3] < 0){
             p[3] = -p[3];
         }
         p[2] = 0.15*m13*radius - 0.15*qmax6*radius;
         if (p[2] < 0){
             p[2] = -p[2];
         }
         p[4] = 0.15*m14*radius - 0.15*qmax6*radius;
         if (p[4] < 0){
             p[4] = -p[2];
         }
         p[5] = 0.15*m14*radius - 0.15*m13*radius;
         if (p[5] < 0){
             p[5] = -p[5];
         }
         for (int n = 0; n < 6; n++){
             if (p[n] >= maxRad){
                 maxRad = p[n];
             }
         }
         mas[6] = maxRad;
         }
         else {
             m13 = 0;
             m14 = 0;
             maxRad = 0.15*qmax6*radius - radius;
             mas[6] = maxRad;
         }
        Point check6(cvRound(circles[i][0] - 0.12*qmax6*radius), cvRound(circles[i][1])- 0.12*qmax6*radius);
        Point checkf(cvRound(circles[i][0] - 0.12*m13*radius), cvRound(circles[i][1])- 0.12*m13*radius);
        Point checkg(cvRound(circles[i][0] - 0.12*m14*radius), cvRound(circles[i][1])- 0.12*m14*radius);
        m.clear();

        int max7 = 0;
        int qmax7 = 0;
        qmin = 1;
                for(size_t q = 0; q < 9; q++)
                {
                Point check(cvRound(circles[i][0]+ 0.12*q*radius), cvRound(circles[i][1])- 0.12*q*radius);
                Vec3b ch = image.at<Vec3b>(check);
                Vec3b lch = lab.at<Vec3b>(check);
                int l, a, b;
                l = lch[0];
                a = lch[1];
                b = lch[2];
                L.push_back(l);
                A.push_back(a);
                B.push_back(b);
                int avg = 0;
                    for (int i = 0; i < ch.rows; i++)
                      avg += ch[i];
                avg /= ch.rows;
                int r = val - avg;
                if (r == val){
                    qmin = 0;}
                    if (r > max7){
                    max7 = r;
                    qmax7 = q;
                    m.push_back(qmax7);
                    }
                }
         if ((qmax7 == 0))
            fall++;
         if ((qmin == 0))
             continue;
         int m15, m16;
         if (m.size() == 2){
         m15 = m[0];}
         else if (m.size() > 2) {
         m15 = m[m.size() - 2];
         m16 = m[m.size() - 3];
         }
         else {
             m15 = 0;
             m16 = 0;
         }
        Point check7(cvRound(circles[i][0] + 0.12*qmax7*radius), cvRound(circles[i][1])- 0.12*qmax7*radius);
        Point checkh(cvRound(circles[i][0] + 0.12*m15*radius), cvRound(circles[i][1])- 0.12*m15*radius);
        Point checkj(cvRound(circles[i][0] + 0.12*m16*radius), cvRound(circles[i][1])- 0.12*m16*radius);
        m.clear();

        int avr_mas = (mas[0] + mas[2] + mas[4] + mas[6])/4;
        int sum_of_elem_l = accumulate(L.begin(),L.end(),0);
        int sum_of_elem_a = accumulate(A.begin(),A.end(),0);
        int sum_of_elem_b = accumulate(B.begin(),B.end(),0);
        int l_size = L.size();
        int a_size = A.size();
        int b_size = B.size();
        int avg_h = (sum_of_elem_l)/(l_size);
        int avg_s = (sum_of_elem_a * 100)/(255 * a_size);
        int avg_v = (sum_of_elem_b * 100)/(255 * b_size);
        if ((avr_mas > minTh) && (avr_mas < maxTh) && (color == 1) && (avg_s > 0) && (avg_s < 15) && (avg_v > 85) && (avg_v < 100)){
//        // draw the circle center
        circle( image, center, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
//        circle( image, check7, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
        // draw the circle outline
        if (fall < 3)
        circle( image, center, radius, Scalar(255, 0, 0), 1, 8, 0 );
        }
        else if ((avr_mas > minTh) && (avr_mas < maxTh) && (color == 2) && (avg_v < 30)){
        circle( image, center, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          if (fall < 3)
          circle( image, center, radius, Scalar(255, 0, 0), 1, 8, 0 );
        }
        else if ((avr_mas > minTh) && (avr_mas < maxTh) && (color == 3) && (avg_h > 0) && (avg_h < 30) && (((avg_s > 50) && (avg_v > 70)) || ((avg_s < 85) && (avg_v > 15)))){
        circle( image, center, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          if (fall < 3)
          circle( image, center, radius, Scalar(255, 0, 0), 1, 8, 0 );
        }
        else if ((avr_mas > minTh) && (avr_mas < maxTh) && (color == 4) && (avg_h > 10) && (avg_h < 55) && (avg_s > 50) && (avg_v > 50)){
        circle( image, center, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          if (fall < 3)
          circle( image, center, radius, Scalar(255, 0, 0), 1, 8, 0 );
        }
        else if ((avr_mas > minTh) && (avr_mas < maxTh) && (color == 5) && (((avg_s < 15) && (avg_s > 0)) || ((avg_s < 35) && (avg_v < 35) && (avg_s > 0) && (avg_v > 0)))){
        circle( image, center, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          if (fall < 3)
          circle( image, center, radius, Scalar(255, 0, 0), 1, 8, 0 );
        }
        else if ((avr_mas >= minTh) && (maxTh >= avr_mas ) && (color == 6)){
        circle( image, center, 0.5, Vec3b(255, 0, 0), -1, 8, 0 );
          if (fall < 3)
          circle( image, center, radius, Scalar(255, 0, 0), 1, 8, 0 );
          /*circle( image, check0, 0.5, Vec3b(0, 0, 255), -1, 8, 0 );
          circle( image, checkk, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, checkq, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, check1, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          circle( image, checkw, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, checkr, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, check2, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          circle( image, checkt, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, checky, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, check3, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          circle( image, checki, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, checko, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, check4, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          circle( image, checkp, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, checka, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, check5, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          circle( image, checks, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, checkd, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, check6, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          circle( image, checkf, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, checkg, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, check7, 0.5, Vec3b(0, 0, 0), -1, 8, 0 );
          circle( image, checkh, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );
          circle( image, checkj, 0.5, Vec3b(0, 255, 0), -1, 8, 0 );*/
        }
    }
    imshow("HoughCircles", image);

    return MatToQImage(image);
}

QImage npointSelection(QString fileName, Filters filter, int filterSize,
                      int coreGauss_x, int coreGauss_y,
                      int minDistance, int treshold,
                      int tresholdStorage , int minRadius, int maxRadius)
{
    Mat image = imread(fileName.toStdString(), 1);
    Mat gray;
    //   отделяем новообразования от фона
    getNevusSegment(image, true);
    imshow("original", image);
    cvtColor(image, gray, COLOR_BGR2GRAY);
//    imshow("gray", gray);
    // smooth it, otherwise a lot of false circles may be detected
    switch (filter) {
    case gauss:
        // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
        GaussianBlur(gray, gray, Size(filterSize, filterSize), coreGauss_x, coreGauss_y, BORDER_DEFAULT);
        break;
    case median:
        medianBlur(gray, gray, filterSize);
        break;
    case box:
        blur(gray, gray, Size(filterSize, filterSize));
        break;
    }
   // imshow("blur", gray);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1, minDistance, treshold, tresholdStorage, minRadius, maxRadius);
//    qDebug() << circles.size();

    for ( size_t i = 0; i < circles.size(); i++ ) {
        Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
        Vec3b tmp = image.at<Vec3b>(center);
        if (tmp == Vec3b(0, 0, 0))
            continue;
        int val = 0;
        int t = 0;
        t = tmp[2] / tmp[0];
        for (int i = 0; i < tmp.rows; i++)
          val += tmp[i];
        val /= tmp.rows;
        if (val > 100 || (t > 5) )
          continue;
        int radius = cvRound(circles[i][2]);
//        // draw the circle center
        circle( image, center, 1, Vec3b(0, 0, 0), -1, 8, 0 );
//        // draw the circle outline
        circle( image, center, radius, Scalar(255, 0, 0), 1, 8, 0 );
    }
    imshow("HoughCircles", image);

    return MatToQImage(image);
}

QImage objectSegmentation(QString fileName, Filters filter, int filterSize, int lowThreshold, int kernel_size, int ratio)
{
    Mat src, src_gray;
    Mat dst, detected_edges;

    src = imread(fileName.toStdString(), IMREAD_COLOR);
    getNevusSegment(src, true);
    imshow("original", src);
    dst.create( src.size(), src.type() );
    cvtColor( src, src_gray, COLOR_BGR2GRAY );
    imshow("gray", src_gray);
    switch (filter) {
    case gauss:
        // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
        GaussianBlur(src_gray, detected_edges, Size(filterSize, filterSize), 0, 0, BORDER_DEFAULT);
        break;
    case median:
        medianBlur(src_gray, detected_edges, filterSize);
        break;
    case box:
        blur(src_gray, detected_edges, Size(filterSize, filterSize));
        break;
    }
    imshow("blur", detected_edges);
    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, kernel_size );
    imshow("Canny", detected_edges);
    dst = Scalar::all(0);
    detected_edges = 255 - detected_edges;
    src.copyTo( dst, detected_edges);
    detected_edges = 255 - detected_edges;
    dst.setTo(Scalar(0, 255, 0), detected_edges);
    return MatToQImage(dst);
}
