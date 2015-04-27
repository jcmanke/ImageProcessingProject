
#pragma once
#include <qtimagelib.h>
#include "tiepoint.h"
#include "tpdialog.h"

class ImageTransformations : public QObject
{
    Q_OBJECT

private:
    Pixel* Bilinear(double x, double y, Image &image);

public slots:
    bool Menu_Transformation_ScaleByNearestNeighbor(Image &image);
    bool Menu_Transformation_ScaleByBilinearIntensity(Image &image);

    bool Menu_Transformation_RotationByNearestNeighbor(Image &image);
    bool Menu_Transformation_RotationByBilinearIntensity(Image &image);

    bool Menu_Transformation_GeneralWarp(Image &image);
};
