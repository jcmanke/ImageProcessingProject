#include "DIPProject.h"

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
        // a = c1 - b1 + a -d1
        // b = b1 - a1
        // c = d1 - a1
        // d = a1
        // e = c2 - b2 + a2 - d2
        // f = b2 - a2
        // g = d2 - a2
        // h = a2



        return true;
    }

    return false;
}
