#pragma once

#include <boost/asio.hpp>

#include <list>
#include <string>
#include <ctime>
#include <iostream>

#include "thread_wrapper.h"
#include "Processor.h"
#include "H12.h"

#define MAX_BLOCK_SIZE 50


using namespace boost::asio;
using namespace boost::asio::ip;

class StartTask{


    static void do_accept( io_service& ioservice
                          , tcp::acceptor& tcp_acceptor
                          , std::shared_ptr<ReceivingBulk> unit
                          , yield_context yield )
    {
        for (;;)
        {
            auto tcp_socket = std::make_unique<tcp::socket>( ioservice );
            tcp_acceptor.async_accept( *tcp_socket, yield );
            auto client = std::make_shared<Client>( std::move( tcp_socket ), ioservice, unit  );

            client->start();
        }
    }

    static void StopTheProgramm( io_service& ioservice, int sig )
    {
        //std::cerr << "Signal No " << sig << " appeared.\n";
        std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
        ioservice.stop();
    }

public:

    static void Start( int argc, char* argv[] )
    {
        size_t BlockSize;
        size_t Port;


        // checking incoming parameters

        if( argc != 3 )
        {
            throw std::invalid_argument( "Program parameters should be port and block size.\n" );
        }

        try
        {
            BlockSize = std::stoi( argv[2] );
            Port = std::stoi( argv[1] );
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

        if( Port > 65535 )
        {
            throw std::invalid_argument( "Program parameter ( port ) should be natural number in [1, 65535).\n " );
        }

        // initialization
        try
        {
            std::vector<std::exception_ptr> exc;

            // Инициализируем основной объект для получения и распределения данных
            auto receiver = std::make_shared<ReceivingBulk>( BlockSize, std::cout );

            // Инициализируем очередь для потоков под запись в файл
            auto LineToDisk    = std::make_shared<Queue>();
            auto LineToConsole = std::make_shared<Queue>();

            // Инициализируем два объекта, которые будут получать оповещения
            // и отправлять данные в потоки
            auto logger = std::make_shared<LoggingToFile>( *LineToDisk.get() );
            auto shower = std::make_shared<ShowOnDisplay>( *LineToConsole.get() );

            receiver->AddProcessor( logger );
            receiver->AddProcessor( shower );

            auto pr1 = std::make_shared<Processor>( LineToDisk, exc );
            auto pr2 = std::make_shared<Processor>( LineToDisk, exc );
            auto pr3 = std::make_shared<Processor>( LineToConsole, exc );

            // boost asio initialization part

            io_service ioservice;
            tcp::endpoint tcp_endpoint{tcp::v4(), Port};
            tcp::acceptor tcp_acceptor{ioservice, tcp_endpoint};
            tcp_acceptor.listen();

            boost::asio::signal_set signals( ioservice, SIGINT, SIGTERM );

            signals.async_wait( [&ioservice, &receiver]( const boost::system::error_code& err, int sig  )
                               {
                                   if( !err )
                                   {
                                       StopTheProgramm( ioservice, sig );
                                   }

                               } );

            spawn(ioservice, [&ioservice, &tcp_acceptor, &receiver]( yield_context yield)
                  {
                      do_accept( ioservice, tcp_acceptor, receiver, yield );
                  });

            {

                // going ahead with starting program
                std::vector<thread_wrapper> threads;
                threads.push_back( std::move( thread_wrapper( std::thread( &Processor::Loop, pr1 ) ) ) );
                threads.push_back( std::move( thread_wrapper( std::thread( &Processor::Loop, pr2 ) ) ) );
                threads.push_back( std::move( thread_wrapper( std::thread( &Processor::Loop, pr3 ) ) ) );

                try
                {
                    ioservice.run();
                }
                catch( std::exception& e )
                {
                    if( !receiver->CheckCommands() )
                    {
                        receiver->ClearCash();
                    }

                    LineToDisk->push( RequestType::Quit, std::string{}, 0 );
                    LineToDisk->push( RequestType::Quit, std::string{}, 0 );
                    LineToConsole->push( RequestType::Quit, std::string{}, 0 );

                    std::cout << e.what();

                    if( !exc.empty() )
                    {
                        std::rethrow_exception( exc.front() );
                    }
                    else
                    {
                        throw std::runtime_error( "" );
                    }
                }

                if( !receiver->CheckCommands() )
                {
                    receiver->ClearCash();
                }


                LineToDisk->push( RequestType::Quit, std::string{}, 0 );
                LineToDisk->push( RequestType::Quit, std::string{}, 0 );
                LineToConsole->push( RequestType::Quit, std::string{}, 0 );
            }


            std::cout << "Write statistics: "; pr1->PrintStatistics();
            std::cout << "Write statistics: "; pr2->PrintStatistics();
            std::cout << "Console statistics: "; pr3->PrintStatistics();

            receiver->PrintStatistics();
        }
        catch( const std::exception& e )
        {
            std::cerr << e.what();
            return;
        }
    }
};
