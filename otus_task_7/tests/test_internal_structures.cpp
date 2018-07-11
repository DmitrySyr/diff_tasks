#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include "test_header.h"

BOOST_AUTO_TEST_SUITE ( principal_task )



BOOST_AUTO_TEST_CASE ( check_I )
{
    ReceivingBulk receiver;

    auto logger = std::make_shared<LoggingToFile>( );
    auto shower = std::make_shared<ShowOnDisplay>( );

    BOOST_CHECK_EQUAL ( receiver.GetNumberOfSubscribers(), 0 );

    receiver.AddProcessor( shower );

    BOOST_CHECK_EQUAL ( receiver.GetNumberOfSubscribers(), 1 );

    receiver.AddProcessor( logger );

    BOOST_CHECK_EQUAL ( receiver.GetNumberOfSubscribers(), 2 );
}

BOOST_AUTO_TEST_SUITE_END()


