#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include "test_header.h"

BOOST_AUTO_TEST_SUITE ( run_lib_with_some_bulk )

BOOST_AUTO_TEST_CASE ( check_one_by_one )
{
    auto h1 = async::connect( 1 );
    BOOST_CHECK_EQUAL( reinterpret_cast<size_t>(h1), 1 );

    auto h2 = async::connect( 2 );
    BOOST_CHECK_EQUAL( reinterpret_cast<size_t>(h2), 2 );

    auto h3 = async::connect( 5 );
    BOOST_CHECK_EQUAL( reinterpret_cast<size_t>(h3), 3 );

    auto h4 = async::connect( 1 );
    BOOST_CHECK_EQUAL( reinterpret_cast<size_t>(h4), 4 );

    async::disconnect(h1);
    async::disconnect(h2);
    async::disconnect(h3);
    async::disconnect(h4);
}

BOOST_AUTO_TEST_CASE( wrong_block_size )
{
    BOOST_CHECK(async::connect( -1 ) == nullptr);
    async::clear();
    BOOST_CHECK(async::connect( 0 ) == nullptr);
    async::clear();
    BOOST_CHECK(async::connect( 51 ) == nullptr);
}

BOOST_AUTO_TEST_SUITE_END()


