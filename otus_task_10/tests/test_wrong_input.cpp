#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include "test_header.h"

BOOST_AUTO_TEST_SUITE ( test_wrong_input )

void test_helper_shower( std::istream& in, std::ostream& out, std::string n )
{
    auto receiver = std::make_shared<ReceivingBulk>( std::cout );

    // Инициализируем очередь для потоков под запись в файл
    auto LineToDisk    = std::make_shared<Queue>();
    auto LineToConsole = std::make_shared<Queue>();

    // Инициализируем два объекта, которые будут получать оповещения
    // и отправлять данные в потоки
    auto logger = std::make_shared<LoggingToFile>( *LineToDisk.get() );
    auto shower = std::make_shared<ShowOnDisplay>( *LineToConsole.get() );

    receiver->AddProcessor( shower );

    auto pr1 = std::make_shared<Processor>( LineToDisk );
    auto pr2 = std::make_shared<Processor>( LineToDisk );
    auto pr3 = std::make_shared<Processor>( LineToConsole, out );

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
}

BOOST_AUTO_TEST_CASE ( check_I )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in << "cmd1\ncmd2\n\ncmd4\ncmd5\n";

    BOOST_CHECK_NO_THROW( test_helper_shower( ss_in, ss_out, "2" ) );
}

BOOST_AUTO_TEST_CASE ( check_II )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in << std::string( STRING_MAX_SIZE + 1, '0' );

    BOOST_CHECK_THROW( test_helper_shower( ss_in, ss_out, "2" ), std::invalid_argument );
}

BOOST_AUTO_TEST_CASE ( check_III )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "{\n"
                "cmd1\n"
                "cmd2\n"
                "{\n"
                "cmd3\n"
                "cm{d4\n"
                "}\n"
                "cmd5\n"
                "cmd6\n"
                "}\n";

    BOOST_CHECK_NO_THROW( test_helper_shower( ss_in, ss_out, "3" ) );
}

BOOST_AUTO_TEST_CASE ( check_IV )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "cmd2\n"
                "{\n"
                "}cmd3\n"
                "cmd4\n";

    BOOST_CHECK_NO_THROW( test_helper_shower( ss_in, ss_out, "3" ) );
}

BOOST_AUTO_TEST_CASE ( check_V )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in << "cmd1}\n";

    BOOST_CHECK_NO_THROW( test_helper_shower( ss_in, ss_out, "3" ) );

}

BOOST_AUTO_TEST_CASE ( check_VI )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "}\n";

    BOOST_CHECK_THROW( test_helper_shower( ss_in, ss_out, "4" ), std::invalid_argument );
}

BOOST_AUTO_TEST_CASE ( check_VII )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "}\n";

    BOOST_CHECK_THROW( test_helper_shower( ss_in, ss_out, "-1" ), std::invalid_argument );
}

BOOST_AUTO_TEST_CASE ( check_VIII )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "}\n";

    BOOST_CHECK_THROW( test_helper_shower( ss_in, ss_out, std::to_string( MAX_BLOCK_SIZE) )
                                          , std::invalid_argument );
}

BOOST_AUTO_TEST_SUITE_END()

