#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include "test_header.h"

BOOST_AUTO_TEST_SUITE ( test_output )

auto test_helper_logger( std::istream& in, std::ostream& out, std::string n )
{
    std::vector<std::exception_ptr> exc;

    auto receiver = std::make_shared<ReceivingBulk>( std::cout );

    // Инициализируем очередь для потоков под запись в файл
    auto LineToDisk    = std::make_shared<Queue>();
    auto LineToConsole = std::make_shared<Queue>();

    // Инициализируем два объекта, которые будут получать оповещения
    // и отправлять данные в потоки
    auto logger = std::make_shared<LoggingToFile>( *LineToDisk.get() );
    auto shower = std::make_shared<ShowOnDisplay>( *LineToConsole.get() );

    receiver->AddProcessor( logger );

    auto pr1 = std::make_shared<Processor>( LineToDisk, exc, out, true );
    auto pr2 = std::make_shared<Processor>( LineToDisk, exc, out, true );
    auto pr3 = std::make_shared<Processor>( LineToConsole, exc, out );

    std::thread th1( &Processor::Loop, pr1 );
    std::thread th2( &Processor::Loop, pr2 );
    std::thread th3( &Processor::Loop, pr3 );

    try
    {
        receiver->MainLoop( n, in );
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

        throw;
    }

    LineToDisk->push( RequestType::Quit, std::string{}, 0 );
    LineToDisk->push( RequestType::Quit, std::string{}, 0 );
    LineToConsole->push( RequestType::Quit, std::string{}, 0 );

    th1.join();
    th2.join();
    th3.join();

    return std::make_pair( pr1->filesForTesting, pr2->filesForTesting );
}

BOOST_AUTO_TEST_CASE ( check_files_in_output )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "cmd2\n"
                "{\n"
                "cmd3\n"
                "cmd4\n"
                "cmd5\n"
                "}\n";

    //Размер контейнера при чтении из файлов не равен размеру контейнера
    //результата, где то ошибка при чтении файлов?

    auto res = test_helper_logger( ss_in, ss_out, "2" );

    std::set<std::string> result;
    result.insert( "bulk: cmd1, cmd2" );
    result.insert( "bulk: cmd3, cmd4, cmd5" );

    std::fstream file;
    std::set<std::string> textFromFiles{};


    if( !res.first.empty() )
    {
        for( const auto& name : res.first )
        {
            file.open( name, std::ios::in );
            if( file )
            {
                std::string str{""};
                std::getline( file, str );
                textFromFiles.insert( str );
            }
            file.close();
        }
    }

    if( !res.second.empty() )
    {
        for( const auto& name : res.second )
        {
            file.open( name, std::ios::in );
            if( file )
            {
                std::string str{""};
                std::getline( file, str );
                textFromFiles.insert( str );
            }
            file.close();
        }
    }

    BOOST_CHECK_EQUAL ( textFromFiles.size(), 2 );

    BOOST_CHECK_EQUAL_COLLECTIONS( result.begin(), result.end(), textFromFiles.begin(), textFromFiles.end() );
}

BOOST_AUTO_TEST_SUITE_END()

