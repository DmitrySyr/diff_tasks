#include <iostream>
#include <array>
#include <cassert>
#include <iomanip>
#include <chrono>

#include "Matrix.h"

int main()
{

    try{

        Matrix<int, 0, 2> matrix;

        for( uint16_t i = 0; i != 10; ++i )
        {
            matrix[i][i] = i;
            matrix[i][9-i] = 9 - i;
        }

        for ( uint16_t i = 1; i < 9; ++i )
        {
            for ( uint16_t k = 1; k < 9; ++k )
            {
                std::cout << std::setw( 3 ) << matrix[i][k] << ' ';
            }

            std::cout << "\n";
        }

        std::cout << "Matrix size is : " << matrix.size() << "\n";


        for( const auto& el : matrix )
        {
            int x, y, val;
            std::tie( x,y,val ) = el;
            std::cout << x << y << val << "\n";
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
    }

    return 0;
}
