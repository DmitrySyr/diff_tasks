#define DEBUG 0

#include <iostream>
#include <cassert>
#include <sstream>
#include <thread>

#include "HW7.h"
#include "Queue.h"
#include "Processor.h"



int main( int argc, char* argv[] )
{

    if( argc != 2 )
    {
        std::cout << "Program parameter (commands' block size) should be natural number.\n";
        return 1;
    }

    try
    {
        std::vector<std::exception_ptr> exc;

        // Инициализируем основной объект для получения и распределения данных
        auto receiver = std::make_shared<ReceivingBulk>( std::cout );

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

        std::thread th1( &Processor::Loop, pr1 );
        std::thread th2( &Processor::Loop, pr2 );
        std::thread th3( &Processor::Loop, pr3 );

        try
        {
            receiver->MainLoop( argv[1], std::cin );
        }
        catch( std::exception& e )
        {
            LineToDisk->push( RequestType::Quit, std::string{}, 0 );
            LineToDisk->push( RequestType::Quit, std::string{}, 0 );
            LineToConsole->push( RequestType::Quit, std::string{}, 0 );

            th1.join();
            th2.join();
            th3.join();

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


        LineToDisk->push( RequestType::Quit, std::string{}, 0 );
        LineToDisk->push( RequestType::Quit, std::string{}, 0 );
        LineToConsole->push( RequestType::Quit, std::string{}, 0 );

        th1.join();
        th2.join();
        th3.join();

        std::cout << "Write statistics: "; pr1->PrintStatistics();
        std::cout << "Write statistics: "; pr2->PrintStatistics();
        std::cout << "Console statistics: "; pr3->PrintStatistics();

        receiver->PrintStatistics();
    }
    catch( const std::exception& e )
    {
        std::cout << e.what();
        return 1;
    }

    return 0;
}
