#ifndef TH_WRAPPER_H_INCLUDED
#define TH_WRAPPER_H_INCLUDED

class thread_wrapper{

    std::thread this_thread{};

public:

    thread_wrapper() = delete;
    thread_wrapper( const thread_wrapper& ) = delete;
    thread_wrapper( thread_wrapper&& old_thread )
    {
        if( this != &old_thread )
        {
            this->this_thread = std::move( old_thread.this_thread );
        }
    }

    thread_wrapper( std::thread && new_thread ) :
        this_thread( std::move( new_thread ) ) {}

    ~thread_wrapper()
    {
        if( this_thread.joinable() )
        {
            this_thread.join();
        }
    }

};

#endif // TH_WRAPPER_H_INCLUDED
