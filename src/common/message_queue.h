#pragma once

#include "message.h"

#include <queue>
#include <vector>

#include <asio.hpp>

class MessageQueue {
public:
  MessageQueue(asio::ip::tcp::socket &socket);

  void begin();

  bool is_empty();

  Message dequeue();

private:
  void begin_enqueue_next();
  void finish_enqueue_next(MessageHeader header, size_t len);

private:
  asio::ip::tcp::socket &socket;
  std::queue<Message> queue;
  std::vector<char> buf;
};
