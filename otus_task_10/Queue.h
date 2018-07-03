#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <vector>
#include <exception>
#include <tuple>
#include <functional>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

enum class RequestType{
    WriteToDisk
    , Quit
};


class Queue{

public:

    using QueueElement = std::tuple<RequestType, std::vector<std::string>, size_t>;

    Queue( const Queue& )            = delete;
    Queue( Queue&& )                 = delete;
    Queue& operator=( const Queue& ) = delete;
    Queue& operator=( Queue&& )      = delete;

    std::mutex queue_lock;

    std::condition_variable queue_is_not_empty;

    Queue( size_t sizeQueue ) : m_container( sizeQueue + 1 )
    {
        if( !sizeQueue )
        {
            throw std::invalid_argument( "The queue size should be > 0.\n" );
        }
    }

    bool push( RequestType request, const std::vector<std::string>& data, size_t time )
    {
        {
            std::lock_guard<std::mutex> lk( queue_lock );


            if(  m_popPos == incrementPosition( m_pushPos ) )
            {
                return false;
            }


            if( DEBUG ) { std::cout << "Inside the push with thread id : " << std::this_thread::get_id() << " \n";}
            if( DEBUG ) { std::cout << "m_pushPos is " << m_pushPos << "\n";}

            auto& item = m_container.at(m_pushPos);
            item = std::make_tuple( request, data, time );

            if( DEBUG ) { std::cout << "Pushing data: " << (int)request << " and time " << time << "\n";}

            m_pushPos = this->incrementPosition( m_pushPos );
        }

        queue_is_not_empty.notify_one();

        return true;
    }

    QueueElement pop()
    {
        std::unique_lock<std::mutex> lk( queue_lock );

        queue_is_not_empty.wait( lk, [this]()
                                {
                                    return ( this->incrementPosition( this->m_popPos ) !=  this->m_pushPos );
                                } );

        m_popPos = incrementPosition( m_popPos );

        if( DEBUG ) { std::cout << "Inside the pop with thread id : " << std::this_thread::get_id() << " \n";}

        if( DEBUG ) {std::cout << "m_popPos is " << m_popPos << "\n";}

        auto tmp = m_container.at(m_popPos);

        if( DEBUG ) { std::cout << "Poping data: " << (int)std::get<0>( tmp ) << "\n";}

        lk.unlock();

        return tmp;
    }


private:
    std::vector<QueueElement> m_container;
    size_t m_popPos = {0};
    size_t m_pushPos = {1};

    size_t incrementPosition(size_t pos) { return (pos != m_container.size() - 1) ? pos + 1 : 0; }
};

#endif // QUEUE_H_INCLUDED
