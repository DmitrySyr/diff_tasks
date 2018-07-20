#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <algorithm>

#include "test_header.h"
#include "../Handler.h"


BOOST_AUTO_TEST_SUITE ( principal_task )


BOOST_AUTO_TEST_CASE ( single_input )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    Handler handler{ ss_out };

    auto h = handler.getIndex( 2 );

    std::string in{ "cmd1\ncmd2\ncmd3\ncmd4\ncmd5\n" };

    handler.proceed( h, in.c_str(), 25 );
    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

    std::string result{"bulk: cmd1, cmd2\nbulk: cmd3, cmd4\nbulk: cmd5\n"};

    BOOST_CHECK_EQUAL ( ss_out.str(), result );
}

BOOST_AUTO_TEST_CASE ( check_multi_calls )
{
    std::stringstream ss_in, ss_in_II;
    std::stringstream ss_out;

    Handler handler{ ss_out };

    auto h = handler.getIndex( 3 );

    ss_in << "cmd1\n"
             "cmd2\n"
             "cmd3\n"
             "{\n"
             "cmd4\n"
             "cmd5\n"
             "cmd6\n"
             "cmd7\n"
             "}\n";

    ss_in_II << "dd1\n"
                "dd2\n"
                "dd3\n"
                "{\n"
                "dd5\n";

    handler.proceed( h, ss_in.str().c_str(), 39 );
    handler.proceed( h, ss_in_II.str().c_str(), 18 );
    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

    std::string res{"bulk: cmd1, cmd2, cmd3\nbulk: cmd4, cmd5, cmd6, cmd7\nbulk: dd1, dd2,"
                    " dd3\n"};
    std::sort( res.begin(), res.end() );

    std::string out = ss_out.str();
    std::sort( out.begin(), out.end() );

    BOOST_CHECK_EQUAL ( res, out );
}

BOOST_AUTO_TEST_CASE ( test_parentnesis )
{
    std::stringstream ss_in;
    std::stringstream ss_out;

    ss_in <<    "cmd1\n"
                "cmd2\n"
                "{\n"
                "cmd3\n"
                "cmd4\n"
                "}\n"
                "cmd5\n"
                "cmd6\n";

    Handler handler{ ss_out };

    auto h = handler.getIndex( 3 );

    handler.proceed( h, ss_in.str().c_str(), 34 );

    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );
    BOOST_CHECK_EQUAL ( ss_out.str(), "bulk: cmd1, cmd2\nbulk: cmd3, cmd4\nbulk: cmd5, cmd6\n" );
}

BOOST_AUTO_TEST_CASE ( check_multi_input )
{
    std::stringstream ss_out;

    std::string thlow_I_I{ "cmd1\ncmd2\n{\ncmd4\ncmd5\n" };
    std::string thlow_I_II{ "cmd6\ncmd7\n" };
    std::string thlow_II_I{ "f1\nf2\nf3\n{\nf4\nf5\n}\n" };
    std::string thlow_II_II{ "f6\n" };
    std::string thlow_III_I{ "c1\nc2\n{\nc3\nc4\nc5\n}\nc6\nc7\nc8\n" };

    Handler handler{ ss_out };

    auto h1 = handler.getIndex( 3 );
    auto h2 = handler.getIndex( 8 );
    auto h3 = handler.getIndex( 2 );

    handler.proceed( h1, thlow_I_I.c_str(), 22 );
    handler.proceed( h3, thlow_III_I.c_str(), 28 );
    handler.proceed( h2, thlow_II_II.c_str(), 3 );
    handler.proceed( h1, thlow_I_II.c_str(), 10 );
    handler.proceed( h2, thlow_II_I.c_str(), 19 );

    std::this_thread::sleep_for( std::chrono::milliseconds( 500 ) );

    std::string out = ss_out.str();
    std::sort( out.begin(), out.end() );

    std::string res{"bulk: cmd1, cmd2\nbulk: cmd6, cmd7\n"
                    "bulk: c1, c2\nbulk: c3, c4, c5\nbulk: c6, c7\nbulk: c8\n"
                    "bulk: f6\n"
                    "bulk: f1, f2, f3\nbulk: f4, f5\n"
                    };
    std::sort( res.begin(), res.end() );

    BOOST_CHECK_EQUAL ( res, out );
}

BOOST_AUTO_TEST_SUITE_END()

