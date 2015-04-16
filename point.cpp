#include "point.h"

Point::Point(double _x, double _y, Pixel pixel)
{
    x = _x;
    y = _y;
    pix = pixel;
}

double Point::X()
{
    return x;
}

double Point::Y()
{
    return y;
}

Pixel Point::Pix()
{
    return pix;
}
