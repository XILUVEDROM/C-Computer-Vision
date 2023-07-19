#ifndef IMGOBJECT_H
#define IMGOBJECT_H
#include "countour.h"


class ImgObject
{

private:

    Countour countour;
    typedef std::pair<int,int> obj_pixelcoord;
    std::vector <obj_pixelcoord> Img_Object;
    int Square;
    int Perimetr;
    bool IsArea;


public:
    ImgObject();
    int getSize();
    void PushValue(int x,int y);
    void setCountour(const Countour &value);
    std::pair<int,int> getObjectPixel(int x);
    Countour getCountour() const;
    int getSquare();
    int getPerimetr();

    void setIsArea(bool value);
    bool getIsArea() const;
};

#endif // IMGOBJECT_H
