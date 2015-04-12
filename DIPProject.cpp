#include "DIPProject.h"

int main( int argc, char *argv[] )
{
    ImageTransformations obj;
    ImageApp app( argc, argv );
    app.AddActions( &obj );
    return app.Start();
}

bool ImageTransformations::Menu_Transformation_GeneralWarp(Image &image)
{
    return true;
}

bool ImageTransformations::Menu_Transformation_RotationByBilinearIntensity(Image &image)
{
    return true;
}

bool ImageTransformations::Menu_Transformation_RotationByNearestNeighbor(Image &image)
{
    return true;
}

bool ImageTransformations::Menu_Transformation_ScaleByBilinearIntensity(Image &image)
{
    return true;
}

bool ImageTransformations::Menu_Transformation_ScaleByNearestNeighbor(Image &image)
{
    return true;
}
