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

#include "Queue.h"


class ReceivingBulk;

class IProcessor{

public:

    virtual ~IProcessor(){}

    virtual void process( const std::vector<std::string>&
                         , const size_t& ) = 0;
};

// ******************************************************************************

class ReceivingBulk{

    std::vector<std::weak_ptr<IProcessor>> processors;

    size_t AllLines = 0;
    size_t AllBlocks = 0;
    size_t AllCommands = 0;

    std::ostream& out;

public:

    ReceivingBulk() = delete;
    ReceivingBulk( std::ostream& out = std::cout ) : out( out ) {}

    void PrintStatistics() const
    {
        out << "Main thread: " << AllLines << " lines, "
        << AllBlocks << " blocks, " << AllCommands << " commands.\n";
    }

    void AddProcessor( std::shared_ptr<IProcessor> obj)
    {
        processors.push_back( obj );
    }

    void MainLoop(int N, std::istream& in = std::cin )
    {
        int BracesCounter = 0;
        std::vector<std::string> Commands;
        int Internal_N = N;
        size_t time = 0;

        for( std::string line; std::getline(in, line ); )
        {
            ++AllLines;
            if( line == "{" && !BracesCounter )
            {
                ++BracesCounter;

                if( !Commands.empty() )
                {
                    SendNotification( Commands, time );
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
                    SendNotification( Commands, time );
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
                SendNotification( Commands, time );
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
        }

        if( !Commands.empty() && !BracesCounter )
        {
            SendNotification( Commands, time );
        }
    }

    void SendNotification( const std::vector<std::string>& vec
                          , size_t& time )
    {
        ++AllBlocks;
        AllCommands += vec.size();

        for( auto it = processors.begin(); it != processors.end(); )
        {
            auto ptr = (*it).lock();
            if( ptr )
            {
                ptr->process( vec, time );
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

    Queue& line;

public:

    LoggingToFile( Queue& line_object ) : line( line_object ){}

    LoggingToFile( const LoggingToFile& ) = delete;

    void process( const std::vector<std::string>& data
                 , const size_t& time ) override
     {
         while( !line.push( RequestType::WriteToDisk, data, time ) )
         {
             std::cerr << "Queue is full.\n";
         }
     }
};

// ******************************************************************************



class ShowOnDisplay final : public IProcessor {

    std::vector<std::vector<std::string>> data;

    std::mutex PrintToConsoleMutex;
    std::condition_variable PrintQueue;

    std::ostream& out;

    size_t BlocksQuantity = 0;
    size_t CommandsQuantity = 0;

    std::thread th3;

    bool ToWorkOrNotToWork = true;
    bool ProcessData = true;



public:

    ShowOnDisplay( std::ostream& out = std::cout ) : out( out )
    {
        th3 = std::thread( &ShowOnDisplay::ImplementOutputConsole, this );
    }

    ShowOnDisplay( const ShowOnDisplay& ) = delete;

    ~ShowOnDisplay()
    {
        ProcessData = false;
        ToWorkOrNotToWork = false;

        {
            std::lock_guard<std::mutex> lock( PrintToConsoleMutex );
            data.push_back({""});
        }

        PrintQueue.notify_one();
        th3.join();
    }

    void process( const std::vector<std::string>& new_data
                 , const size_t& time ) override
     {
        {
            std::lock_guard<std::mutex> lock( PrintToConsoleMutex );
            if( DEBUG && !data.empty() )
            {
                std::cerr << "Data is not empty!\n";
            }

            data.push_back( new_data );
        }

        PrintQueue.notify_one();
     }

    void ImplementOutputConsole( )
    {
        while( ToWorkOrNotToWork )
        {
            std::unique_lock<std::mutex> lk( PrintToConsoleMutex );

            PrintQueue.wait( lk, [this]()
                            {
                                return ( !this->data.empty() );
                            });

            if( DEBUG )
            {
                std::cout << "Received parameters are: \n" <<
                            "ProcessData is " << ProcessData << "\n" <<
                            "data size is " << data.size() << "\n";
            }

            if( ProcessData || data.size() > 1 )
            {

                if( !ProcessData )
                {
                    data.erase( --data.end(), data.end() );
                }




                for( const auto& d : data )
                {
                    out << "bulk: ";

                    ++BlocksQuantity;

                    CommandsQuantity += d.size();

                    for( auto it = d.begin(); it != d.end(); ++it )
                    {
                        if( it != d.begin() )
                        {
                            out << ", ";
                        }

                        out << *it;
                    }
                    out << "\n";
                }


                data.erase( data.begin(), data.end() );
            }
        }
    }

    void PrintStatistics()
    {
        std::lock_guard<std::mutex> lock( PrintToConsoleMutex );

        out << "Log thread: " << BlocksQuantity << " blocks and "<< CommandsQuantity << " commands.\n";
    }
};
#endif // HW7_H_INCLUDED
