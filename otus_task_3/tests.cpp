#include <map>
#include <tuple>
#include <iostream>
#include <sstream>


#include "m_list.h"
#include "pool_allocator.h"

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_main

#include <boost/test/unit_test.hpp>

#include <boost/test/output_test_stream.hpp>

BOOST_AUTO_TEST_SUITE(test_fact_func)

template <typename T>
constexpr T factorial(T n)
{
  if(n < 0) throw std::invalid_argument("Argument of factorial function should be positive.");

  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

BOOST_AUTO_TEST_CASE(test_factorial)
{
  BOOST_STATIC_ASSERT(factorial(0) == 1);
  BOOST_STATIC_ASSERT(factorial(1) == 1);
  BOOST_STATIC_ASSERT(factorial(2) == 2);
  BOOST_STATIC_ASSERT(factorial(5) == 120);

  BOOST_CHECK_THROW(factorial(-10), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

//============================================================

BOOST_AUTO_TEST_SUITE(test_suite_m_list)

BOOST_AUTO_TEST_CASE(test_custom_m_list_create)
{
  MList<int> test_list;
  BOOST_CHECK( test_list.empty() == true );
  BOOST_CHECK( test_list.begin() == test_list.end() );
}

BOOST_AUTO_TEST_CASE(test_custom_m_list_push_pop_front)
{

    MList<int> test_list;

    test_list.push_front( 123456789 );
    test_list.push_front( 987654321 );

    BOOST_CHECK( test_list.empty() == false );

    BOOST_CHECK( *( test_list.begin() ) == 987654321 );



    BOOST_CHECK( *( ++test_list.begin() ) == 123456789 );



    test_list.pop_front();

    BOOST_CHECK( *( test_list.begin() ) == 123456789 );

    test_list.pop_front();

    BOOST_CHECK( test_list.empty() == true );

}


BOOST_AUTO_TEST_CASE(test_custom_m_list_other_type)
{
  MList<float> test_list;
  BOOST_CHECK( test_list.empty() == true );

  MList<long> test_list1;
  BOOST_CHECK( test_list1.empty() == true );

  MList<bool> test_list2;
  BOOST_CHECK( test_list2.empty() == true );

  MList<uint64_t> test_list3;
  BOOST_CHECK( test_list3.empty() == true );
}



BOOST_AUTO_TEST_SUITE_END()

//============================================================

BOOST_AUTO_TEST_SUITE(test_allocator)

template <typename T>
constexpr T factorial(T n)
{
  if(n < 0) throw std::invalid_argument("Argument of factorial function should be positive.");

  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

BOOST_AUTO_TEST_CASE(test_my_allocator)
{
   auto gen_value_list = [i=0] () mutable {
            return i++;
        };

  const size_t n = 3;

  MList<int, MyAlliocator<Node<int>, n>> map_custom{};

  MList<int> map_standart{};

  std::generate_n(std::front_inserter( map_custom ),
                    n,
                    gen_value_list);

  std::generate_n(std::front_inserter( map_standart ),
                    n,
                    gen_value_list);

  BOOST_CHECK( map_standart.size() == map_custom.size() );
  BOOST_CHECK( map_custom.get_allocator().get_size() == n );
  BOOST_CHECK( map_custom.get_allocator().aviable_pool_size() == 0 );

  auto it_standart = map_standart.begin();
  auto it_custom   = map_custom.begin();

  for(;it_standart != map_standart.end(); ++it_standart, ++it_custom) {
    BOOST_CHECK(*it_custom == *it_standart);
  }

  map_standart.clear(); map_custom.clear();

  BOOST_CHECK( map_custom.size() == map_custom.get_allocator().get_size() );
  BOOST_CHECK( map_custom.get_allocator().aviable_pool_size() == n );
}

BOOST_AUTO_TEST_SUITE_END()
