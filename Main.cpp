#include "DIPProject.h"

int main( int argc, char *argv[] )
{
    ImageTransformations obj;
    ImageApp app( argc, argv );
    app.AddActions( &obj );
    return app.Start();
}
