#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include "test_header.h"

BOOST_AUTO_TEST_SUITE ( principal_task )

    ReceivingBulk receiver;

    auto logger = std::make_shared<LoggingToFile>( );
    auto shower = std::make_shared<ShowOnDisplay>( );

BOOST_AUTO_TEST_CASE ( check_I )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in << "cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n";

    receiver.AddProcessor( shower );

    receiver.MainLoop( 2, ss_in, ss_out );

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1, cmd2\nbulk: cmd3, cmd4\nbulk: cmd5\n" );
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

    receiver.MainLoop( 3, ss_in, ss_out );

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

    receiver.MainLoop( 3, ss_in, ss_out );

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

    receiver.MainLoop( 4, ss_in, ss_out );

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1, cmd2\n" );
}

BOOST_AUTO_TEST_CASE ( check_V )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n";

    receiver.MainLoop( 4, ss_in, ss_out );

    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1\n" );

}

BOOST_AUTO_TEST_SUITE_END()

