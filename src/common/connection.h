#pragma once

#include "message.h"
#include "message_queue.h"

#include <asio.hpp>
#include <fmt/core.h>

/**
 * A class representing a TCP Connection
 */
class Connection {
public:
  Connection(asio::io_context &context);

  void write_message(Message &message);

  void begin_queue();

  bool has_message();
  Message get_message();

public:
  asio::ip::tcp::socket socket;

private:
  void handle_write(const asio::error_code &err, size_t len);

private:
  MessageQueue queue;
};
