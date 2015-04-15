#ifndef POINT_H
#define POINT_H

#include <qtimagelib.h>

class Point
{
    private:
        double x;
        double y;
        uint intensity;

    public:
        Point(double x, double y, uint intensity);
        double X();
        double Y();
        uint Intensity();
};

#endif // POINT_H
