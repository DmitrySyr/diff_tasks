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
        std::cout << "The only parameter should be digital(natural) which is commands'"
                     " block size.\n";
        return 1;
    }

    int N;

    try
    {
        N = std::stoi( argv[1] );
    }
    catch(const std::exception& e)
    {
        std::cout << "Program parameter should be a natural number.\n";
    }

    if( N < 1 )
    {
        std::cerr << "Program parameter should be not less than one.\n";
        return 1;
    }

    // Инициализируем основной объект для получения и распределения данных
    ReceivingBulk receiver( std::cout );

    // Инициализируем очередь для потоков под запись в файл
    auto LineToDisk    = std::make_shared<Queue>( 5 );

    // Инициализируем два объекта, которые будут получать оповещения
    // и отправлять данные в потоки
    auto logger = std::make_shared<LoggingToFile>( *LineToDisk.get() );
    auto shower = std::make_shared<ShowOnDisplay>( );

    receiver.AddProcessor( logger );
    receiver.AddProcessor( shower );


    auto pr1 = std::make_shared<Processor>( *LineToDisk.get() );
    auto pr2 = std::make_shared<Processor>( *LineToDisk.get() );

    std::thread th1( &Processor::Loop, pr1.get() );
    std::thread th2( &Processor::Loop, pr2.get() );

    try
    {
        receiver.MainLoop( N, std::cin );
    }
    catch( const std::exception& e )
    {
        std::cout << e.what();
    }

    LineToDisk->push( RequestType::Quit, std::vector<std::string>{}, 0 );
    LineToDisk->push( RequestType::Quit, std::vector<std::string>{}, 0 );

    th1.join();
    th2.join();

    // ставим паузу в выводе статистик, чтобы выводились последовательно
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    pr1->PrintStatistics();
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    pr2->PrintStatistics();
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    shower->PrintStatistics();
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    receiver.PrintStatistics();

    return 0;
}
