#include <iostream>
#include <exception>

#include "StartTask.h"

using namespace std;

int main( int argc, char* argv[] )
{
    try
    {
        StartTask::Start( argc, argv );
    }
    catch( std::exception& e )
    {
        std::cerr << e.what();
        return 1;
    }


    return 0;
}
