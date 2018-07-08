#include <iostream>
#include <cassert>
#include <sstream>

#include "HW7.h"

using namespace std;

int main( int argc, char* argv[] )
{

    if( argc != 2 )
    {
        std::cout << "Program parameter (commands' block size) should be natural number in [1, 200)\n";
        return 1;
    }

    int N;

    try
    {
        N = std::stoi( argv[1] );
    }
    catch(const std::exception& e)
    {
        std::cerr << "Program parameter (commands' block size) should be natural number in [1, 200)\n";
        return 1;
    }

    if( N < 1 || N > 199 )
    {
        std::cerr << "Program parameter (commands' block size) should be natural number in [1, 200)\n";
        return 1;
    }


    try
    {
        ReceivingBulk receiver;

        auto logger = std::make_shared<LoggingToFile>( );
        auto shower = std::make_shared<ShowOnDisplay>( );

        receiver.AddProcessor( logger );
        receiver.AddProcessor( shower );


        receiver.MainLoop( N, std::cin, std::cout );
    }
    catch( const std::exception& e )
    {
        std::cout << e.what();
    }

    return 0;
}
