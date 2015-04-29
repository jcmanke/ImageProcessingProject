#include "DIPProject.h"
#include "point.h"
#include <cmath>

bool ImageTransformations::Menu_Transformation_GeneralWarp(Image &image)
{
    TPDialog dlg(image.ToQImage(), 2, 2 );

    if(dlg.exec())
    {
        TiePoints *stp = dlg.Source();
        TiePoints *ttp = dlg.Target();
        // Only getting the four courner Tie points of source
        // and target image.

        // equations!
        // Square to quadrilateral
        // using a matrix A
        // [x', y', w'] = [u, v, w]* A
        // x = x'/w' = (a11*u + a21*v + a31)/(a13*u + a23*v + a33)
        // y = y'/w' = (a12*u + a22*v + a32)/(a12*u + a23*v + a33)
        // some special terms:
        // (x,y are target image coordinates, u,v are source image coords)
        // DeltaX1 = x1 - x2; DeltaX2 = x3-x2; DeltaX3 = x0 - x1 + x2 - x3
        // DeltaY1 = y1 - y2; DeltaY2 = y3-y2; DeltaY3 = y0 - y1 + y2 - y3
        // if DelataX3 == 0 and DeltaY3 == 0 than
        // a33 = 1
        // a11 = x1 - x0
        // a21 = x2 - x1
        // a31 = x0
        // a12 = y1 -y0
        // a22 = y2 - y1
        // a32 = y0
        // a13 = 0
        // a23 = 0
        // else
        // a33 = 1
        // a13 = DeltaX3*DeltaY2 - DeltaX2*DeltaY3/ DeltaX1*DeltaY2 - DeltaX2*DeltaY1
        // a23 = DeltaX1*DeltaY3 - DeltaY1*DeltaX3/DeltaX1*DeltaY2 - DeltaX2*DeltaY1
        // a11 = x1 - x0 + a13*x1
        // a21 = x3 - x0 + a23*x3
        // a31 = x0
        // a12 = y1 - y0 +a13*y1
        // a22 = y3 - y0 +a23*y3
        // a32 = y0
        // For Quaadrilateral to Square:
        // do S to Q then find the Adjoint of A
        // For Q to Q:
        // Do Case 2  and then flow into Case 1

        // (U0,V0);(U1,V1);(U2,V2);(U3,V3); (X0,Y0);(X1,Y1);(X2,Y2);(X3,Y3)
        QPoint pointA, pointB, pointC,pointD, pointE, pointF, pointG, pointH;
        pointA = stp->Point(0, 0).toPoint();
        pointB = stp->Point(0, stp->Cols()-1).toPoint();
        pointC = stp->Point(stp->Rows()-1, 0).toPoint();
        pointD = stp->Point(stp->Rows()-1, stp->Cols()-1).toPoint();
        pointE = ttp->Point(0,0).toPoint();
        pointF = ttp->Point(0, ttp->Cols()-1).toPoint();
        pointG = ttp->Point(ttp->Rows()-1, 0).toPoint();
        pointH = ttp->Point(ttp->Rows()-1, ttp->Cols()-1).toPoint();
        double deltaX1, deltaX2, deltaX3, deltaY1, deltaY2, deltaY3;
        deltaX1 = pointF.x() - pointG.x();
        deltaX2 = pointH.x() - pointG.x();
        deltaX3 = pointE.x() - pointF.x() + pointG.x() - pointH.x();
        deltaY1 = pointF.y() - pointG.y();
        deltaY2 = pointH.y() - pointG.y();
        deltaY3 = pointE.y() - pointF.y() + pointG.y() - pointH.y();

        double A[3][3];
        A[2][2] = 1;
        if(deltaX3 == 0 && deltaY3 == 0)
        {
            A[0][2] = 0;
            A[1][2] = 0;
        }
        else
        {
            A[0][2] = (deltaX3*deltaY2 - deltaX2*deltaY3)/ (deltaX1*deltaY2 - deltaX2*deltaY1);
            A[1][2] = deltaX1*deltaY3 - deltaY1*deltaX3 / (deltaX1*deltaY2 - deltaX2*deltaY1);
        }
        A[0][0] = pointF.x() - pointE.x() + A[0][2]*pointF.x();
        A[1][0] = pointG.x() - pointF.x() + A[1][2]*pointG.x();
        A[2][0] = pointE.x();
        A[0][1] = pointF.y() - pointE.y() + A[0][2]*pointF.y();
        A[1][1] = pointG.y()  - pointF.y()+ A[1][2]*pointG.y();
        A[2][1] = pointE.y();

        if (image.Height() > image.Width())
        {
            if(ScaleToSquare(image, image.Height()/((double)image.Width()), 1.0) == false)
            {
                return false;
            }
        }
        else if(image.Width() > image.Height())
        {
            if(ScaleToSquare(image, 1.0, image.Width()/((double)image.Height())) == false)
            {
                return false;
            }
        }

        Image newImage = Image(image.Height(), image.Width());

        for(uint y = 0; y < image.Height(); y++)
        {
            for(uint x = 0; x < image.Width(); x++)
            {
                uint newX = (A[0][0]*x + A[1][0]*y+A[2][0])/
                            (A[0][2]*x + A[1][2]*y+A[2][2]);
                uint newY = (A[0][1]*x + A[1][1]*y+A[2][1])/
                            (A[0][2]*x + A[1][2]*y+A[2][2]);
                Pixel *pix = Bilinear(newX, newY, image);
                newImage[newX][newY].SetRGB(pix->Red(), pix->Green(), pix->Blue());
            }
        }

        image = newImage;
        return true;
    }

    return false;
}

bool ImageTransformations::ScaleToSquare(Image &image, double x_scale, double y_scale)
{

    //create new image of scaled size
    Image newImage = Image(image.Height() * y_scale, image.Width() * x_scale);

    for (uint y = 0; y < newImage.Height(); y++)
    {
        //inverse mapping of Y coordinate
        uint origY = y / y_scale + 0.5;
        origY = fmin(origY, image.Height() - 1);

        for (uint x = 0; x < newImage.Width(); x++)
        {
            //inverse mapping of X coordinate
            uint origX = x / x_scale + 0.5;
            origX = fmin(origX, image.Width() - 1);

            newImage[y][x] = image[origY][origX];
        }
    }

    image = newImage;
    return true;
}
