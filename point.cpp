#include "point.h"

Point::Point(double _x, double _y, uint _intensity)
{
    x = _x;
    y =_y;
    intensity = _intensity;
}

double Point::X()
{
    return x;
}

double Point::Y()
{
    return y;
}

uint Point::Intensity()
{
    return intensity;
}
