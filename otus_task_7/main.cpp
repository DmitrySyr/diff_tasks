#include <iostream>
#include <cassert>
#include <sstream>

#include "HW7.h"

using namespace std;

int main( int argc, char* argv[] )
{

    if( argc != 2 )
    {
        std::cout << "The only parameter should be digital(natural) which is commands'"
                     " block size.\n";
        return 1;
    }

    try
    {
        int N = std::stoi( argv[1] );
    }
    catch(const std::exception& e)
    {
        std::cout << "Program parameter should be a natural.\n";
    }

    ReceivingBulk receiver;

    auto logger = std::make_shared<LoggingToFile>( );
    auto shower = std::make_shared<ShowOnDisplay>( );

    receiver.AddProcessor( logger );
    receiver.AddProcessor( shower );

    try
    {
        receiver.MainLoop( 2, std::cin, std::cout );
    }
    catch( const std::exception& e )
    {
        std::cout << e.what();
    }

    return 0;
}
