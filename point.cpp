#include "point.h"

/**
    Author: Adam Meaney
    Description: This is a convenience class to allow me to store each of the points
        for ease of reference.
  */
Point::Point(double _x, double _y, Pixel &pixel)
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
