#pragma once

#define MAX_BLOCK_SIZE 50

#include <map>
#include <atomic>

#include "HW11.h"
#include "Processor.h"
#include "async.h"

class Handler{

    using handle_t = void *;

    std::atomic_flag init_flag = ATOMIC_FLAG_INIT;

    std::shared_ptr<Queue> LineToDisk;
    std::shared_ptr<Queue> LineToConsole;

    // Инициализируем два объекта, которые будут получать оповещения
    // и отправлять данные в потоки
    std::shared_ptr<LoggingToFile> logger;
    std::shared_ptr<ShowOnDisplay> shower;

    std::shared_ptr<Processor> pr1;
    std::shared_ptr<Processor> pr2;
    std::shared_ptr<Processor> pr3;

    std::thread th1;
    std::thread th2;
    std::thread th3;

    void Init( std::ostream& out, bool test )
    {
        if( init_flag.test_and_set() == 0 )
        {
            // Инициализируем очередь для потоков под запись в файл
            LineToDisk    = std::make_shared<Queue>();
            LineToConsole = std::make_shared<Queue>();

            // Инициализируем два объекта, которые будут получать оповещения
            // и отправлять данные в потоки
            logger = std::make_shared<LoggingToFile>( *LineToDisk.get() );
            shower = std::make_shared<ShowOnDisplay>( *LineToConsole.get() );

            pr1 = std::make_shared<Processor>( LineToDisk, exc, out, test );
            pr2 = std::make_shared<Processor>( LineToDisk, exc, out, test );
            pr3 = std::make_shared<Processor>( LineToConsole, exc, out );

            th1 = std::thread( &Processor::Loop, pr1 );
            th2 = std::thread( &Processor::Loop, pr2 );
            th3 = std::thread( &Processor::Loop, pr3 );
        }
    }

    Handler( const Handler& ) {}
    Handler& operator=( const Handler& ) {}

    std::atomic<size_t> index{1};
    std::atomic<bool> state{1};

    std::map<size_t, std::shared_ptr<ReceivingBulk>> handlers;

    //size_t* index_ptr = nullptr;

public:

    std::vector<std::exception_ptr> exc;

    Handler( std::ostream& out = std::cout, bool tests = false )
    {
        Init( out, tests );
    }

    ~Handler()
    {
        LineToDisk->push( RequestType::Quit, std::string{}, 0 );
        LineToDisk->push( RequestType::Quit, std::string{}, 0 );
        LineToConsole->push( RequestType::Quit, std::string{}, 0 );

        if( th1.joinable() ) { th1.join();}
        if( th2.joinable() ) { th2.join();}
        if( th3.joinable() ) { th3.join();}
    }

    size_t getIndex( size_t bulk )
    {
        if( bulk < 1 || bulk > MAX_BLOCK_SIZE )
        {
            std::stringstream ss;
            ss << "Program parameter (commands' block size) should be natural number in [1, ";
            ss << MAX_BLOCK_SIZE;
            ss << ")\n";

            throw std::invalid_argument( ss.str().c_str() );
        }

        auto local_index = index++;

        handlers[local_index] = std::make_shared<ReceivingBulk>( bulk );
        handlers[local_index] -> AddProcessor( logger );
        handlers[local_index] -> AddProcessor( shower );

        return local_index;
    }

    void proceed( size_t indexForProceed, const char *data, std::size_t size__ )
    {
        if( handlers.find( indexForProceed ) == handlers.end() )
        {
            throw std::runtime_error( "Handler didn't found.");
        }

        handlers[indexForProceed]->ProcessNewData( data, size__ );
    }

    void printStatistics( size_t indexForProceed )
    {
        if( handlers.find( indexForProceed ) == handlers.end() )
        {
            throw std::runtime_error( "Handler didn't found.");
        }

        handlers[indexForProceed]->PrintStatistics();
    }

    void disconnect( handle_t handler )
    {
        auto indexForProceed = reinterpret_cast<size_t>( handler );
        handlers.erase( indexForProceed );
    }

    void setStateBad()
    {
        state.store( false );
    }

    bool good()
    {
        return ( state.load() == true );
    }

    void clear()
    {
        state.store( true );
    }

    std::vector<std::string> getFileNamesForTest()
    {
        std::vector<std::string> fileNames{};

        for( const auto& name : pr1->filesForTesting )
        {
            fileNames.push_back( name );
        }

        for( const auto& name : pr2->filesForTesting )
        {
            fileNames.push_back( name );
        }

        return fileNames;
    }


};
