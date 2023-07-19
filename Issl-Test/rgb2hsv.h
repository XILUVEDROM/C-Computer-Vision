#ifndef RGB2HSV_H
#define RGB2HSV_H

#include <iostream>

class RGB2HSV
{
public:
    RGB2HSV();

public:
   void RGB2HSVv (unsigned int &t, unsigned char &sat, unsigned char &val,

    unsigned char r, unsigned char g, unsigned char b);
};

#endif // RGB2HSV_H
