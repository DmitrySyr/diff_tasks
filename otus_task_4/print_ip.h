#pragma once

#include <string>
#include <type_traits>
#include <vector>
#include <list>
#include <algorithm>
#include <tuple>
#include <iostream>
#include <array>


namespace HW4{

    //------------------------------------------------------

    template <typename T>
    struct is_container{

        static const bool value = false;
    };

    template <typename T, typename Alloc>
    struct is_container<std::vector<T, Alloc>>{

        static const bool value = true;
    };


    template <typename T, typename Alloc>
    struct is_container<std::list<T, Alloc>>{

        static const bool value = true;
    };

    //------------------------------------------------------

    template <typename  T>
    struct is_tuple{

        static const bool value = false;
    };

    template <typename ... Tp>
    struct is_tuple<std::tuple<Tp...> >{

        static const bool value = true;
    };
    //------------------------------------------------------

    template<typename T>
    auto print_ip(const std::basic_string<T>& str) {

        std::cout << str;
    }

    template <  typename T
              , typename std::enable_if<is_container<T>::value, int>::type = 0
              , typename std::enable_if<!(
                                         is_container<typename T::value_type>::value
                                         || is_tuple<typename T::value_type>::value
                                         ), int>::type = 0
              >

    auto print_ip(const T& vec) {

        for(auto it = vec.begin(); it != vec.end(); ++it) {

            if(it != vec.begin()){
                std::cout << ".";
            }

            std::cout << *it;
        }
    }

    template <  typename T
              , typename std::enable_if<is_container<T>::value, int>::type = 0
              , typename std::enable_if<(
                                         is_container<typename T::value_type>::value
                                         || is_tuple<typename T::value_type>::value
                                         )
                                         , int>::type = 0
              >
    auto print_ip(const T& vec) {

        for(auto it = vec.begin(); it != vec.end(); ++it) {

            print_ip( *it ); std::cout << "\n";
        }
    }


    template <typename T, typename std::enable_if<std::is_integral<T>::value, int>::type = 0>
    auto print_ip(const T& t) {

        auto tmp = t;
            std::array<int, sizeof(T)> vec;

            size_t count_ = 1;

            while(true) {

                vec[count_ - 1] = ( ( tmp&255 ) );
                if(count_ < sizeof(T)) {
                    tmp = tmp >> 8;
                    ++count_;
                }
                else {
                    break;
                }
            }

            std::reverse(vec.begin(), vec.end());

            for(auto it = vec.begin(); it != vec.end(); ++it) {

                if(it != vec.begin()){
                    std::cout << ".";
                }

                std::cout << *it;
            }
    }

    //------------------------------------------------------


    template < typename T, size_t ... I>
    auto print_tuple_helper( const T& tp, std::index_sequence<I...> ) {

        ((std::cout << (I == 0 ? "" : ".") << std::get<I>(tp)), ...);
    }

    template < typename T, typename std::enable_if<is_tuple<T>::value, int>::type = 0 >
    auto print_ip( const T& tp ) {

        print_tuple_helper( tp, std::make_index_sequence<std::tuple_size<T>::value>() );
    }
    //------------------------------------------------------

    template<typename T>
    void print(const T& t) {


       print_ip(t);
    }

} // end of namespace HW4
