#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include "test_header.h"

BOOST_AUTO_TEST_SUITE ( test_output )

BOOST_AUTO_TEST_CASE ( check_files_in_output )
{
    std::stringstream ss_out;

    Handler handler{ ss_out, true };

    auto h = handler.getIndex( 3 );

    std::string in{ "cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n" };

    handler.proceed( h, in.c_str(), 25 );

    std::this_thread::sleep_for( std::chrono::milliseconds( 30 ) );

    auto fileNames = handler.getFileNamesForTest();

    std::set<std::string> result;
    result.insert( "bulk: cmd1, cmd2, cmd3" );
    result.insert( "bulk: cmd4, cmd5" );

    std::fstream file;
    std::set<std::string> textFromFiles{};


    if( !fileNames.empty() )
    {
        for( const auto& name : fileNames )
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

