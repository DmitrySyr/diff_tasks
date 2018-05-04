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
    using Index = std::shared_ptr< std::array<int, N> >;
    using Data  = std::shared_ptr< std::map< std::array<int, N>, T> >;

    Matrix<T, filler, N, ind>()
        : data( std::make_shared<std::map< std::array<int, N>, T>>() )
        , index( std::make_shared<std::array<int, N>>() )
        {}

    Matrix<T, filler, N, ind>(Data data_, Index index_) : data( data_ ), index( index_ ) {}

    Matrix<T, filler, N, ind-1> operator[]( int i )
    {
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

            const_iterator( typename std::map< std::array<int, N>, T>::const_iterator p ): current_( p ) {}

            auto operator*()
            {
                return std::tuple_cat( Convert( current_->first ), std::make_tuple( current_->second ) );
            }


            const_iterator& operator++()
            {
                 ++(this->current_);
                return *this;
            }

            const_iterator operator++( int ) = delete;

            const_iterator& operator--()
            {
                 --(this->current_);
                return *this;
            }

            const_iterator operator--( int ) = delete;

            bool operator==( const const_iterator& other ) const { return this->current_ == other.current_; }
            bool operator!=( const const_iterator& other ) const { return this->current_ != other.current_; }

        private:
            typename std::map< std::array<int, N>, T>::const_iterator current_;

    };

    const_iterator begin() { return const_iterator( ( *data ).begin() ); }
    const_iterator end() { return const_iterator( ( *data ).end() ); }

    Data data;
    Index index;
};

template <typename T, T filler, int N>
struct Matrix<T, filler, N, 1>
{
    using Index = std::shared_ptr< std::array<int, N> >;
    using Data  = std::shared_ptr< std::map< std::array<int, N>, T > >;

    Matrix<T, filler, N, 1>( Data data_, Index index_ )
        : data( data_ )
        , index( index_ ) {}

    auto& operator[]( int i )
    {
        ( *index )[N-1] = i;
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
            if( ( *data ).find( *index ) != ( *data ).end() )
            {
                ( *data ).erase( *index );
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
        if( ( *data ).find( *index ) != ( *data ).end() )
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
        if( ( *matrix.data ).find( *matrix.index ) != ( *matrix.data ).end() )
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
    Index index;
};
