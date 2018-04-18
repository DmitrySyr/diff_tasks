#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iterator>

#include "pool_allocator.h"
#include "m_list.h"



template <typename T>
constexpr T factorial(T n)
{
  if(n < 0) throw std::invalid_argument("Argument of factorial function should be positive.");

  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

static_assert(factorial(0) == 1, "1");
static_assert(factorial(5) == 120, "5");

int main()
{
    try{

        auto gen_func = [i=0] () mutable {
            auto value = std::make_pair(i, factorial(i));
            ++i;
            return value;
        };

        auto gen_value_list = [i=0] () mutable {
            return i++;
        };

        auto map1 = std::map<int,int>{};

        std::generate_n( std::inserter( map1, std::begin( map1 ) ), 10, gen_func );


        std::map<int, int, std::less<int>, MyAlliocator<std::pair<const int, int>, 10>> map2{};

        std::generate_n( std::inserter( map2, std::begin( map2 ) ), 10, gen_func);

        for (const auto &i : map2)
            std::cout << i.first << " " << i.second << "\n";


        MList<int> ml_1;

        std::generate_n( std::front_inserter( ml_1 ), 10, gen_value_list);


        MList<int, MyAlliocator<Node<int>, 10>> ml_2;

        std::generate_n( std::front_inserter( ml_2 ), 10, gen_value_list);


        for(auto& i : ml_2) {
            std::cout << i << "\n";
        }

        MList<int> ml_3(ml_1);

    }
    catch(std::exception& e){
        std::cout << e.what();
    }


    return 0;
}
