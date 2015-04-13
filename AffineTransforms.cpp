#include "DIPProject.h"

bool getScaledImage(Image &originalImage, Image &newImage)
{
    double x_scale = 1.0;
    double y_scale = 1.0;

    Dialog input = Dialog("Scaling").Add(x_scale, "X Scale Factor", 0.0, 10.0).Add(y_scale, "Y Scale Factor", 0.0, 10.0);

    if (input.Show())
    {
        newImage = Image(originalImage.Height() * y_scale, originalImage.Width() * x_scale);
        return true;
    }
    else
    {
        return false;
    }
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
    Image newImage;
    if (getScaledImage(image, newImage))
    {
        for (uint y = 0; y < image.Height(); y++)
        {
            for (uint x = 0; x < image.Width(); x++)
            {
                //DO THE STUFF
            }
        }

        image = newImage;
        return true;
    }
    else
    {
        return false;
    }
}

bool ImageTransformations::Menu_Transformation_ScaleByNearestNeighbor(Image &image)
{
    Image newImage;
    if (getScaledImage(image, newImage))
    {
        for (uint y = 0; y < image.Height(); y++)
        {
            for (uint x = 0; x < image.Width(); x++)
            {
                //DO THE STUFF
            }
        }

        image = newImage;
        return true;
    }
    else
    {
        return false;
    }
}
