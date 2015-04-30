#include "DIPProject.h"
#include "point.h"
#include <cmath>

using namespace std;

double degreesToRadians(int angle)
{
    return angle * M_PI / 180;
}

bool ImageTransformations::Menu_Transformation_RotationByBilinearIntensity(Image &image)
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
        double centerY = image.Height() / 2.0;
        double centerX = image.Width() / 2.0;

        for (uint y = 0; y < newImage.Height() - 1; y++)
        {
            double diffY = y - centerY;

            for (uint x = 0; x < newImage.Width() - 1; x++)
            {
                double diffX = x - centerX;

                //inverse mapping
                double origX = diffX * cosine - diffY * sine + centerX;
                double origY = diffY * cosine + diffX * sine + centerY;

                if (origY >= 0 && origY < image.Height()-1 && origX >= 0 && origX < image.Width()-1)
                {
                    Pixel* pix = Bilinear(origX, origY, image);
                    newImage[y][x] = *pix;
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

    // Get scale factors
    Dialog input = Dialog("Scaling").Add(x_scale, "X Scale Factor", 0.0, 10.0).Add(y_scale, "Y Scale Factor", 0.0, 10.0);

    if (input.Show())
    {
        // Construct scaled image.
        Image newImage = Image(image.Height() * y_scale, image.Width() * x_scale);

        int width = newImage.Width();
        int origWidth = image.Width();

        // Leave off edges for safety
        for (uint y = 1; y < newImage.Height() - 1; y++)
        {
            double origY = y / y_scale;
            origY = min(origY, (double) (image.Height() - 2));

            for (uint x = 1; x < width - 1; x++)
            {
                double origX = x / x_scale;
                origX = min(origX, (double) (origWidth - 2));

                Pixel* pix = Bilinear(origX, origY, image);
                newImage[y][x].SetRGB(pix->Red(), pix->Green(), pix->Blue());
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

Pixel* ImageTransformations::Bilinear(double newx, double newy, Image &image)
{
    uint x = (int) newx;
    uint y = (int) newy;

    // Find the 4 nearest points to the mapped point
    Point point11 = Point(x, y, image[y][x]);
    Point point12 = Point(x, y+1, image[y+1][x]);
    Point point21 = Point(x+1, y, image[y][x+1]);
    Point point22 = Point(x+1, y+1, image[y+1][x+1]);

    // Weighted average of the points.
    double scaleFactor1 = (point21.X() - newx) / (point21.X() - point11.X());
    double scaleFactor2 = (newx - point11.X()) / (point21.X() - point11.X());

    uint intensity = scaleFactor1 * point11.Pix().Intensity();
    double inphase = scaleFactor1 * point11.Pix().Inphase();
    double quad = scaleFactor1 * point11.Pix().Quadrature();

    Pixel *pix = new Pixel();
    pix->SetIntensity(intensity + scaleFactor2 * point21.Pix().Intensity());
    pix->SetInphase(inphase + scaleFactor2 * point21.Pix().Inphase());
    pix->SetQuadrature(quad + scaleFactor2 * point21.Pix().Quadrature());

    Point row1 = Point(newx, y, *pix);


    // Get the second midpoint to average
    scaleFactor1 = (point22.X() - newx) / (point22.X() - point12.X());
    scaleFactor2 = (newx - point12.X()) / (point22.X() - point12.X());

    intensity = scaleFactor1 * point12.Pix().Intensity();
    inphase = scaleFactor1 * point12.Pix().Inphase();
    quad = scaleFactor1 * point12.Pix().Quadrature();

    Pixel* pix2 = new Pixel();
    pix2->SetIntensity(intensity + scaleFactor2 * point22.Pix().Intensity());
    pix2->SetInphase(inphase + scaleFactor2 * point22.Pix().Inphase());
    pix2->SetQuadrature(quad + scaleFactor2 * point22.Pix().Quadrature());

    Point row2 = Point(newx, y + 1, *pix2);

    // Average the 2 temporary pixels.
    scaleFactor1 = (row2.Y() - newy) / (row2.Y() - row1.Y());
    scaleFactor2 = (newy - row1.Y()) / (row2.Y() - row1.Y());

    intensity = scaleFactor1 * row1.Pix().Intensity();
    inphase = scaleFactor1 * row1.Pix().Inphase();
    quad = scaleFactor1 * row1.Pix().Quadrature();

    Pixel* pix3 = new Pixel();
    pix3->SetIntensity(intensity + scaleFactor2 * row2.Pix().Intensity());
    pix3->SetInphase(inphase + scaleFactor2 * row2.Pix().Inphase());
    pix3->SetQuadrature(quad + scaleFactor2 * row2.Pix().Quadrature());

    return pix3;
}
