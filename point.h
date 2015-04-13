#ifndef POINT_H
#define POINT_H

#include <qtimagelib.h>

class Point
{
    private:
        uint x;
        uint y;
        uint intensity;

    public:
        Point(uint x, uint y, uint intensity);
        uint X();
        uint Y();
        uint Intensity();
};

#endif // POINT_H
