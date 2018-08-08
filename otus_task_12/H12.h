#ifndef HW12_H_INCLUDED
#define HW12_H_INCLUDED

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
#include <iostream>
#include <unordered_map>

#include "Client.h"
#include "Queue.h"

#define STRING_MAX_SIZE 50


namespace fs = std::experimental::filesystem;

enum class State{
    ProcessData
    , ProcessClientData
    , Wait
};


class ReceivingBulk;
class Client;

class IProcessor{

public:

    virtual ~IProcessor(){}

    virtual void process( const std::string&
                         , const size_t& ) = 0;
};

// ******************************************************************************

class ReceivingBulk{

    std::vector<std::weak_ptr<IProcessor>> processors;
    std::unordered_map<std::shared_ptr<Client>, std::tuple<std::string, size_t, size_t>> base;

    size_t AllLines = 0;
    size_t AllBlocks = 0;
    size_t AllCommands = 0;
    std::istream& in = std::cin;
    std::ostream& out = std::cout;

    int BlockSize = 1;

    std::string Commands{ "" };
    int Internal_N = BlockSize;
    size_t time = 0;
    State state{State::Wait};

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
    ReceivingBulk( size_t BlockSize, std::ostream& out = std::cout )
    : out( out ), BlockSize( BlockSize ), Internal_N( BlockSize ), time( 0 ) {}

    ReceivingBulk( const ReceivingBulk& ) = delete;
    ReceivingBulk& operator=( const ReceivingBulk& ) = delete;

    ~ReceivingBulk() {}

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

    bool CheckCommands()
    {
        return Commands.empty();
    }

// ******************************************************************************
    void ProcessNewData( std::string line
                        , std::shared_ptr<Client> client
                        , std::istream& in = std::cin
                        , std::ostream& out = std::cout )
    {
        ++AllLines;

        // разбираем входящую строку
        //std::cout << "Incoming message is " << line << "\n";

        if( line == "}" && std::get<1>( base[client] ) < 1 )
        {
            throw std::invalid_argument("Incorrect symbol '}' outside any block.\n");
        }

        if( line.size() > STRING_MAX_SIZE )
        {
            throw std::invalid_argument("Incorrect string (too long).\n");
        }

        //=========================  processing ===========================

        if( line == "{" )
        {
            std::get<1>( base[client] ) += 1;
            state = State::Wait;
        }
        else if( std::get<1>( base[client] ) )
        {
            // working with client as it has parenthesis previously
            state = State::Wait;

            if( line != "}" )
            {
                std::get<0>( base[client] ).append( line +", " );
                if( std::get<2>( base[client] ) == 0 )
                {
                    std::get<2>( base[client] ) = std::time( nullptr );
                }
            }
            else if( !(--std::get<1>( base[client] )) )
            {
                state = State::ProcessClientData;
            }
        }
        else if( --Internal_N == 0 )
        {
            // as there is no parenthesis, work with mixed commands
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
            time = 0;
        }
        else if( state == State::ProcessClientData )
        {
            SendNotification( std::get<0>( base[client] ), std::get<2>( base[client] ) );
            std::get<0>( base[client] ) = "";
            std::get<2>( base[client] ) = 0;
        }

        return;
    }


// ******************************************************************************
// ************              Clearing cash in case of stopping       ************
// ******************************************************************************

    void ClearCash()
    {
        if( !Commands.empty() )
        {
            SendNotification( Commands, time );
        }
    }



// ******************************************************************************
// ************              Processing a string     ****************************
// ******************************************************************************

    void SendNotification( std::string str, size_t time )
    {
        std::string data{( str.find_last_of(',') != std::string::npos ) ?
                                                            str.substr( 0, str.find_last_of(',') )
                                                            : str};
        ++AllBlocks;
        AllCommands += countCommands( data );

        if( fs::space( fs::current_path() ).free < sizeof( data.c_str() ) )
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

        return;
    }

// ******************************************************************************
//                        Functions to include / exclude clients
// ******************************************************************************
    void AddClient( std::shared_ptr<Client> client )
    {
        base[client] = std::make_tuple( "", 0, 0 );
    }

    void RemoveClient( std::shared_ptr<Client> client )
    {
        base.erase( client );
    }
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
#endif // HW12_H_INCLUDED
