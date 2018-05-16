#include "Matrix.h"
#include <sstream>
#include <iostream>


#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE ( matrix_tests )

BOOST_AUTO_TEST_CASE ( check_empty )
{
    Matrix<int, 0, 3> matrix;

    BOOST_CHECK_EQUAL ( matrix.size(), 0 );

    int a = matrix[0][0][150];

    BOOST_CHECK_EQUAL ( a, 0 );

}

BOOST_AUTO_TEST_CASE (new_element)
{
    Matrix<int, 0, 2> matrix;
    matrix[100][100] = 314;
    BOOST_CHECK_EQUAL (matrix[100][100], 314);
    BOOST_CHECK_EQUAL (matrix.size(), 1);

    Matrix<int, -10, 5> matrixII;
    matrixII[1][1][1][1][2] = 8989;
    BOOST_CHECK_EQUAL ( matrixII.size(), 1 );
}

BOOST_AUTO_TEST_CASE ( assignment_to_element )
{
    Matrix<int, 0, 2> matrix;
    ((matrix[1][100] = 314) = 0) = 217;

    BOOST_CHECK_EQUAL (matrix[1][100], 217);
    BOOST_CHECK_EQUAL (matrix.size(), 1);
}

BOOST_AUTO_TEST_CASE ( deleting_element )
{
    Matrix<int, 0, 3> matrix;

    matrix[1][1][1] = 1;
    matrix[1][2][1] = 2;
    matrix[1][3][1] = 3;
    BOOST_CHECK_EQUAL (matrix.size(), 3);

    matrix[1][1][1] = 0;
    BOOST_CHECK_EQUAL (matrix.size(), 2);

    matrix[1][2][1] = 0;
    BOOST_CHECK_EQUAL (matrix.size(), 1);

    matrix[1][3][1] = 0;
    BOOST_CHECK_EQUAL (matrix.size(), 0);

    matrix[1][1][1] = 1;
    BOOST_CHECK_EQUAL (matrix.size(), 1);

    matrix[1][1][0] = 0;
    BOOST_CHECK_EQUAL (matrix.size(), 1);
}

BOOST_AUTO_TEST_CASE ( printing_element )
{
    Matrix<int64_t, -1, 4> matrix;


    std::stringstream ss;

    matrix[0][0][0][1] = 555;
    matrix[0][0][0][10] = 888;


    ss << matrix[0][0][0][1];
    BOOST_CHECK_EQUAL ( ss.str(), "555" );
    ss.str( std::string() );

    ss << matrix[0][0][0][10];
    BOOST_CHECK_EQUAL ( ss.str(), "888" );
    ss.str( std::string() );

    matrix[0][0][0][1] = -1;
    ss << matrix[0][0][0][1];
    BOOST_CHECK_EQUAL ( ss.str(), "-1" );
    ss.str( std::string() );

    matrix[0][0][0][1] = -5;
    ss << matrix[0][0][0][1];
    BOOST_CHECK_EQUAL ( ss.str(), "-5" );
    ss.str( std::string() );

}

BOOST_AUTO_TEST_CASE ( select_from )
{
    Matrix<int32_t, 0, 2> matrix;

    std::stringstream ss;
    matrix[5][9] = 888;

    for( auto el : matrix )
    {
        int x = 0;
        int y = 0;
        int value = 0;

        std::tie( x, y, value ) = el;
        ss << x << y << value;

        BOOST_CHECK_EQUAL ( ss.str(), "59888" );
    }

}
BOOST_AUTO_TEST_CASE ( check_TechnicalTask )
{

    Matrix<int, -1, 2> matrix;

    BOOST_CHECK_EQUAL ( 0, matrix.size() );

    auto value = matrix[3][5];

    BOOST_CHECK_EQUAL ( -1, value );

    matrix[100][100] = 314;
    BOOST_CHECK ( matrix[100][100] == 314 );
    BOOST_CHECK ( matrix.size() == 1 );

    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        BOOST_CHECK_EQUAL ( 100, x );
        BOOST_CHECK_EQUAL ( 100, y );
        BOOST_CHECK_EQUAL ( 314, v );
    }

}


BOOST_AUTO_TEST_CASE ( check_incorrect_requests )
{

    Matrix<int, 0, 2> matrix;

    matrix[3][5] = 1;
    matrix[3][1] = 2;
    matrix[5][1] = 3;



    BOOST_CHECK_THROW( int value = matrix[3][5][1], std::exception );

    BOOST_CHECK_THROW( int v = matrix[3], std::exception );

    BOOST_CHECK_THROW( auto v2 = matrix[3][-9], std::exception );

    BOOST_CHECK_THROW( auto v2 = matrix[-3][9], std::exception );


}

BOOST_AUTO_TEST_CASE ( check_iterators )
{

    Matrix<int, 0, 2> matrix;

    matrix[3][5] = 1;
    matrix[3][1] = 2;
    matrix[5][1] = 3;


    auto iter1 = matrix.begin();

    ++iter1; ++iter1;

    auto iter2 = matrix.begin();

    int x, y, val;

    std::tie( x, y, val ) = *iter1;

    BOOST_CHECK_EQUAL( x, 5 );
    BOOST_CHECK_EQUAL( y, 1 );
    BOOST_CHECK_EQUAL( val, 3 );


    std::tie( x, y, val ) = *iter2;

    BOOST_CHECK_EQUAL( x, 3 );
    BOOST_CHECK_EQUAL( y, 1 );
    BOOST_CHECK_EQUAL( val, 2 );

    ++iter1;

    BOOST_CHECK_EQUAL( true, iter1 == matrix.end() );

}

BOOST_AUTO_TEST_SUITE_END()

