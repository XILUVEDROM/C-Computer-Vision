#ifndef COUNTOUR_H
#define COUNTOUR_H
#include <vector>


class Countour
{

private:

    //vector to store coords
    typedef std::pair<int,int> pixelcoord;
    std::vector <pixelcoord> CountourObject;




public:
    Countour();
    int getSize();
    void PushValue(int x,int y);
    std::pair<int,int> getCountourBeginning();
    std::pair<int,int> getCountourPixel(int x);


};

#endif // COUNTOUR_H
