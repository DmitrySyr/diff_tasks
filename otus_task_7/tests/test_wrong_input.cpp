#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include "test_header.h"

BOOST_AUTO_TEST_SUITE ( test_wrong_input )

    ReceivingBulk receiver;

    auto logger = std::make_shared<LoggingToFile>( );
    auto shower = std::make_shared<ShowOnDisplay>( );

BOOST_AUTO_TEST_CASE ( check_I )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in << "cmd1\ncmd2\n\ncmd4\ncmd5\n";

    receiver.AddProcessor( shower );

    BOOST_CHECK_NO_THROW( receiver.MainLoop( "2", ss_in, ss_out ) );
}

BOOST_AUTO_TEST_CASE ( check_II )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in << std::string( STRING_MAX_SIZE + 1, '0' );

    BOOST_CHECK_THROW( receiver.MainLoop( "3", ss_in, ss_out ), std::invalid_argument );
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

    BOOST_CHECK_NO_THROW( receiver.MainLoop( "3", ss_in, ss_out ) );
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

    BOOST_CHECK_NO_THROW( receiver.MainLoop( "3", ss_in, ss_out ) );
}

BOOST_AUTO_TEST_CASE ( check_V )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in << "cmd1}\n";

    BOOST_CHECK_NO_THROW( receiver.MainLoop( "3", ss_in, ss_out ) );

}

BOOST_AUTO_TEST_CASE ( check_VI )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "}\n";

    BOOST_CHECK_THROW( receiver.MainLoop( "4", ss_in, ss_out ), std::invalid_argument );
}

BOOST_AUTO_TEST_CASE ( check_VII )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "}\n";

    BOOST_CHECK_THROW( receiver.MainLoop( "-1", ss_in, ss_out ), std::invalid_argument );
}

BOOST_AUTO_TEST_CASE ( check_VIII )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "}\n";

    BOOST_CHECK_THROW( receiver.MainLoop( std::to_string( MAX_BLOCK_SIZE)
                                         , ss_in, ss_out ), std::invalid_argument );
}

BOOST_AUTO_TEST_SUITE_END()

