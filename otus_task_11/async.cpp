#include "async.h"

auto handler = std::make_shared<Handler>();

namespace async {

    void* connect( std::size_t bulk )
    {
        if( !handler->good() )
        {
            return nullptr;
        }

        size_t ndx;

        try
        {
            ndx = ( handler->getIndex( bulk ) );
        }
        catch( std::exception& e )
        {
            std::cerr << e.what();
            handler->setStateBad();
            return nullptr;
        }

        return reinterpret_cast<void*>( ndx );
    }

    void receive( handle_t handle, const char *data, std::size_t size__ )
    {
        if( handler->good() )
        {
            auto indexForProceed = reinterpret_cast<size_t>( handle );

            try
            {
                handler->proceed( indexForProceed, data, size__ );
            }
            catch( std::exception& e )
            {
                std::cerr << e.what();
            }
        }


    }

    void disconnect(handle_t handle)
    {
        auto indexForProceed = reinterpret_cast<size_t>( handle );
        //handler->printStatistics( indexForProceed );
        handler->disconnect( handle );
    }

    void clear()
    {
        handler->clear();
    }

}
