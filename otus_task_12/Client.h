#pragma once

#include <iostream>
#include <memory>

#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>

#include "H12.h"

#define STRING_MAX_SIZE 50

using namespace boost::asio;
using namespace boost::asio::ip;

class ReceivingBulk;

class Client : public std::enable_shared_from_this<Client> {

public:

    Client( std::unique_ptr<tcp::socket>&& sock
           , io_service& ioservice
           , std::shared_ptr<ReceivingBulk> unit ) :
                  tcp_socket( std::forward<std::unique_ptr<tcp::socket>>( sock ) )
                  , ioservice( ioservice )
                  , IncomingMessage( "" )
                  , unit( unit )
      {
          //std::cerr << "Client made.\n";
      }


    ~Client()
    {
        tcp_socket->shutdown( tcp::socket::shutdown_send );
        //std::cerr << "Client closed.\n";
    }

    void start();

    void do_receive( yield_context yield );

    void StopClient();

private:
    std::unique_ptr<tcp::socket> tcp_socket;
    io_service& ioservice;
    std::string IncomingMessage;
    char data;
    std::shared_ptr<ReceivingBulk> unit;
};
