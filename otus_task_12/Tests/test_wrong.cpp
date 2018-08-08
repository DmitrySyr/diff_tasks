#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

#include "../StartTask.h"

BOOST_AUTO_TEST_SUITE( test_wrong_start )

BOOST_AUTO_TEST_CASE( wrong_bulk )
{
    char* paramete[3];
    paramete[0] = "";
    paramete[1] = "2222";
    paramete[2] = "0";

    BOOST_CHECK_THROW(StartTask::Start(3, paramete  ), std::invalid_argument);

    paramete[2] = "-20";
    BOOST_CHECK_THROW(StartTask::Start(3, paramete   ), std::invalid_argument);

    paramete[2] = "51";
    BOOST_CHECK_THROW(StartTask::Start(3, paramete ), std::invalid_argument);

    paramete[2] = "aaaaaaaaa";
    BOOST_CHECK_THROW(StartTask::Start(3, paramete ), std::invalid_argument);
}


BOOST_AUTO_TEST_CASE( wrong_port )
{
    char* paramete[3];
    paramete[0] = "";
    paramete[2] = "0";

    paramete[1] = "65536";
    BOOST_CHECK_THROW(StartTask::Start(3, paramete ), std::invalid_argument);

    paramete[1] = "bbbbbbbbbbb";
    BOOST_CHECK_THROW(StartTask::Start(3, paramete ), std::invalid_argument);

    paramete[1] = "-2222";
    BOOST_CHECK_THROW(StartTask::Start(3, paramete ), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE( wrong_number_of_params )
{
    char* paramete[3];
    paramete[0] = "";
    paramete[1] = "2222";
    paramete[2] = "0";

    BOOST_CHECK_THROW(StartTask::Start(2, paramete ), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()
