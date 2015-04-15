#include "DIPProject.h"
#include "point.h"
#include <cmath>

using namespace std;

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

double degreesToRadians(int angle)
{
    return angle * M_PI / 180;
}

bool ImageTransformations::Menu_Transformation_RotationByBilinearIntensity(Image &image)
{
    return true;
}

bool ImageTransformations::Menu_Transformation_RotationByNearestNeighbor(Image &image)
{
    //get angle from user
    int angle = 45;

    Dialog input = Dialog("Rotation").Add(angle, "Rotation Angle", 0, 360);

    if (input.Show())
    {
        //create new image of same size (will result in clipped corners)
        Image newImage = Image(image.Height(), image.Width());

        //calculate constants
        double radian = degreesToRadians(angle);
        double sine = sin(radian);
        double cosine = cos(radian);
        uint centerY = image.Height() / 2;
        uint centerX = image.Width() / 2;

        for (uint y = 0; y < newImage.Height(); y++)
        {
            int diffY = y - centerY;

            for(uint x = 0; x < newImage.Width(); x++)
            {
                int diffX = x - centerX;

                //inverse mapping
                int origX = diffX * cosine - diffY * sine + centerX + 0.5;
                int origY = diffY * cosine + diffX * sine + centerY + 0.5;

                if (origY >= 0 && origY < image.Height() && origX >= 0 && origX < image.Width())
                {
                    newImage[y][x] = image[origY][origX];
                }
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

bool ImageTransformations::Menu_Transformation_ScaleByBilinearIntensity(Image &image)
{
    double x_scale = 1.0;
    double y_scale = 1.0;

    Dialog input = Dialog("Scaling").Add(x_scale, "X Scale Factor", 0.0, 10.0).Add(y_scale, "Y Scale Factor", 0.0, 10.0);

    if (input.Show())
    {
        Image newImage = Image(image.Height() * y_scale, image.Width() * x_scale);

        for (uint y = 1; y < newImage.Height() - 1; y++)
        {
            double origY = y / y_scale;
            origY = min(origY, (double) (image.Height() - 2));

            for (uint x = 1; x < newImage.Width() - 1; x++)
            {
                double origX = x / x_scale;
                origX = min(origX, (double) (image.Width() - 2));

                newImage[y][x] = Bilinear(origX, origY, image) + .0000005;
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
    //get scale factors from user
    double x_scale = 1.0;
    double y_scale = 1.0;

    Dialog input = Dialog("Scaling").Add(x_scale, "X Scale Factor", 0.0, 10.0).Add(y_scale, "Y Scale Factor", 0.0, 10.0);

    if (input.Show())
    {
        //create new image of scaled size
        Image newImage = Image(image.Height() * y_scale, image.Width() * x_scale);

        for (uint y = 0; y < newImage.Height(); y++)
        {
            //inverse mapping of Y coordinate
            uint origY = y / y_scale + 0.5;
            origY = min(origY, image.Height() - 1);

            for (uint x = 0; x < newImage.Width(); x++)
            {
                //inverse mapping of X coordinate
                uint origX = x / x_scale + 0.5;
                origX = min(origX, image.Width() - 1);

                newImage[y][x] = image[origY][origX];
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

uint ImageTransformations::Bilinear(double newx, double newy, Image &image)
{
    uint x = (int) newx;
    uint y = (int) newy;

    int h = image.Height();
    int w = image.Width();

    Point point11 = Point(x, y, image[x][y].Intensity());
    Point point12 = Point(x, y+1, image[x][y+1].Intensity());
    Point point21 = Point(x+1, y, image[x+1][y].Intensity());
    Point point22 = Point(x+1, y+1, image[x+1][y+1].Intensity());

    uint intensity = (point12.X() - newx) / (point12.X() - point11.X()) * point11.Intensity() +
                     (newx - point11.X()) / (point12.X() - point11.X()) * point12.Intensity();

    Point row1 = Point(newx, y, intensity);

    intensity = (point22.X() - newx) / (point22.X() - point21.X()) * point21.Intensity() +
                (newx - point21.X()) / (point22.X() - point21.X()) * point22.Intensity();

    Point row2 = Point(newx, y + 1, intensity);

    intensity = (row2.Y() - newy) / (row2.Y() - row1.Y()) * row1.Intensity() +
                (newy - row1.Y()) / (row2.Y() - row1.Y()) * row2.Intensity();

    return intensity;
}
