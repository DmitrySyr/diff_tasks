#define DEBUG 0

#include <sstream>
#include <iostream>
#include <sstream>
#include <thread>

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

#include "HW7.h"
#include "Queue.h"
#include "Processor.h"

BOOST_AUTO_TEST_SUITE ( bulk_tests )

BOOST_AUTO_TEST_CASE ( check_I )
{
    std::stringstream ss_in{ std::string() };

    std::stringstream ss_out{ std::string() };

    ReceivingBulk receiver( std::cout );
    auto shower = std::make_shared<ShowOnDisplay>( ss_out );
    receiver.AddProcessor( shower );

    ss_in << "cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n";

    receiver.MainLoop( 2, ss_in );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    shower->PrintStatistics();

    std::string result{"bulk: cmd1, cmd2\nbulk: cmd3, cmd4\nbulk: cmd5\n"
                                        "Log thread: 3 blocks and 5 commands.\n"};

    BOOST_CHECK_EQUAL ( ss_out.str(), result );
}

BOOST_AUTO_TEST_CASE ( check_II )
{
    std::stringstream ss_in{ std::string() };

    std::stringstream ss_out{ std::string() };

    ReceivingBulk receiver( std::cout );
    auto shower = std::make_shared<ShowOnDisplay>( ss_out );
    receiver.AddProcessor( shower );

    ss_in << "cmd1\n"
             "cmd2\n"
             "cmd3\n"
             "{\n"
             "cmd4\n"
             "cmd5\n"
             "cmd6\n"
             "cmd7\n"
             "}\n";

    receiver.MainLoop( 3, ss_in );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    shower->PrintStatistics();

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5, cmd6, cmd7\n"
                                        "Log thread: 2 blocks and 7 commands.\n");
}

BOOST_AUTO_TEST_CASE ( check_III )
{
    std::stringstream ss_in{ std::string() };

    std::stringstream ss_out{ std::string() };

    ReceivingBulk receiver( std::cout );
    auto shower = std::make_shared<ShowOnDisplay>( ss_out );
    receiver.AddProcessor( shower );

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

    receiver.MainLoop( 3, ss_in );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    shower->PrintStatistics();

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1, cmd2, cmd3, cmd4, cmd5, cmd6\n"
                                        "Log thread: 1 blocks and 6 commands.\n" );
}

BOOST_AUTO_TEST_CASE ( check_IV )
{
    std::stringstream ss_in{ std::string() };

    std::stringstream ss_out{ std::string() };

    ReceivingBulk receiver( std::cout );
    auto shower = std::make_shared<ShowOnDisplay>( ss_out );
    receiver.AddProcessor( shower );

    ss_in <<    "cmd1\n"
                "cmd2\n"
                "{\n"
                "cmd3\n"
                "cmd4\n";

    receiver.MainLoop( 4, ss_in );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    shower->PrintStatistics();

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1, cmd2\n"
                                        "Log thread: 1 blocks and 2 commands.\n"  );
}

BOOST_AUTO_TEST_CASE ( check_V )
{
    std::stringstream ss_in{ std::string() };

    std::stringstream ss_out{ std::string() };

    ReceivingBulk receiver( std::cout );
    auto shower = std::make_shared<ShowOnDisplay>( ss_out );
    receiver.AddProcessor( shower );

    ss_in <<    "cmd1\n";

    receiver.MainLoop( 4, ss_in );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    shower->PrintStatistics();

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1\n"
                                        "Log thread: 1 blocks and 1 commands.\n"  );
}

BOOST_AUTO_TEST_CASE ( check_VI )
{
    std::stringstream ss_in{ std::string() };

    std::stringstream ss_out{ std::string() };

    ReceivingBulk receiver( std::cout );
    auto shower = std::make_shared<ShowOnDisplay>( ss_out );
    receiver.AddProcessor( shower );
    std::this_thread::sleep_for( std::chrono::seconds( 1 ) );
    shower->PrintStatistics();

    ss_in <<    "cmd1\n"
                "}\n";

    BOOST_CHECK_THROW( receiver.MainLoop( 4, ss_in ), std::exception );
}


BOOST_AUTO_TEST_CASE ( check_VII )
{
    std::stringstream ss_in{ std::string() };

    std::stringstream ss_out{ std::string() };

    ReceivingBulk receiver{ std::cout };

    auto LineToDisk = std::make_shared<Queue>( 5 );

    auto logger = std::make_shared<LoggingToFile>( *LineToDisk.get() );

    auto pr1 = std::make_shared<Processor>( *LineToDisk.get(), ss_out, true );

    std::thread th1( &Processor::Loop, pr1.get() );

    receiver.AddProcessor( logger );


    ss_in <<    "cmd1\n"
                "cmd2\n"
                "{\n"
                "cmd3\n"
                "cmd4\n"
                "cmd5\n"
                "}\n";

    receiver.MainLoop( 2, ss_in );

    std::vector<std::string> result;
    result.push_back( "bulk: cmd1, cmd2\n");
    result.push_back( "bulk: cmd3, cmd4, cmd5\n" );

    while( pr1->filesForTesting.empty() )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( 300 ) );
    }

    for(int i = 0; i != 2; ++i )
    {
        std::fstream file;
        std::string file_name{ pr1->filesForTesting[i] };
        file.open( file_name, std::ios::in );
        std::string str;

        if(file)
        {
            std::getline( file, str );
            file.close();
        }

        BOOST_CHECK_EQUAL ( result[i], str.append( "\n" ) );
    }

    LineToDisk->push( RequestType::Quit, std::vector<std::string>{}, 0 );

    th1.join();
}

BOOST_AUTO_TEST_SUITE_END()

