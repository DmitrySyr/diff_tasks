#pragma once

#include <iostream>

//=====================================
// helper for making tuple and convert to tuple
//=====================================

template<typename T, size_t... I>
auto Convert(const T& a, std::index_sequence<I...>)
{
    return std::make_tuple(a[I]...);
}

template<typename T, size_t N, typename Seq = std::make_index_sequence<N>>
auto Convert(const std::array<T, N>& a)
{
    return Convert(a, Seq{});
}

template<std::size_t N>
struct tuple_n
{
  using rest_tuple = typename tuple_n<N-1>::type;
  using type = decltype(std::tuple_cat(std::declval<std::tuple<int>>(), std::declval<rest_tuple>()));
};

template<>
struct tuple_n<0>
{
  using type = std::tuple<>;
};



//=====================================
//             print tuple
//=====================================


    template <typename  T>
    struct is_tuple
    {
        static const bool value = false;
    };

    template <typename ... Tp>
    struct is_tuple<std::tuple<Tp...> >
    {
        static const bool value = true;
    };

   template < typename T, size_t ... I>
    auto print_tuple_helper( const T& tp, std::index_sequence<I...>, std::ostream& out = std::cout ) {

        ((out << (I == 0 ? "" : "") << std::get<I>(tp)), ...);
    }

    template < typename T, typename std::enable_if<is_tuple<T>::value, int>::type = 0 >
    auto print_tuple( const T& tp, std::ostream& out = std::cout ) {

        print_tuple_helper( tp, std::make_index_sequence<std::tuple_size<T>::value>(), out );
    }

    template < typename T, typename std::enable_if<is_tuple<T>::value, int>::type = 0 >
    std::ostream& operator<<( std::ostream& out, const T& tp )
    {
        print_tuple( tp );
        return out;
    }

//=====================================
//               iterator
//=====================================

template<typename T, size_t N>
class iterator
{
    using Index       = std::array<int, N>;
    using CleverIndex = std::shared_ptr< Index >;

    using tupleHelper = typename tuple_n<N>::type;
    using Data        = std::map< Index, decltype( std::tuple_cat( std::declval<tupleHelper>(), std::declval<std::tuple<T>>() ) ) >;
    using cleverData  = std::shared_ptr< Data >;

    public:
        iterator() = delete;

        iterator( typename Data::iterator p ): CurrentV( p ) {}

        decltype( std::tuple_cat( std::declval<tupleHelper>(), std::declval<std::tuple<T>>() ) )& operator*()
        {
            return CurrentV -> second;
        }


        iterator& operator++()
        {
             ++(this->CurrentV);
            return *this;
        }

        iterator operator++( int ) = delete;

        iterator& operator--()
        {
             --(this->CurrentV);
            return *this;
        }

        iterator operator--( int ) = delete;

        bool operator==( const iterator& other ) const { return this->CurrentV == other.CurrentV; }
        bool operator!=( const iterator& other ) const { return this->CurrentV != other.CurrentV; }

    private:
        typename Data::iterator CurrentV;

};
