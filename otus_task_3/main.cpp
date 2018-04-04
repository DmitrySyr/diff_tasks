#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iterator>

#include "MyAlliocator.h"
#include "MList.h"


template <typename T>
constexpr T factorial(T n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

int main()
{

    auto map1 = std::map<int,int>{};

    for (size_t i = 0; i < 10; i++) {
        map1[i] = factorial(i);
    }


    auto map2 = std::map<int, int, std::less<int>, MyAlliocator<std::pair<const int, int>, 10>>{};

    for (size_t i = 0; i < 10; ++i){
        map2[i] = factorial(i);
    }

    for (const auto &i : map2)
        std::cout << i.first << " " << i.second << "\n";


    MList<int> ml_1;

    for (size_t i = 0; i < 10; ++i){
        ml_1.push(factorial(i));
    }

    MList<int, MyAlliocator<Node<int>, 10>> ml_2;

    for (size_t i = 9; i != -1; --i){
        ml_2.push(factorial(i));
    }

    for(auto& i : ml_2) {
        std::cout << i << "\n";
    }

    return 0;
}
