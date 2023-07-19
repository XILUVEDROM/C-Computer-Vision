#include "imagematrix.h"

/*!
 * \brief Конструктор пустой матрицы с заданными размерами
 * \param c - ширина
 * \param r - длина
 */
ImageMatrix::ImageMatrix(int c, int r):_cols(c),_rows(r)
{
    matrix = new int* [_rows];
    for (int i=0; i<_rows; ++i)
        matrix[i] = new int [_cols];
}

/*!
 * \brief Конструктор для преобразования изображения QImage в ImageMatrix
 * \param src - изображение QImage
 */
ImageMatrix::ImageMatrix(const QImage &src):ImageMatrix(src.width(), src.height())
{
    for(int i=0;i<_rows;++i) for(int j=0;j<_cols;++j)
        matrix[i][j] = (src.pixelColor(j,i).red()+src.pixelColor(j,i).green()
                        +src.pixelColor(j,i).blue())/3;
}

/*!
 * \brief Конструктор копирования
 * \param src
 */
ImageMatrix::ImageMatrix(ImageMatrix &src):ImageMatrix(src.cols(), src.rows())
{
    for(int r=0;r<_rows;++r)
        memcpy(matrix[r],src.matrix[r],sizeof(int)*_cols);
}

/*!
 * \brief Копирование части марицы
 * \param src - исходная матрица
 * \param x - x-координата верхней левой точки
 * \param y - y-координата верхней левой точкис
 * \param w - ширина копируемого участка
 * \param h - длина копируемого участка
 */
ImageMatrix::ImageMatrix(ImageMatrix &src, int x, int y, int w, int h): ImageMatrix(w, h)
{
    for(int i=0;i<_rows;++i) for(int j=0;j<_cols;++j)
        matrix[i][j] = src.get(i+x, j+y);
}

/*!
 * \brief Функция преобразования матрицы в изображение QImage
 * \param img - выходное изображение
 */
void ImageMatrix::to_image(QImage &img)
{
    img.scaled(_cols,_rows);
    for(int i=0;i<_rows;++i) for(int j=0;j<_cols;++j)
        img.setPixelColor(j,i,QColor(matrix[i][j],matrix[i][j],matrix[i][j]));
}

int ImageMatrix::rows()
{
    return _rows;
}

int ImageMatrix::cols()
{
    return _cols;
}

int ImageMatrix::get(int x, int y)
{
    if (x>0 && x<_rows && y>0 && y<_cols)
        return matrix[x][y];
    else
        return 0;
}

void ImageMatrix::set(int value, int x, int y)
{
    if (x>0 && x<_rows && y>0 && y<_cols)
        matrix[x][y] = value;
    else
        return;
}

/*!
 * \brief Функция приведения значений матрицы к нжному диапазону
 * \param min - минимальное значение
 * \param max - максимальное значение
 */
void ImageMatrix::scale(int min, int max)
{
    int m_max = matrix[0][0], m_min = matrix[0][0], t;
    for (int i=0; i<_rows; ++i)
        for (int j=0; j<_cols; ++j)
        {
            t = matrix[i][j];
            m_max = std::max(m_max, t);
            m_min = std::min(m_min, t);
        }
    for (int i=0; i<_rows; ++i)
        for (int j=0; j<_cols; ++j)
        {
            t = matrix[i][j];
            matrix[i][j] = ((max-min)*(t-m_min)/(m_max-m_min))+min;
        }
}

/*!
 * \brief Вспомогательная функция для проверки количества "белых" пикселей
 * \return true, если белый, иначе - false
 */
bool ImageMatrix::isWhite()
{
    int cx = 0, cy = 0;
    for (int i = 25; i <= 75; ++i)
        if (this->get(i, 25) == 255)
            ++cx;
    for (int i = 25; i <= 75; ++i)
        if (this->get(25, i) == 255)
            ++cy;
    if (cx > 40 && cy > 40)
        return true;
    else
        return false;
}

ImageMatrix::~ImageMatrix()
{
    for (int r=0;r<_rows;++r)
        delete[] matrix[r];
    delete[] matrix;
}
