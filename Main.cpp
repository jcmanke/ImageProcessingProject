/**
    Authors: Adam Meaney, Joe Manke, Jonathan Tomes
    Description: This program is to demonstrate a few methods of geometric transformations.
        We cover affine transformations, showing both rotation and scaling. To do these, we use
        bilinear and nearest neighbor interpolation.

        Additionally, there is attempted code at rubber sheet transforms. This code is incomplete,
        but includes documentation of the intent and what was accomplished.
  */

#include "DIPProject.h"

int main( int argc, char *argv[] )
{
    ImageTransformations obj;
    ImageApp app( argc, argv );
    app.AddActions( &obj );
    return app.Start();
}
