#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <map>
#include <array>
#include <tuple>

#include "utils.h"

template <typename T, T filler, int N, int ind = N>
struct Matrix
{
    using Index       = std::array<int, N>;
    using CleverIndex = std::shared_ptr< Index >;

    using tupleHelper = typename tuple_n<N>::type;
    using Data        = std::map< Index, decltype( std::tuple_cat( std::declval<tupleHelper>(), std::declval<std::tuple<T>>() ) ) >;
    using cleverData  = std::shared_ptr< Data >;

    Matrix<T, filler, N, ind>()
        : data( std::make_shared< Data >() )
        , index( std::make_shared<Index>() )
        {}

    Matrix<T, filler, N, ind>( cleverData DataT, CleverIndex IndexT ) : data( DataT ), index( IndexT ) {}

    Matrix<T, filler, N, ind-1> operator[]( int i )
    {
        if( i < 0 ) { throw std::exception(); }

        ( *index )[N-ind] = i;
        return Matrix<T, filler, N, ind-1>( data, index );
    }

    size_t size() const
    {
        return data -> size();
    }



    iterator<T, N> begin() { return iterator<T, N>( data -> begin() ); }
    iterator<T, N> end()   { return iterator<T, N>( data -> end() ); }

    auto& operator=(const T& value )
    {
        throw std::exception();
        return *this;
    }

    operator T() const
    {
        throw std::exception();
    }

    cleverData data;
    CleverIndex index;
};

template <typename T, T filler, int N>
struct Matrix<T, filler, N, 1>
{
    using Index       = std::array<int, N>;
    using CleverIndex = std::shared_ptr< Index >;

    using tupleHelper = typename tuple_n<N>::type;
    using Data        = std::map< Index, decltype( std::tuple_cat( std::declval<tupleHelper>(), std::declval<std::tuple<T>>() ) ) >;
    using cleverData  = std::shared_ptr< Data >;

    Matrix<T, filler, N, 1>( cleverData DataT, CleverIndex IndexT )
        : data( DataT )
        , index( IndexT )
        , index_complete ( false ) {}

    auto& operator[]( int i )
    {
        if( i < 0 ) { throw std::exception(); }

        if( index_complete ) { throw std::exception(); }

        ( *index )[N-1] = i;
        index_complete = true;

        return *this;
    }

    const auto& operator[]( int i ) const
    {
        if( i < 0 ) { throw std::exception(); }

        if( index_complete ) { throw std::exception(); }

        ( *index )[N-1] = i;
        index_complete = true;

        return static_cast<const Matrix<T, filler, N, 1>&>( *this );
    }

    auto& operator=(const T& value )
    {
        if( value != filler )
        {
            ( *data )[*index] = std::tuple_cat( Convert( *index ), std::make_tuple( value ) );
        }
        else
        {
            if( data -> find( *index ) != data -> end() )
            {
                data -> erase( *index );
            }
        }

        return *this;
    }

    bool operator==(const T& value ) const
    {
        return std::get<N>( ( *data )[*index] ) == value;
    }

    operator T() const
    {
        if( !index_complete ) { throw std::exception(); }

        if( data -> find( *index ) != data -> end() )
            {
                return std::get<N>( ( *data )[*index] );
            }
        else
            {
                return filler;
            }
    }

    friend std::ostream& operator<<( std::ostream& out, const Matrix<T, filler, N, 1>& matrix)
    {
        if( ( matrix.data ) -> find( *matrix.index ) != ( matrix.data ) -> end() )
        {
            out << std::get<N>( ( *matrix.data )[( *matrix.index )] );
        }
        else
        {
            out << filler;
        }

        return out;
    }

    cleverData data;
    CleverIndex index;
    bool index_complete;
};
