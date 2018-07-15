#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>
#include <string>
#include <ostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <algorithm>
#include <iterator>

#include "Queue.h"

class Processor{

    std::ostream& out = std::cout;
    bool tests = false;

    std::string data;
    size_t time;
    RequestType state;

     bool ToQuitOrNotToQuit = true;

    size_t BlocksQuantity{0};
    size_t CommandsQuantity{0};

    size_t countCommands()
    {
        size_t counter{0};
        size_t pos{0};

        if( data == "" ) { return 0;}

        while( data.find_first_of( ',', pos ) != std::string::npos )
        {
            ++counter;
            pos = data.find_first_of( ',', pos ) + 1;
        }

        return counter + 1;
    }

public:

    std::shared_ptr<Queue> line;
    std::vector<std::string> filesForTesting{};

    Processor( std::shared_ptr<Queue> queue_obj, std::ostream& out = std::cout, bool tests = false ) :
        line( queue_obj ), out( out ), tests( tests ), data({}), time( 0 ), state( RequestType::WriteToConsole ) {}

    Processor() = delete;
    Processor( const Processor& ) = delete;

    //**************************************************************

    void ImplementOutputConsole( )
    {
        ++BlocksQuantity;
        CommandsQuantity += countCommands();

        out << "bulk: " << data << "\n";
    }

    //**************************************************************

    void ImplementOutputFile( )
    {
        std::ofstream file;
        std::string file_name;

        size_t id = line->file_id++;

        file_name = "bulk" + std::to_string(time) + "_" + std::to_string( id ) + ".log";

        file.open( file_name, std::ios::out | std::ios::trunc );

        ++BlocksQuantity;
        CommandsQuantity += countCommands();

        file << "bulk: " << data;

        if( file.fail() )
        {
            throw std::runtime_error( "Can't write to file.\n" );
        }

        file.close();

        if( tests )
        {
            filesForTesting.push_back( file_name );
        }
    }

    //**************************************************************

    void PrintStatistics() const
    {
        out << BlocksQuantity << " blocks and " << CommandsQuantity << " commands.\n";
    }

    //**************************************************************

    size_t getNumberOfBlocks()
    {
        return BlocksQuantity;
    }

    size_t getNumberOfCommands()
    {
        return CommandsQuantity;
    }
    //**************************************************************

    void QuitProcessor()
    {
        ToQuitOrNotToQuit = false;
    }

    //**************************************************************

    void Loop( )
    {
        while( ToQuitOrNotToQuit )
        {
            line->pop( state, data, time );
            {
                switch( state )
                {
                    case RequestType::WriteToDisk :
                        ImplementOutputFile( );
                        break;

                    case RequestType::WriteToConsole :
                        ImplementOutputConsole();
                        break;

                    case RequestType::Quit :
                        ToQuitOrNotToQuit = false;
                        break;
                }
            }
        }
    }

    // **********************************************************
};

#endif // PROCESSOR_H_INCLUDED
