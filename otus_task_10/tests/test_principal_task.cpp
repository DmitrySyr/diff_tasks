#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include "test_header.h"


BOOST_AUTO_TEST_SUITE ( principal_task )

void test_helper_shower( std::istream& in, std::ostream& out, std::string n )
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

    receiver->AddProcessor( shower );

    auto pr1 = std::make_shared<Processor>( LineToDisk, exc );
    auto pr2 = std::make_shared<Processor>( LineToDisk, exc );
    auto pr3 = std::make_shared<Processor>( LineToConsole, exc, out );

    std::thread th1( &Processor::Loop, pr1 );
    std::thread th2( &Processor::Loop, pr2 );
    std::thread th3( &Processor::Loop, pr3 );

    receiver->MainLoop( n, in );

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

    ss_in << "cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n";

    test_helper_shower( ss_in, ss_out, "2" );

    std::string result{"bulk: cmd1, cmd2\nbulk: cmd3, cmd4\nbulk: cmd5\n"};

    BOOST_CHECK_EQUAL ( ss_out.str(), result );
}

BOOST_AUTO_TEST_CASE ( check_II )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in << "cmd1\n"
             "cmd2\n"
             "cmd3\n"
             "{\n"
             "cmd4\n"
             "cmd5\n"
             "cmd6\n"
             "cmd7\n"
             "}\n";

    test_helper_shower( ss_in, ss_out, "3" );

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5, cmd6, cmd7\n" );
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
                "cmd4\n"
                "}\n"
                "cmd5\n"
                "cmd6\n"
                "}\n";

    test_helper_shower( ss_in, ss_out, "3" );

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6\n" );
}

BOOST_AUTO_TEST_CASE ( check_IV )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "cmd2\n"
                "{\n"
                "cmd3\n"
                "cmd4\n";

    test_helper_shower( ss_in, ss_out, "3" );

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1, cmd2\n" );
}

BOOST_AUTO_TEST_CASE ( check_V )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n";

    test_helper_shower( ss_in, ss_out, "4" );

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1\n" );

}

BOOST_AUTO_TEST_CASE ( check_VI )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "cmd2\n"
                "{\n"
                "}cmd3\n"
                "cmd4\n"
                "}";

    test_helper_shower( ss_in, ss_out, "4" );

    BOOST_CHECK_EQUAL( ss_out.str(), "bulk: cmd1, cmd2\nbulk: }cmd3, cmd4\n" );

}

BOOST_AUTO_TEST_SUITE_END()

