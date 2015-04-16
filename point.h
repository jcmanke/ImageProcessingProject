#ifndef POINT_H
#define POINT_H

#include <qtimagelib.h>

class Point
{
    private:
        double x;
        double y;
        Pixel pix;

    public:
        Point(double x, double y, Pixel pixel);
        double X();
        double Y();
        Pixel Pix();
};

#endif // POINT_H
