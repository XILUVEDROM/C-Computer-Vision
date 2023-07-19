#include "processing.h"

#include <math.h>
using namespace std;

Processing::Processing()
{

}

/*!
 * \brief Билатеральный фильтр
 * \param matrix - изображение для обработки в фотмате ImageMatrix
 */
void Processing::bilateralFilter(ImageMatrix &matrix, int radius, int sigma_d, int sigma_i)
{
    ImageMatrix copy(matrix);
    int rows = matrix.rows();
    int cols = matrix.cols();
    for (int x=radius;x<rows-radius;++x)
        for (int y=radius;y<cols-radius;++y)
        {
            double I = copy.get(x,y);
            double intens = 0, weight=0;
            for(int i=-radius;i<=radius;++i) for(int j=-radius;j<=radius;++j)
            {
                int xn=x+i; int yn=y+j;
                double tw = exp(-((i*i+j*j)/(2*sigma_d*sigma_d))-
                                ((copy.get(xn,yn)-I)*(copy.get(xn,yn)-I)/(2*sigma_i*sigma_i)));
                weight+=tw;
                intens += copy.get(xn,yn)*tw;
            }
            matrix.set(int(intens/weight),x,y);
        }
}

/*!
 * \brief Функция бинаризации алгоритмом Отсу
 * \param matrix [in] - изображение для обработки в фотмате ImageMatrix
 * \param [out] threshold - порог, определенный алгоритмом Отсу
 */
void Processing::otsuThreshold(ImageMatrix &matrix, int &threshold)
{
    matrix.scale(0,255);
    int rows = matrix.rows();
    int cols = matrix.cols();
    int hist[256];
    int sum=0;
    for (int i=0; i<256; ++i)
            hist[i]=0;
    for (int i=0; i<rows; ++i)
        for (int j=0;j<cols;++j)
            ++hist[int(matrix.get(i,j))];
    for (int i=0; i<rows; ++i)
        for (int j=0;j<cols;++j)
            sum += matrix.get(i,j);

    int all_pixel_count = rows*cols;
    int all_intensity_sum = sum;

    int best_thresh = 0;
    double best_sigma = 0.0;

    int first_class_pixel_count = 0;
    int first_class_intensity_sum = 0;

    for (int thresh = 0; thresh < 255; ++thresh)
    {
        first_class_pixel_count += hist[thresh];
        first_class_intensity_sum += thresh * hist[thresh];

        double first_class_prob = first_class_pixel_count / (double)all_pixel_count;
        double second_class_prob = 1.0 - first_class_prob;

        double first_class_mean = first_class_intensity_sum / (double)first_class_pixel_count;
        double second_class_mean = (all_intensity_sum - first_class_intensity_sum)
                / (double)(all_pixel_count - first_class_pixel_count);

        double mean_delta = first_class_mean - second_class_mean;

        double sigma = first_class_prob * second_class_prob * mean_delta * mean_delta;

        if (sigma > best_sigma) {
            best_sigma = sigma;
            best_thresh = thresh;
        }
    }

    threshold = best_thresh;
    for (int i=0; i<rows; ++i)
        for (int j=0; j<cols; ++j)
            matrix.set(matrix.get(i,j)<best_thresh?0:255,i,j);
}

/*!
 * \brief Функция пороговой бинаризации
 * \param matrix - изображение для обработки в фотмате ImageMatrix
 * \param threshold - порог бинаризации
 */
void Processing::threshold(ImageMatrix &matrix, int threshold)
{
    int rows = matrix.rows();
    int cols = matrix.cols();
    for (int i=0; i<rows; ++i)
        for (int j=0; j<cols; ++j)
            matrix.set(matrix.get(i,j)<threshold?0:255,i,j);
}

void Processing::erosion(ImageMatrix &matrix, int radius)
{
    matrix.scale(0,1);
    ImageMatrix tmp (matrix);
    int rows = matrix.rows();
    int cols = matrix.cols();
    int xn, yn;
    for (int x=radius; x<rows-radius; ++x)
        for (int y=radius; y<cols-radius; ++y)
        {
                int b = 1;
                for (int i=-radius; i<=radius; ++i)
                    for (int j=-radius; j<=radius; ++j)
                        if (i!=j)
                        {
                            xn = x+i; yn = y+j;
                            if (true && (tmp.get(xn,yn) < b))
                                b = tmp.get(xn,yn);
                        }
                matrix.set(b,x,y);
        }
    matrix.scale(0,255);
}

void Processing::dilatation(ImageMatrix &matrix, int radius)
{
    matrix.scale(0,1);
    ImageMatrix tmp (matrix);
    int rows = matrix.rows();
    int cols = matrix.cols();
    int xn, yn;
    for (int x=radius; x<rows-radius; ++x)
        for (int y=radius; y<cols-radius; ++y)
        {
                int b = 0;
                for (int i=-radius; i<=radius; ++i)
                    for (int j=-radius; j<=radius; ++j)
                    {
                        xn = x+i; yn = y+j;
                        if (true && tmp.get(xn,yn) > b)
                            b = tmp.get(xn,yn);
                    }
                matrix.set(b,x,y);
        }
    matrix.scale(0,255);
}

/*!
 * \brief Функция определения граничых пискелей
 * \param matrix - изображение в фотмате ImageMatrix
 * \param x, y - координаты пикселя
 * \param r - радиус рассмотрения соседних пикселей
 * \return true, если в радиусе r есть черные пиксели, иначе - false
 */
bool Processing::hasBlackNeig(ImageMatrix &matrix, int x, int y, int r)
{
    int xn, yn;
    if (matrix.get(x, y) == 0)
        return true;
    for (int i=-r*2; i<=r*2; ++i)
        for (int j=-r*2; j<=r*2; ++j)
        {
            xn = x+i; yn = y+j;
            if (matrix.get(xn, yn) == 0)
                return true;
        }
    return false;
}

/*!
 * \brief Функция устанавливает значение соседних пикселей в 0
 * \param matrix - изображение в фотмате ImageMatrix
 * \param x, y - координаты пикселя
 * \param r - радиус рассмотрения соседей
 */
void Processing::setNeigBlack(ImageMatrix &matrix, int x, int y, int r)
{
    int xn, yn;
    for (int i=-r*2; i<=r*2; ++i)
        for (int j=-r*2; j<=r*2; ++j)
        {
            xn = x+i; yn = y+j;
            matrix.set(0,xn,yn);
        }
}

/*!
 * \brief Функция обраотки изображение согласованными фильтрами
 * \param matrix - изображение для обработки в фотмате ImageMatrix
 */
void Processing::matchedFiler(ImageMatrix &matrix)
{
    int v_matrix[13][13] = { {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8},
                             {8,4,2,1,-1,-2,-24,-2,-1,1,2,4,8} };

    int h_matrix[13][13] = { {8,8,8,8,8,8,8,8,8,8,8,8,8},
                             {4,4,4,4,4,4,4,4,4,4,4,4,4},
                             {2,2,2,2,2,2,2,2,2,2,2,2,2},
                             {1,1,1,1,1,1,1,1,1,1,1,1,1},
                             {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                             {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
                             {-24,-24,-24,-24,-24,-24,-24,-24,-24,-24,-24,-24,-24},
                             {-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2,-2},
                             {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
                             {1,1,1,1,1,1,1,1,1,1,1,1,1},
                             {2,2,2,2,2,2,2,2,2,2,2,2,2},
                             {4,4,4,4,4,4,4,4,4,4,4,4,4},
                             {8,8,8,8,8,8,8,8,8,8,8,8,8} };

    int r_matrix[13][13] = {{79,32,16,12,8,4,2,1,-1,-2,-4,-8,-24},
                            {32,16,12,8,4,2,1,-1,-2,-4,-8,-24,-8},
                            {16,12,8,4,2,1,-1,-2,-4,-8,-24,-8,-4},
                            {12,8,4,2,1,-1,-2,-4,-8,-24,-8,-4,-2},
                            {8,4,2,1,-1,-2,-4,-8,-24,-8,-4,-2,-1},
                            {4,2,1,-1,-2,-4,-8,-24,-8,-4,-2,-1,1},
                            {2,1,-1,-2,-4,-8,-24,-8,-4,-2,-1,1,2},
                            {1,-1,-2,-4,-8,-24,-8,-4,-2,-1,1,2,4},
                            {-1,-2,-4,-8,-24,-8,-4,-2,-1,1,2,4,8},
                            {-2,-4,-8,-24,-8,-4,-2,-1,1,2,4,8,12},
                            {-4,-8,-24,-8,-4,-2,-1,1,2,4,8,12,16},
                            {-8,-24,-8,-4,-2,-1,1,2,4,8,12,16,32},
                            {-24,-8,-4,-2,-1,1,2,4,8,12,16,32,79} };

    int l_matrix[13][13] = { {-24,-8,-4,-2,-1,1,2,4,8,12,16,32,79},
                             {-8,-24,-8,-4,-2,-1,1,2,4,8,12,16,32},
                             {-4,-8,-24,-8,-4,-2,-1,1,2,4,8,12,16},
                             {-2,-4,-8,-24,-8,-4,-2,-1,1,2,4,8,12},
                             {-1,-2,-4,-8,-24,-8,-4,-2,-1,1,2,4,8},
                             {1,-1,-2,-4,-8,-24,-8,-4,-2,-1,1,2,4},
                             {2,1,-1,-2,-4,-8,-24,-8,-4,-2,-1,1,2},
                             {4,2,1,-1,-2,-4,-8,-24,-8,-4,-2,-1,1},
                             {8,4,2,1,-1,-2,-4,-8,-24,-8,-4,-2,-1},
                             {12,8,4,2,1,-1,-2,-4,-8,-24,-8,-4,-2},
                             {16,12,8,4,2,1,-1,-2,-4,-8,-24,-8,-4},
                             {32,16,12,8,4,2,1,-1,-2,-4,-8,-24,-8},
                             {79,32,16,12,8,4,2,1,-1,-2,-4,-8,-24} };


    int rows = matrix.rows();
    int cols = matrix.cols();
    int radius = 6, xn, yn;
    int v,h,r,l, max_effect;
    ImageMatrix tmp (matrix);
    for (int x=radius; x<rows-radius; ++x)
        for (int y=radius; y<cols-radius; ++y)
        {
            if (!hasBlackNeig(tmp, x, y, radius))
            {
                v=0;h=0;r=0;l=0;
                for (int i=-radius; i<=radius; ++i)
                    for (int j=-radius; j<=radius; ++j)
                    {
                        xn = x+i; yn = y+j;
                        v += tmp.get(xn,yn) * v_matrix[i+radius][j+radius];
                        h += tmp.get(xn,yn) * h_matrix[i+radius][j+radius];
                        r += tmp.get(xn,yn) * r_matrix[i+radius][j+radius];
                        l += tmp.get(xn,yn) * l_matrix[i+radius][j+radius];
                    }
                max_effect = std::max(v,h);
                matrix.set(max_effect, x,y);
            }
            else
                setNeigBlack(matrix, x, y, radius);
        }
    matrix.scale(0,255);
}

/*!
 * \brief Функция удаления сегментов на изображении-маске
 *        с длиной линии скелета меньше заданного значения
 * \param matrix - изображение-маска
 * \param skelet - линии скелета от изображения-маски
 * \param min_size - минимальный допустимый размер участка скелета
 */
void Processing::removeSmallOnMask(ImageMatrix &matrix, ImageMatrix &skelet, int min_size)
{
    deque<QPoint> skelet_segm;
    skelet.scale(0,1);
    matrix.scale(0,1);
    for(int x=0;x<skelet.rows();++x)
        for(int y=0;y<skelet.cols();++y)
            skelet.set(skelet.get(x,y)-2,x,y);
    for(int x=0;x<skelet.rows();++x)
        for(int y=0;y<skelet.cols();++y)
        {
            if (skelet_segm.size() != 0)
                skelet_segm.clear();
            if (skelet.get(x,y) == -1)
            {
                skelet_segm.push_back({x,y});
                int count=0;
                while(count < skelet_segm.size())
                {
                    int xn = (*(skelet_segm.begin()+count)).x();
                    int yn = (*(skelet_segm.begin()+count)).y();
                    vector<QPoint> neigh {{xn, yn-1},{xn-1, yn},{xn+1, yn},{xn, yn+1}
                                               ,{xn-1,yn-1},{xn-1,yn+1},{xn+1,yn-1},{xn+1,yn+1}     };
                    for (QPoint p:neigh)
                    {
                        if (p.x()>0 && p.x()<skelet.rows() && p.y()>0 && p.y()<skelet.cols() &&
                                skelet.get(p.x(),p.y()) == -1)
                            if(std::count(skelet_segm.begin(), skelet_segm.end(), p)==0)
                                skelet_segm.push_back(p);
                    }
                    ++count;
                }
                if(count>min_size)
                {
                    for (QPoint p:skelet_segm)
                        skelet.set(0, p.x(), p.y());
                }
                //if ((count+1)<=max_size)
                else
                {
                    for (QPoint p:skelet_segm)
                        skelet.set(-2, p.x(), p.y());
                    QPoint f = skelet_segm.at(0);
                    deque<QPoint> matrix_segm;
                    matrix_segm.push_back(f);
                    int count=0;
                    while(count < matrix_segm.size())
                    {
                        int xn = (*(matrix_segm.begin()+count)).x();
                        int yn = (*(matrix_segm.begin()+count)).y();
                        vector<QPoint> neigh {{xn, yn-1},{xn-1, yn},{xn+1, yn},{xn, yn+1}
                                             ,{xn-1,yn-1},{xn-1,yn+1},{xn+1,yn-1},{xn+1,yn+1}};
                        for (QPoint p:neigh)
                        {
                            if (p.x()>0 && p.x()<matrix.rows() && p.y()>0 && p.y()<matrix.cols() &&
                                    matrix.get(p.x(),p.y()) == 1)
                                if(std::count(matrix_segm.begin(), matrix_segm.end(), p)==0)
                                    matrix_segm.push_back(p);
                        }
                        ++count;
                    }
                    for (QPoint p:matrix_segm)
                        matrix.set(0, p.x(), p.y());
                }
            }
        }
    matrix.scale(0,255);
    skelet.scale(0,255);
}

/*!
 * \brief Функция удаления сегментов линии скелета размером меньше заданного значения
 * \param skelet - изображение скелета
 * \param max_size - минимальный допустимый размер сегмента
 */
void Processing::removeSmallOnSkelet(ImageMatrix &skelet, int max_size)
{
    deque<QPoint> skelet_segm;
    skelet.scale(0,1);
    for(int x=0;x<skelet.rows();++x)
        for(int y=0;y<skelet.cols();++y)
            skelet.set(skelet.get(x,y)-2,x,y);
    for(int x=0;x<skelet.rows();++x)
        for(int y=0;y<skelet.cols();++y)
        {
            if (skelet_segm.size() != 0)
                skelet_segm.clear();
            if (skelet.get(x,y) == -1)
            {
                skelet_segm.push_back({x,y});
                int count=0;
                while(count < skelet_segm.size())
                {
                    int xn = (*(skelet_segm.begin()+count)).x();
                    int yn = (*(skelet_segm.begin()+count)).y();
                    vector<QPoint> neigh {{xn, yn-1},{xn-1, yn},{xn+1, yn},{xn, yn+1}
                                               ,{xn-1,yn-1},{xn-1,yn+1},{xn+1,yn-1},{xn+1,yn+1}     };
                    for (QPoint p:neigh)
                    {
                        if (p.x()>0 && p.x()<skelet.rows() && p.y()>0 && p.y()<skelet.cols() &&
                                skelet.get(p.x(),p.y()) == -1)
                            if(std::count(skelet_segm.begin(), skelet_segm.end(), p)==0)
                                skelet_segm.push_back(p);
                    }
                    ++count;
                }
                if(count>max_size)
                {
                    for (QPoint p:skelet_segm)
                        skelet.set(0, p.x(), p.y());
                }
                if ((count+1)<=max_size)
                {
                    for (QPoint p:skelet_segm)
                        skelet.set(-2, p.x(), p.y());
                }
            }
        }
    skelet.scale(0,255);
}

/*!
 * \brief Функция определения количества соседей
 * \param matrix - изображение
 * \param x, y - координаты пикселя
 * \param r - радиус рассмотрения соседей
 * \return true, если у пикселя в радиусе r больше 2 соседей, иначе - false
 */
bool Processing::hasMoreThanTwoNeig(ImageMatrix &matrix, int x, int y, int r)
{
    int xn, yn, count = 0;
    for (int i=-r; i<=r; ++i)
        for (int j=-r; j<=r; ++j)
        {
            xn = x+i; yn = y+j;
            if (matrix.get(xn, yn) == 1)
                ++count;
        }
    return count > 3;
}

/*!
 * \brief Функция удаления точек пересечения линий в 1 пиксель
 */
void Processing::removeCross(ImageMatrix &matrix)
{
    vector<QPoint> vect;
    matrix.scale(0,1);
    for (int i=0;i<matrix.rows();++i)
        for (int j=0;j<matrix.cols();++j)
            if (matrix.get(i, j) == 1 && hasMoreThanTwoNeig(matrix, i, j, 1))
                vect.push_back(QPoint(j, i));
    for (QPoint p : vect)
        matrix.set(0, p.y(), p.x());
    matrix.scale(0, 255);
}

/*!
 * \brief Функция выделения на оригинальном изображении областей, заданных маской
 * \param orig - оригинальное изображение
 * \param mask - изображение-маска: белые линии на черном фоне
 */
void Processing::cutOrigin(QImage &orig, QImage &mask)
{
    for (int i = 0; i < orig.width(); ++i)
        for (int j = 0; j < orig.height(); ++j)
            if (mask.pixelColor(i, j) == QColor(0,0,0))
                orig.setPixelColor(i, j, QColor(0,0,0));
}

void Processing::laplace(ImageMatrix &matrix)
{
    matrix.scale(0,1);
    int rows = matrix.rows();
    int cols = matrix.cols();
    int radius = 1, xn, yn, t;
    ImageMatrix tmp (matrix);
    int G[3][3] = { {0,1,0},
                    {1,-4,1},
                    {0,1,0} };

    for (int x=radius; x<rows-radius; ++x)
        for (int y=radius; y<cols-radius; ++y)
        {
            t=0;
            for (int i=-radius; i<=radius; ++i)
                for (int j=-radius; j<=radius; ++j)
                {
                    xn = x+i; yn = y+j;
                    t += tmp.get(xn,yn) * G[i+radius][j+radius];
                }
            matrix.set( (t==0)?0:1 ,x,y);
        }
    matrix.scale(0,255);
}



