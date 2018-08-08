#include "Client.h"

void Client::start()
{
    unit->AddClient( shared_from_this() );

    spawn( ioservice, [this]( yield_context yield )
          {
              do_receive( yield );
          } );
}

void Client::do_receive( yield_context yield )
{
    try
    {
        while( true )
        {
            async_read( *tcp_socket, buffer( &data, 1 ), yield );

            if( data == '\n' )
            {
                unit->ProcessNewData( IncomingMessage, shared_from_this() );
                IncomingMessage = "";
            }
            else
            {
                IncomingMessage += data;
            }
        }
    }
    catch( ... )
    {
        unit->RemoveClient( shared_from_this() );
    }

}

void Client::StopClient()
{
    unit->RemoveClient( shared_from_this() );
}
