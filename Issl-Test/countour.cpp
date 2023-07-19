#include "countour.h"
#include <iostream>

Countour::Countour()
{

}

int Countour::getSize()
{
    return CountourObject.size();
}


void Countour::PushValue(int x, int y)
{
    CountourObject.push_back(std::make_pair(x,y));
}

std::pair<int, int> Countour::getCountourBeginning()
{
    return CountourObject[0];
}

std::pair<int, int> Countour::getCountourPixel(int x)
{
    return CountourObject[x];
}

