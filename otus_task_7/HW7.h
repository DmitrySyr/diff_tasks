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
#include <unistd.h>
#include <ios>

#define STRING_MAX_SIZE 50
#define MAX_BLOCK_SIZE 200

enum class State{
    ProcessData
    , Wait
};

class ReceivingBulk;

class IProcessor{

public:

    virtual ~IProcessor(){}

    virtual void process( const std::vector<std::string>&
                         , const size_t&
                         , std::ostream& ) = 0;
};

// ******************************************************************************

class ReceivingBulk{

    std::vector<std::weak_ptr<IProcessor>> processors;

public:

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
        std::vector<std::string> Commands;
        int Internal_N = BlockSize;
        size_t time = 0;
        State state{State::Wait};


        for( std::string line; std::getline(in, line ); )
        {
            // проверка валидности строки
            if( line.size() > STRING_MAX_SIZE )
            {
                throw std::invalid_argument("Incorrect command (too long).\n");
            }

            // разбираем входящую строку
            if( line == "{" )
            {
                if( !BracesCounter && !Commands.empty() )
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

                if( BracesCounter == 0 && !Commands.empty() )
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
                Commands.push_back( line );

                // в случае блока из одной команды, мы должны записать время здесь
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

                Commands.push_back( line );
                state = State::Wait;
            }

            if( state == State::ProcessData )
            {
                SendNotification( Commands, time, out );
                Commands.erase( Commands.begin(), Commands.end() );
                Internal_N = BlockSize;
                time = 0;
            }
        }

        if( !Commands.empty() && !BracesCounter )
        {
            SendNotification( Commands, time, out );
        }
    }

    void SendNotification( const std::vector<std::string>& vec
                          , size_t& time
                          , std::ostream& out)
    {
        for( auto it = processors.begin(); it != processors.end(); )
        {
            auto ptr = (*it).lock();
            if( ptr )
            {
                ptr->process( vec, time, out );
                ++it;
            }
            else
            {
                it = processors.erase( it );
            }
        }
    }
};


// ******************************************************************************

class LoggingToFile final : public IProcessor {


public:

    std::vector<std::string> files;

    LoggingToFile(){}

    LoggingToFile( const LoggingToFile& ) = delete;

    void process( const std::vector<std::string>& data
                 , const size_t& time
                 , std::ostream& out ) override
    {
        int add = 0;
        std::fstream file;

        std::string file_name = "bulk" + std::to_string(time) + "_" + std::to_string( add ) + ".log";

        file.open( file_name, std::ios::in );

        while( std::fstream file{ file_name, std::ios::in } )
        {
            file.close();
            ++add;
            file_name = "bulk" + std::to_string(time) + "_" + std::to_string( add ) + ".log";
        }

        file.close();


        if( std::fstream file{ file_name, std::ios::out } )
        {
            files.push_back( file_name );

            file << "bulk: ";

            for( auto it = data.begin(); it != data.end(); ++it )
            {
                if( it != data.begin() )
                {
                    file << ", ";
                }

                file << *it;
            }

            file << std::endl;

            if( file.fail() )
            {
                throw std::invalid_argument( "Can not write to file.\n" );
            }
        }
    }
};

// ******************************************************************************

class ShowOnDisplay final : public IProcessor {
public:
    ShowOnDisplay(){}

    ShowOnDisplay( const ShowOnDisplay& ) = delete;

    void process( const std::vector<std::string>& data
                 , const size_t& time
                 , std::ostream& out ) override
    {
        out << "bulk: ";

        for( auto it = data.begin(); it != data.end(); ++it )
        {
            if( it != data.begin() )
            {
                out << ", ";
            }

            out << *it;
        }
        out << "\n";
    }
};
#endif // HW7_H_INCLUDED
