#pragma once

#include <unordered_map>
#include <memory>
#include <vector>
#include <map>
#include <array>
#include <tuple>

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

template <typename T, T filler, int N, int ind = N>
struct Matrix
{
    using Index = std::array<int, N>;
    using CleverIndex = std::shared_ptr< Index >;
    using Data  = std::shared_ptr< std::map< Index, T> >;

    Matrix<T, filler, N, ind>()
        : data( std::make_shared<std::map< Index, T>>() )
        , index( std::make_shared<Index>() )
        {}

    Matrix<T, filler, N, ind>(Data DataT, CleverIndex IndexT) : data( DataT ), index( IndexT ) {}

    Matrix<T, filler, N, ind-1> operator[]( int i ) const
    {
        if( i < 0 ) { throw std::exception(); }

        ( *index )[N-ind] = i;
        return Matrix<T, filler, N, ind-1>( data, index );
    }

    size_t size() const
    {
        return ( *data ).size();
    }


    class const_iterator
    {
        public:
            const_iterator() = delete;

            const_iterator( typename std::map< Index, T>::const_iterator p ): CurrentV( p ) {}

            auto operator*()
            {
                return std::tuple_cat( Convert( CurrentV->first ), std::make_tuple( CurrentV->second ) );
            }


            const_iterator& operator++()
            {
                 ++(this->CurrentV);
                return *this;
            }

            const_iterator operator++( int ) = delete;

            const_iterator& operator--()
            {
                 --(this->CurrentV);
                return *this;
            }

            const_iterator operator--( int ) = delete;

            bool operator==( const const_iterator& other ) const { return this->CurrentV == other.CurrentV; }
            bool operator!=( const const_iterator& other ) const { return this->CurrentV != other.CurrentV; }

        private:
            typename std::map< Index, T>::const_iterator CurrentV;

    };

    const_iterator begin() { return const_iterator( data -> begin() ); }
    const_iterator end()   { return const_iterator( data -> end() ); }

    auto& operator=(const T& value )
    {
        throw std::exception();
        return *this;
    }

    operator T() const
    {
        throw std::exception();
    }

    Data data;
    CleverIndex index;
};

template <typename T, T filler, int N>
struct Matrix<T, filler, N, 1>
{
    using Index = std::array<int, N>;
    using CleverIndex = std::shared_ptr< Index >;
    using Data  = std::shared_ptr< std::map< std::array<int, N>, T > >;

    Matrix<T, filler, N, 1>( Data DataT, CleverIndex IndexT )
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

    auto& operator=(const T& value )
    {
        if( value != filler )
        {
            ( *data )[*index] = value;
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
        return ( *data )[*index] == value;
    }

    operator T() const
    {
        if( !index_complete ) { throw std::exception(); }

        if( data -> find( *index ) != data -> end() )
            {
                return ( *data )[*index];
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
            out << ( *matrix.data )[( *matrix.index )];
        }
        else
        {
            out << filler;
        }

        return out;
    }

    Data data;
    CleverIndex index;
    bool index_complete;
};
