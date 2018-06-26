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

class ReceivingBulk;

class IProcessor{

public:

    virtual ~IProcessor(){}

    virtual void process( const std::vector<std::string>&
                         , const size_t&
                         , std::istream&
                         , std::ostream& ) = 0;
};

// ******************************************************************************

class ReceivingBulk{

    std::vector<std::weak_ptr<IProcessor>> processors;

public:

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

        for( std::string line; std::getline(in, line ); )
        {
            if( line == "{" && !BracesCounter )
            {
                ++BracesCounter;

                if( !Commands.empty() )
                {
                    SendNotification( Commands, time, in, out );
                }

                Commands.erase( Commands.begin(), Commands.end() );
            }
            else if( line == "{" )
            {
                ++BracesCounter;
            }
            else if( line == "}" )
            {
                --BracesCounter;
                if( BracesCounter == 0 && !Commands.empty() )
                {
                    SendNotification( Commands, time, in, out );
                    Commands.erase( Commands.begin(), Commands.end() );
                    Internal_N = N;
                }
                else if( BracesCounter < 0 )
                {
                    throw std::exception();
                }
            }
            else if(!BracesCounter && --Internal_N == 0 )
            {
                Commands.push_back( line );
                SendNotification( Commands, time, in, out );
                Commands.erase( Commands.begin(), Commands.end() );
                Internal_N = N;
            }
            else
            {
                if( !time )
                {
                    time = GetCurrentTime();
                }
                Commands.push_back( line );
            }

            //std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
        }

        if( !Commands.empty() && !BracesCounter )
        {
            SendNotification( Commands, time, in, out );
        }
    }

    void SendNotification( const std::vector<std::string>& vec
                          , size_t& time
                          , std::istream& in
                          , std::ostream& out)
    {
        for( auto it = processors.begin(); it != processors.end(); )
        {
            auto ptr = (*it).lock();
            if( ptr )
            {
                ptr->process( vec, time, in, out );
                ++it;
            }
            else
            {
                it = processors.erase( it );
            }
        }
        time = 0;
    }

    size_t GetCurrentTime()
    {
        return std::time( nullptr );
    }
};


// ******************************************************************************

class LoggingToFile final : public IProcessor {

    bool CheckIfFileExists( std::string fileName ) const
    {
        std::fstream file;

        file.open( fileName, std::ios::in );

        if( file )
        {
            std::cout << "File " << fileName << " is already exists.\n";
            file.close();

            return 1;
        }

        return 0;
    }



public:

    std::vector<std::string> files;

    LoggingToFile(){}

    LoggingToFile( const LoggingToFile& ) = delete;

    void process( const std::vector<std::string>& data
                 , const size_t& time
                 , std::istream& in
                 , std::ostream& out ) override
    {
        std::string file_name = "bulk" + std::to_string(time) + ".log";

        if ( CheckIfFileExists( file_name ) )
        {
            return;
        }

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
    }
};

// ******************************************************************************

class ShowOnDisplay final : public IProcessor {
public:
    ShowOnDisplay(){}

    ShowOnDisplay( const ShowOnDisplay& ) = delete;

    void process( const std::vector<std::string>& data
                 , const size_t& time
                 , std::istream& in
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
