#pragma once

#include <string>
#include <type_traits>
#include <vector>
#include <list>
#include <algorithm>
#include <tuple>
#include <iostream>


namespace PRNT{

    template <typename T, bool, bool >
    class print_ip {};


    template <>
    class print_ip<std::string, false, true>{

    public:

        static void print_pr(const std::string& str) {

            std::cout << str;
        }

    };

    template <typename T>
    class print_ip<T, false, true>{

    public:

        static void print_pr(const T& vec) {

            for(auto it = vec.begin(); it != vec.end(); ++it) {

                if(it != vec.begin()){
                    std::cout << ".";
                }

                std::cout << *it;
            }
        }

    };


    template <typename T>
    class print_ip<T, true, false>{

    public:


        static void print_pr(const T& t) {

            auto tmp = t;
            std::vector<int> vec;
            vec.reserve(sizeof(T));
            size_t count_ = 1;

            while(true) {

                vec.push_back((tmp&255));
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
    };

    //==================================================================

    template<size_t I = 0, typename... Tp>
    typename std::enable_if<I == sizeof...(Tp), void>::type
    print(const std::tuple<Tp...>& t)
    { }

    template<size_t I = 0, typename... Tp>
    typename std::enable_if<I < sizeof...(Tp), void>::type
    print(const std::tuple<Tp...>& t)
    {
        if(I != 0) {
            std::cout << ".";
        }
        std::cout << std::get<I>(t);
        print<I + 1, Tp...>(t);
    }


    template <typename ... Tp>
    class print_ip<std::tuple<Tp...>, true, false> {

    public:

        static void print_pr(const std::tuple<Tp...>& t) {

            print(t);
        }
    };


    //------------------------------------------------------
    template <typename T>
    struct is_container{

        static const bool value = false;
    };

    template <typename T, typename Alloc>
    struct is_container<std::vector<T, Alloc>>{

        static const bool value = true;
    };


     template <>
    struct is_container<std::string>{

        static const bool value = true;
    };

     template <typename T, typename Alloc>
    struct is_container<std::list<T, Alloc>>{

        static const bool value = true;
    };

    //------------------------------------------------------

    template<typename T>
    void print(const T& t) {

        print_ip<T, std::is_integral<T>::value, is_container<T>::value>::print_pr(t);

    }

} // end of namespace PRNT
