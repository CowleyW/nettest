#pragma once

#include "asio/io_context.hpp"
#include "asio/ip/tcp.hpp"
#include <string>

class Server {
public:
  static void start();

private:
  Server(asio::io_context context);

private:
  asio::io_context context;
  asio::ip::tcp::acceptor acceptor;
};
