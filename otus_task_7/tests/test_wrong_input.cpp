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

    BOOST_CHECK_THROW( receiver.MainLoop( 2, ss_in, ss_out ), std::invalid_argument );
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
             "cmd6123456789123456789123456789123456789123456789123456789\n"
             "cmd7\n"
             "}\n";

    BOOST_CHECK_THROW( receiver.MainLoop( 3, ss_in, ss_out ), std::invalid_argument );
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

    BOOST_CHECK_THROW( receiver.MainLoop( 3, ss_in, ss_out ), std::invalid_argument );
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

    BOOST_CHECK_THROW( receiver.MainLoop( 3, ss_in, ss_out ), std::invalid_argument );
}

BOOST_AUTO_TEST_CASE ( check_V )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in << "cmd1}\n";

    BOOST_CHECK_THROW( receiver.MainLoop( 3, ss_in, ss_out ), std::invalid_argument );

}

BOOST_AUTO_TEST_CASE ( check_VI )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "}\n";

    BOOST_CHECK_THROW( receiver.MainLoop( 4, ss_in, ss_out ), std::invalid_argument );
}


BOOST_AUTO_TEST_CASE ( check_VII )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    receiver.AddProcessor( logger );

    ss_in <<    "cmd1\n"
                "cmd2\n"
                "{\n"
                "cmd3\n"
                "cmd4\n"
                "cmd5\n"
                "}\n";

    receiver.MainLoop( 2, ss_in, ss_out );

    std::vector<std::string> result;
    result.push_back( "bulk: cmd1, cmd2\n");
    result.push_back( "bulk: cmd3, cmd4, cmd5\n" );

    for(int i = 0; i != 2; ++i )
    {
        std::fstream file;
        std::string file_name( logger->files[i] );
        file.open( file_name, std::ios::in );
        std::string str;

        if(file)
        {
            std::getline( file, str );
            file.close();
        }

        BOOST_CHECK_EQUAL ( result[i], str.append( "\n" ) );
    }
}
BOOST_AUTO_TEST_SUITE_END()

