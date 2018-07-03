#ifndef PROCESSOR_H_INCLUDED
#define PROCESSOR_H_INCLUDED

#include <iostream>
#include <thread>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>

#include "Queue.h"

class Processor{

    using QueueElement = std::tuple<RequestType, std::vector<std::string>, size_t>;

    Queue& line;
    std::ostream& out = std::cout;
    bool tests = false;

    QueueElement data;
    size_t BlocksQuantity = 0;
    size_t CommandsQuantity = 0;




    std::string CheckIfFileExists( const size_t& time, int additional = 0 )
    {
        std::fstream file;

        std::stringstream file_name;

        file_name << "bulk" << time << "_" << std::this_thread::get_id() << additional << ".log";

        std::string fl = file_name.str();

        if( access( fl.c_str(), F_OK ) != -1 )
        {
            return CheckIfFileExists( time, additional + 1 );
        }

        return fl;
    }

public:

    std::vector<std::string> filesForTesting;

    Processor( Queue& queue_obj, std::ostream& out = std::cout, bool tests = false ) :
        line( queue_obj ), out( out ), tests( tests ) {}

    Processor() = delete;
    Processor( const Processor& ) = delete;

    //**************************************************************

    void ImplementOutputFile( )
    {
        std::string file_name = CheckIfFileExists( std::get<2>( data ) );

        std::fstream file;

        file.open( file_name, std::ios::out );

        file << "bulk: ";

        auto Commands = std::get<1>( data );

        ++BlocksQuantity;
        CommandsQuantity += Commands.size();

        for( auto it = Commands.begin(); it != Commands.end(); ++it )
        {
            if( it != Commands.begin() )
            {
                file << ", ";
            }

            file << *it;
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
        out << "Writer thread: " << BlocksQuantity << " blocks and " << CommandsQuantity << " commands.\n";
    }

    //**************************************************************

    void Loop( )
    {
        bool ToQuitOrNotToQuit = true;

        while( ToQuitOrNotToQuit )
        {
            data = line.pop();

            switch( std::get<0>( data) )
            {
                case RequestType::WriteToDisk :
                    ImplementOutputFile( );
                    break;
                case RequestType::Quit :
                    ToQuitOrNotToQuit = false;
                    break;
            }
        }
    }

    // **********************************************************
};

#endif // PROCESSOR_H_INCLUDED
