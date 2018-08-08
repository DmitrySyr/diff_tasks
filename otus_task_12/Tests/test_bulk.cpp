#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>
#include <thread>

#include "../StartTask.h"
#include <experimental/filesystem>
#include <vector>
#include <algorithm>

BOOST_AUTO_TEST_SUITE( test_all_bulk )

BOOST_AUTO_TEST_CASE( simple_bulk )
{
    std::vector<std::string> request {{"1\n"}, {"2\n"}, {"3\n"}, {"4\n"}, {"5\n"}
                                        ,{"6\n"}, {"7\n"}, {"8\n"}, {"9\n"}, {"10\n"}};

    std::vector<std::string> result
        {{"bulk: 1, 2, 3"}
        , {"bulk: 4, 5, 6"}
        , {"bulk: 7, 8, 9"}
        , {"bulk: 10"}};

    std::string path = std::experimental::filesystem::current_path();

    path.append( "/bulk_server 2222 3 > res.txt" );

    std::thread server( [&path]()
                       {
                           std::system( path.c_str() );
                       });

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    io_service ioservice;
    tcp::endpoint ep( tcp::v4(), 2222 );
    tcp::socket sock(ioservice);
    sock.connect(ep);

    std::for_each(std::cbegin( request ),
                std::cend( request ),
                [&sock]( const auto& cmd ) {
                  sock.write_some( buffer(cmd) );
                });

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    sock.close();

    std::system("killall bulk_server");

    server.join();

    std::ifstream f{"res.txt"};

    BOOST_CHECK_EQUAL(true, f.is_open());

    std::vector<std::string> cmd_from_file;

    size_t SizeOfCommands = result.size();

    for(std::string line; getline(f, line);)
    {
        cmd_from_file.push_back(line);
        if( --SizeOfCommands == 0 )
        {
            break;
        }
    }

    f.close();

    BOOST_CHECK_EQUAL_COLLECTIONS(std::cbegin(result)
                                  , std::cend(result)
                                  , std::cbegin(cmd_from_file)
                                  , std::cend(cmd_from_file)
                                  );
    std::system("rm -f *.log");
    std::system("rm -f res.txt");

}

BOOST_AUTO_TEST_CASE( multiply_bulk )
{
    std::vector<std::string> request {{"1\n"}, {"2\n"}, {"3\n"}, {"4\n"}, {"5\n"}
                                        ,{"6\n"}, {"7\n"}, {"8\n"}, {"9\n"}, {"10\n"}};

    std::string result
        {"bulk: 1, 1, 2\nbulk: 2, 3, 3\nbulk: 4, 4, 5\nbulk: 5, 6, 6\nbulk: 7, 7, 8\n"
        "bulk: 8, 9, 9\nbulk: 10, 10\n"};

    std::sort( result.begin(), result.end() );

    std::string path = std::experimental::filesystem::current_path();

    path.append( "/bulk_server 2222 3 > res.txt" );

    std::thread server( [&path]()
                       {
                           std::system( path.c_str() );
                       });

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    std::system( "seq 1 10 | nc localhost 2222&\nseq 1 10 | nc localhost 2222&");

    std::system("killall bulk_server");

    server.join();

    std::ifstream f{"res.txt"};

    BOOST_CHECK_EQUAL(true, f.is_open());

    std::string cmd_from_file{""};

    size_t SizeOfCommands = 7;

    for( std::string line; getline( f, line ); )
    {
        cmd_from_file.append( line.append( "\n" ) );
        if( --SizeOfCommands == 0 )
        {
            break;
        }
    }

    f.close();

    std::sort(cmd_from_file.begin(), cmd_from_file.end() );

    BOOST_CHECK_EQUAL( cmd_from_file, result );
    std::system("rm -f *.log");
    std::system("rm -f res.txt");

}

BOOST_AUTO_TEST_SUITE_END()
