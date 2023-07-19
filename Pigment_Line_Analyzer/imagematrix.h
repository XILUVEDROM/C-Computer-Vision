#ifndef IMAGEMATRIX_H
#define IMAGEMATRIX_H

#include <QImage>

/*!
 * \brief Класс копии загруженного изображения для удобства обработки
 */

class ImageMatrix
{
public:
    ImageMatrix(int c, int r);
    ImageMatrix(const QImage &src);
    ImageMatrix(ImageMatrix &src);
    ImageMatrix(ImageMatrix &src, int x, int y, int w, int h);
    void to_image(QImage &img);
    int rows();
    int cols();
    int get(int x,int y);
    void set(int value, int x, int y);
    void scale(int min, int max);
    bool isWhite();
    ~ImageMatrix();
private:
    int _cols;
    int _rows;
    int **matrix = nullptr;
};

#endif // IMAGEMATRIX_H
