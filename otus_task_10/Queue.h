#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <deque>
#include <exception>
#include <tuple>
#include <functional>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

enum class RequestType{
    WriteToDisk
    , WriteToConsole
    , Quit
};


class Queue{

public:

    size_t Pushing{0};
    size_t Popping{0};

    std::atomic<size_t> file_id{0};

    using QueueElement = std::tuple<RequestType, std::string, size_t>;

    Queue( const Queue& )            = delete;
    Queue( Queue&& )                 = delete;
    Queue& operator=( const Queue& ) = delete;
    Queue& operator=( Queue&& )      = delete;

    std::mutex queue_lock;
    std::mutex write_mutex;
    std::condition_variable goAhead;

    Queue( ) : m_container( {} ){}

    void push( const RequestType request, const std::string& data, const size_t time )
    {

        {
            std::lock_guard<std::mutex> lk( queue_lock );

            m_container.push_back( std::make_tuple( request, data, time ) );

            ++Pushing;
        }

        goAhead.notify_one();
    }

    void pop( RequestType& state, std::string& data, size_t& time )
    {
        std::unique_lock<std::mutex> lk( queue_lock );

        goAhead.wait( lk, [this]
                     {
                         return !m_container.empty();
                     } );

        std::tie( state, data, time ) = m_container.front();
        m_container.pop_front();

        ++Popping;
    }

private:
    std::deque<QueueElement> m_container;

};

#endif // QUEUE_H_INCLUDED
