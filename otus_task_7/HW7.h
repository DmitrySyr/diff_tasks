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

#define STRING_MAX_SIZE 50

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

    void MainLoop(int N, std::istream& in = std::cin, std::ostream& out = std::cout )
    {
        int BracesCounter = 0;
        std::vector<std::string> Commands;
        int Internal_N = N;
        size_t time = 0;
        State state{State::Wait};


        for( std::string line; std::getline(in, line ); )
        {
            // проверка валидности строки
            if( line.size() > STRING_MAX_SIZE )
            {
                throw std::invalid_argument("Incorrect string (too long).\n");
            }
            else if( line.size() > 1 &&
                    ( line.find_first_of('{') != std::string::npos
                     || line.find_first_of('}') != std::string::npos ) )
            {
                throw std::invalid_argument("Incorrect command (begins with '{' or '}').\n");
            }
            else if( line == "" )
            {
                throw std::invalid_argument("Incorrect command (empty).\n");
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
                Internal_N = N;
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
        time = 0;
    }
};


// ******************************************************************************

class LoggingToFile final : public IProcessor {

    std::string CheckIfFileExists( const size_t& time, size_t add = 0 )
    {
        std::fstream file;

        std::string file_name = "bulk" + std::to_string(time) + "_" + std::to_string( add ) + ".log";

        //file.open( file_name, std::ios::in );

        if( access( file_name.c_str(), F_OK ) != -1 )
        {
            return CheckIfFileExists( time, ++add );
        }

        return file_name;
    }



public:

    std::vector<std::string> files;

    LoggingToFile(){}

    LoggingToFile( const LoggingToFile& ) = delete;

    void process( const std::vector<std::string>& data
                 , const size_t& time
                 , std::ostream& out ) override
    {
        std::string file_name = CheckIfFileExists( time );

        std::fstream file;

        file.open( file_name, std::ios::out );

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

        file.close();

        if( file.bad() )
        {
            throw std::system_error( 1, std::iostream_category(), "Can not write to file.\n");
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
