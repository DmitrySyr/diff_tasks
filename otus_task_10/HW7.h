#ifndef HW7_H_INCLUDED
#define HW7_H_INCLUDED

#include <vector>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>
#include <exception>
#include <experimental/filesystem>

#include "Queue.h"

#define STRING_MAX_SIZE 50
#define MAX_BLOCK_SIZE 50

namespace fs = std::experimental::filesystem;

enum class State{
    ProcessData
    , Wait
};


class ReceivingBulk;

class IProcessor{

public:

    virtual ~IProcessor(){}

    virtual void process( const std::string&
                         , const size_t& ) = 0;
};

// ******************************************************************************

class ReceivingBulk{

    std::vector<std::weak_ptr<IProcessor>> processors;

    size_t AllLines = 0;
    size_t AllBlocks = 0;
    size_t AllCommands = 0;
    std::ostream& out;

    size_t countCommands( std::string& data )
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

    ReceivingBulk() = delete;
    ReceivingBulk( std::ostream& out = std::cout ) : out( out ) {}

    ReceivingBulk( const ReceivingBulk& ) = delete;
    ReceivingBulk& operator=( const ReceivingBulk& ) = delete;

    void PrintStatistics() const
    {
        out << "Main thread: " << AllLines << " lines, "
        << AllBlocks << " blocks, " << AllCommands << " commands.\n";
    }

    size_t GetNumberOfSubscribers()
    {
        return processors.size();
    }

    void AddProcessor( std::shared_ptr<IProcessor> obj)
    {
        processors.push_back( obj );
    }

    void MainLoop(std::string N, std::istream& in = std::cin, std::ostream& out = std::cout )
    {
        int BlockSize;

        try
        {
            BlockSize = std::stoi( N );
        }
        catch(const std::exception& e)
        {
            std::stringstream ss;
            ss << "Program parameter (commands' block size) should be natural number in [1, ";
            ss << MAX_BLOCK_SIZE;
            ss << ")\n";

            throw std::invalid_argument( ss.str().c_str() );
        }

        if( BlockSize < 1 || BlockSize > MAX_BLOCK_SIZE )
        {
            std::stringstream ss;
            ss << "Program parameter (commands' block size) should be natural number in [1, ";
            ss << MAX_BLOCK_SIZE;
            ss << ")\n";

            throw std::invalid_argument( ss.str().c_str() );
        }

        int BracesCounter = 0;
        std::string Commands;
        int Internal_N = BlockSize;
        size_t time = 0;
        State state{State::Wait};


        for( std::string line; std::getline(in, line ); )
        {
            ++AllLines;
            // проверка валидности строки
            if( line.size() > STRING_MAX_SIZE )
            {
                throw std::invalid_argument("Incorrect string (too long).\n");
            }

            // разбираем входящую строку
            if( line == "{" )
            {
                if( !BracesCounter && Commands != "" )
                {
                    state = State::ProcessData;
                }
                else
                {
                    state = State::Wait;
                }

                ++BracesCounter;
            }
            else if( line == "}" )
            {
                --BracesCounter;

                if( BracesCounter == 0 && Commands != "" )
                {
                    state = State::ProcessData;
                }
                else if( BracesCounter < 0 )
                {
                    throw std::invalid_argument("Incorrect symbol '}' outside any block.\n");
                }
                else
                {
                    state = State::Wait;
                }
            }
            else if(!BracesCounter && --Internal_N == 0 )
            {
                Commands.append( line + ", " );

                if( !time )
                {
                    time = std::time( nullptr );
                }

                state = State::ProcessData;
            }
            else
            {
                if( !time )
                {
                    time = std::time( nullptr );
                }

                Commands.append( line + ", " );
                state = State::Wait;
            }

            if( state == State::ProcessData )
            {
                SendNotification( Commands, time );
                Commands.clear();
                Internal_N = BlockSize;
            }
        }

        if( !Commands.empty() && !BracesCounter )
        {
            SendNotification( Commands, time );
        }
    }

    void SendNotification( const std::string& str
                          , size_t time )
    {
        std::string data{( str.find_last_of(',') != std::string::npos ) ?
                                                            str.substr( 0, str.find_last_of(',') )
                                                            : str};
        ++AllBlocks;
        AllCommands += countCommands( data );

        if( fs::space( fs::current_path() ).free < sizeof( str.c_str() ) )
        {
            throw std::runtime_error( "There is no free space available." );
        }

        for( auto it = processors.begin(); it != processors.end(); )
        {
            auto ptr = (*it).lock();
            if( ptr )
            {
                ptr->process( data, time );
                ++it;
            }
            else
            {
                it = processors.erase( it );
            }
        }
        time = 0;
    }

// ******************************************************************************

};


// ******************************************************************************

class LoggingToFile final : public IProcessor {

    Queue& line;

public:

    LoggingToFile( Queue& line_object ) : line( line_object ){}

    LoggingToFile( const LoggingToFile& ) = delete;

    void process( const std::string& data
                 , const size_t& time ) override
     {
         line.push( RequestType::WriteToDisk, data, time );
     }
};

// ******************************************************************************



class ShowOnDisplay final : public IProcessor {

    Queue& line;

public:

    ShowOnDisplay( Queue& line_object ) : line( line_object ){}

    ShowOnDisplay( const ShowOnDisplay& ) = delete;

    ~ShowOnDisplay(){}

    void process( const std::string& data
                 , const size_t& time ) override
     {
         line.push( RequestType::WriteToConsole, data, time );
     }
};
#endif // HW7_H_INCLUDED
