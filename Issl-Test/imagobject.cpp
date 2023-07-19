#include "imagobject.h"

void ImgObject::setCountour(const Countour &value)
{
    countour = value;
}

std::pair<int, int> ImgObject::getObjectPixel(int x)
{
    return Img_Object[x];
}


Countour ImgObject::getCountour() const
{
    return countour;
}


int ImgObject::getSquare()
{
    return Img_Object.size();
}

int ImgObject::getPerimetr()
{
    return countour.getSize();
}



void ImgObject::setIsArea(bool value)
{
    IsArea = value;
}

bool ImgObject::getIsArea() const
{
    return IsArea;
}

ImgObject::ImgObject()
{

}

int ImgObject::getSize()
{
    return Img_Object.size();
}

void ImgObject::PushValue(int x, int y)
{
    Img_Object.push_back(std::make_pair(x,y));
}
