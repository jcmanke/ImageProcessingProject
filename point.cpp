#include "point.h"

Point::Point(uint _x, uint _y, uint _intensity)
{
    x = _x;
    y =_y;
    intensity = _intensity;
}

uint Point::X()
{
    return x;
}

uint Point::Y()
{
    return y;
}

uint Point::Intensity()
{
    return intensity;
}
