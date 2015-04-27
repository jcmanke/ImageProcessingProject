#include "DIPProject.h"
#include "point.h"

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
        // newX = cv + bw +avw + d
        // newY = gv + fw +evw + h
        // a = c1 - b1 + a1 -d1
        // b = b1 - a1
        // c = d1 - a1
        // d = a1
        // e = c2 - b2 + a2 - d2
        // f = b2 - a2
        // g = d2 - a2
        // h = a2

        QPoint pointA, pointB, pointC,pointD, pointE, pointF, pointG, pointH;
        pointA = stp->Point(0, 0).toPoint();
        pointB = stp->Point(0, stp->Cols()-1).toPoint();
        pointC = stp->Point(stp->Rows()-1, 0).toPoint();
        pointD = stp->Point(stp->Rows()-1, stp->Cols()-1).toPoint();
        pointE = ttp->Point(0,0).toPoint();
        pointF = ttp->Point(0, ttp->Cols()-1).toPoint();
        pointG = ttp->Point(ttp->Rows()-1, 0).toPoint();
        pointH = ttp->Point(ttp->Rows()-1, ttp->Cols()-1).toPoint();
        // I am confused on how to use the 8 points in the equations...
        // might have to ask Dr. Weiss tomorrow if no one can figure it out...

        return true;
    }

    return false;
}
