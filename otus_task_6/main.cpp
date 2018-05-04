#include <iostream>
#include <array>
#include <cassert>
#include <iomanip>

#include "Matrix.h"

int main()
{

/*  тестовый вывод для задания

    Matrix<int, -1, 2> matrix;

    std::cout << matrix.size() << "\n";

    auto value = matrix[3][5];

    std::cout << "value " << value << " , size " << matrix.size() << "\n";

    matrix[100][100] = 314;
    assert(matrix[100][100] == 314);
    assert(matrix.size() == 1);
//     выведется одна строка
//     100100314
    for(auto c: matrix)
    {
        int x;
        int y;
        int v;
        std::tie(x, y, v) = c;
        std::cout << x << y << v << std::endl;
    }
    */

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
